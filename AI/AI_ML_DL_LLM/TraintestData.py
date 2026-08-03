import datetime

import torch.optim
from networkx.algorithms.threshold import weights_to_creation_sequence
from torch.nn.functional import cross_entropy
from torch.utils.data import Dataset
from torch.utils.data import DataLoader
from torch.utils.data import random_split
from torchvision import datasets
from torchvision import transforms
from torchvision.models import resnet18
import torch.nn as nn
import os
import pandas as pd
from PIL import Image
from datetime import datetime

dir = r"C:\Users\AppConnect\Downloads\GTSRB-Training_fixed\GTSRB\Training"

class Trafficsigndataset(Dataset):
    def __init__(self,datasetfolder,transform=None,use_roi=False):
        self.root_dir = datasetfolder
        self.samples = []
        self.transform = transform
        self.use_roi = use_roi
        self.details = {}
        for classfoldername in sorted(os.listdir(datasetfolder)):
            classpath = os.path.join(datasetfolder,classfoldername)

            if not os.path.isdir(classpath):
                continue

            csv_file = None

            for file in os.listdir(classpath):
                if file.endswith(".csv"):
                    csv_file = os.path.join(classpath,file)
                    break

            if csv_file is None:
                continue

            csv_df = pd.read_csv(csv_file,sep =';')

            for _,row in csv_df.iterrows():
                sample = {"image_name":os.path.join(classpath,row["Filename"]) ,
                             "label":int(row["ClassId"]),
                             "roi":(int(row["Roi.X1"]),int(row["Roi.Y1"]),int(row["Roi.X2"]),int(row["Roi.Y2"]))}
                self.samples.append(sample)

    def __getitem__(self, index):
        sample = self.samples[index]

        image = Image.open(sample["image_name"]).convert("RGB")

        if self.use_roi:
            x1,y1,x2,y2 = sample["roi"]
            image = image.crop((x1,y1,x2,y2))

        label = sample["label"]

        if self.transform:
            image = self.transform(image)

        return image,label

    def getdatesetdetails(self):
        return self.details

    def __len__(self):
        return len(self.samples)

class Custom_Trafficsign_Dataset_GT(Dataset):
    def __init__(self,root,transform=None,use_roi=False):
        self.transfom = transform
        self.use_roi = use_roi
        self.test_data = []
        self.root = root

        csv_file = None
        for file in os.listdir(root):
            if file.endswith('.csv'):
                csv_file = os.path.join(root,file)
                break

        if csv_file is not None:
            test_df = pd.read_csv(csv_file,sep=';')
            for _,row in test_df.iterrows():
                sample = {"image_name":os.path.join(root,row["Filename"]),"label":int(row["ClassId"]),
                          "roi":(int(row["Roi.X1"]),int(row["Roi.Y1"]),int(row["Roi.X2"]),int(row["Roi.Y2"]))}
                self.test_data.append(sample)

    def __getitem__(self, item):
        sample = self.test_data[item]
        image = Image.open(sample["image_name"]).convert("RGB")
        if self.use_roi:
            x1,y1,x2,y2 = sample["roi"]
            image = image.crop((x1,y1,x2,y2))

        label = sample["label"]

        if self.transfom:
            image = self.transfom(image)
        return image,label

    def __len__(self):
        return len(self.test_data)


class customresnet18(nn.Module):
    def __init__(self,num_classes,fine_tune=False):
        super().__init__()

        self.model = resnet18(weights="DEFAULT")

        in_features = self.model.fc.in_features

        self.model.fc = nn.Linear(in_features, num_classes)

        for param in self.model.parameters():
            param.requires_grad = False

    def unfreeze_lastLayer(self):
        self.model.fc.weight.requires_grad = True
        self.model.fc.bias.requires_grad = True

    def unfreeze_layer4(self):
        for param in self.model.layer4.parameters():
            param.requires_grad = True

    def unfreezeAllLayers(self):
        for param in self.model.parameters():
            param.requires_grad = True

    def forward(self,x):
        return self.model(x)

def train_model(model,epochs=5,model_name="default.pth"):
    model.train()
    criterion  = nn.CrossEntropyLoss()
    optimizer = torch.optim.Adam(
        filter(lambda p: p.requires_grad, model.model.parameters()), lr=0.001)

    trainable = sum(param.numel() for param in model.model.parameters() if param.requires_grad)

    print(f"Trainable Parameters: {trainable}")

    for epoch in range(epochs):
        running_loss = 0.0
        for image, label in train_loader:
            optimizer.zero_grad()
            image = image.to(device)
            label = label.to(device)
            predict = model(image)
            loss = criterion(predict, label)
            loss.backward()
            optimizer.step()
            running_loss += loss.item()
        print(
            f"Epoch {epoch + 1}/{epochs} "
            f"Loss: {running_loss / len(train_loader):.4f}"
        )
    torch.save(
        model.state_dict(),
        model_name
    )

def evaluate_model_accuracy(model, loader,device=None):
    correct = 0
    total = 0
    model.model.eval()
    with torch.no_grad():
        for image,label in loader:
            image = image.to(device)
            label = label.to(device)

            predict = model(image)

            _,pred = torch.max(predict,1)

            correct += (pred==label).sum().item()

            print(f"image:{pred}")
            print(f"label:{label}")

            total += label.size(0)

    accuracy = 100 * (correct / total)
    return accuracy



transform = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
    transforms.Normalize(mean=[0.485,0.456,0.406],std=[0.229,0.224,0.225])
])


def Train_and_eval():
    global train_loader, device, model
    trafficdataset = Trafficsigndataset(datasetfolder=dir, transform=transform, use_roi=True)
    print(trafficdataset.__len__())
    classes = set(x["label"] for x in trafficdataset.samples)
    print(len(classes))
    train_size = int(0.8 * len(trafficdataset))
    val_size = len(trafficdataset) - train_size
    generator = torch.Generator().manual_seed(42)
    train_dataset, val_dataset = random_split(trafficdataset, [train_size, val_size], generator=generator)
    train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)
    val_loader = DataLoader(val_dataset, batch_size=32, shuffle=False)
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = customresnet18(len(classes))
    model.to(device)
    model.unfreeze_lastLayer()
    print("=" * 50)
    print(f"Experiment: FC Only start time {datetime.now()}")
    train_model(model, model_name="unfrozen_FC_layer_traffic_sign.pth")
    print(f"Experiment: FC Only finish time {datetime.now()}")
    print(f"unfrozen_last_layer train Accuracy {evaluate_model_accuracy(model, train_loader,device)}")
    print(f"unfrozen_last_layer test Accuracy {evaluate_model_accuracy(model, val_loader,device)}")

    model_layer4 = customresnet18(len(classes))
    model_layer4.unfreeze_lastLayer()
    model_layer4.unfreeze_layer4()
    model_layer4.to(device)
    print("=" * 50)
    print(f"Experiment: FC and layer4 start time {datetime.now()}")
    train_model(model_layer4, model_name="unfrozen_layer4_traffic_sign.pth")
    print(f"Experiment: FC Only finish time {datetime.now()}")
    print(f"unfrozen_layer4 train Accuracy {evaluate_model_accuracy(model_layer4, train_loader, device)}")
    print(f"unfrozen_layer4 test Accuracy {evaluate_model_accuracy(model_layer4, val_loader, device)}")

    model_heavy = customresnet18(len(classes))
    model_heavy.to(device)
    model_heavy.unfreezeAllLayers()
    print("=" * 50)
    print(f"Experiment: all layer start time {datetime.now()}")
    train_model(model_heavy, model_name="unfrozen_All_layer_traffic_sign.pth")
    print(f"Experiment: all layer finish time {datetime.now()}")
    print(f"unfrozen_All_layer train Accuracy {evaluate_model_accuracy(model_heavy, train_loader,device)}")
    print(f"unfrozen_All_layer test Accuracy {evaluate_model_accuracy(model_heavy, val_loader,device)}")


def Test_with_GT():
    test_dir = r"C:\Users\AppConnect\Downloads\GTSRB_Final_Test_Images\GTSRB\Final_Test\Images"
    testDataset = Custom_Trafficsign_Dataset_GT(root=test_dir,transform=transform,use_roi=True)

    loader = DataLoader(dataset=testDataset,batch_size=32,shuffle=False)

    model = customresnet18(43)

    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    model.load_state_dict(torch.load("unfrozen_All_layer_traffic_sign.pth"))
    model.to(device)

    print(f"trained tested with GT Accuracy {evaluate_model_accuracy(model, loader,device)}")

if __name__ == "__main__":
    #Train_and_eval()
    Test_with_GT()