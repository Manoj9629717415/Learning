import torch
from ultralytics import YOLO
import torch.nn as nn
from torchvision import transforms
from PIL import Image
import matplotlib.pyplot as plt
from ultralytics.utils.torch_utils import model_info


class customc2ftask(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv1 = nn.Conv2d(3,16,3,padding=1)
        self.batch1 = nn.BatchNorm2d(16)
        self.relu = nn.SiLU()

        self.conv2 = nn.Conv2d(16,32,3,padding=1)
        self.batch2 = nn.BatchNorm2d(32)

        self.conv3 = nn.Conv2d(32,64,3,padding=1)
        self.batch3 = nn.BatchNorm2d(64)

        self.conv4 = nn.Conv2d(32,32,3,padding=1)
        self.batch_b1 = nn.BatchNorm2d(32)

        self.conv5 = nn.Conv2d(32,32,3,padding=1)
        self.batch_b2 = nn.BatchNorm2d(32)

        self.conv6 = nn.Conv2d(32, 32, 3,padding=1)
        self.batch_b3 = nn.BatchNorm2d(32)

        self.conv7 = nn.Conv2d(128,64,1,padding=1)
        self.batch7 = nn.BatchNorm2d(64)



    def split(self,x):
        x1,x2 = torch.chunk(x,2,dim=1)
        return x1,x2

    def concat(self,x1,x2):
        return torch.cat([x1,x2],dim=1)

    def forward(self,x):
        y = self.conv1(x)
        y = self.batch1(y)
        y = self.relu(y)

        y = self.conv2(y)
        y = self.batch2(y)
        y = self.relu(y)

        y = self.conv3(y)
        y = self.batch3(y)
        y = self.relu(y)

        x1,x2 = self.split(y)

        b1 = self.conv4(x2)
        b1 = self.batch_b1(b1)
        b1 = self.relu(b1)

        b2 = self.conv5(b1)
        b2 = self.batch_b2(b2)
        b2 = self.relu(b2)

        b3 = self.conv6(b2)
        b3 = self.batch_b3(b3)
        b3 = self.relu(b3)

        y = torch.cat([x1,b1,b2,b3],dim=1)

        y = self.conv7(y)
        y = self.batch7(y)
        y= self.relu(y)

        return y

def train():
    model = YOLO(model="yolov8n.pt")

    # results = model.train(data="coco128.yaml",epochs=1,imgsz=640,batch=16)

    for name, param in model.named_parameters():
        print(name, param.grad)
        if param.grad is not None:
            print(name, param.grad.norm())

def train_yolo_s():
    model = YOLO(model="yolov8s.pt")

    results = model.train(data="coco128.yaml",epochs=10,imgsz=640,batch=16)

def validation():
    model = YOLO("runs/detect/train/weights/best.pt")

    metrics = model.val()

    print(metrics)

def inference():
    model = YOLO("runs/detect/train/weights/best.pt")

    result = model("000000000531.jpg",conf=0.50,save=True)
    print(f"result:{result}")

def testc2f_implemenatation():

    transform = transforms.Compose([
        transforms.Resize((224, 224)),
        transforms.ToTensor(),
        transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
    ])

    image = Image.open(
        r"C:\Users\AppConnect\Downloads\GTSRB-Training_fixed\GTSRB\Training\00037\00000_00001.ppm").convert("RGB")
    image = transform(image)
    image = image.unsqueeze(0)

    model = customc2ftask()

    output = model(image)

    fig, axes = plt.subplots(4, 4, figsize=(10, 5))
    for i in range(8):
        axes[i // 4, i % 4].imshow(
            output[0, i].detach().numpy()
        )
        axes[i // 4, i % 4].set_title(f"FM {i + 1}")
        axes[i // 4, i % 4].axis("off")
    plt.show()
    print("All feature maps saved.")

class custom_SPPF(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv1 = nn.Conv2d(512, 256, 1)
        self.b1 = nn.BatchNorm2d(256)
        self.silu = nn.SiLU()
        self.pool1 = nn.MaxPool2d(kernel_size=5, stride=1, padding=2)
        self.pool2 = nn.MaxPool2d(kernel_size=5, stride=1, padding=2)
        self.pool3 = nn.MaxPool2d(kernel_size=5, stride=1, padding=2)
        self.conv2 = nn.Conv2d(1024, 512, 1)

    def forward(self,x):
        y= self.conv1(x)

        print(f"conv1 shape {y.shape} paramter {sum(p.numel() for p in self.conv1.parameters())}")

        y = self.b1(y)

        print(f"Batch Norm shape {y.shape} paramter {sum(p.numel() for p in self.b1.parameters())}")

        y=self.silu(y)

        print(f"silU shape {y.shape} paramter {sum(p.numel() for p in self.silu.parameters())}")

        b1 = self.pool1(y)

        print(f"pool1 shape {b1.shape} paramter {sum(p.numel() for p in self.pool1.parameters())}")


        b2 = self.pool2(b1)

        print(f"pool2 shape {b2.shape} paramter {sum(p.numel() for p in self.pool2.parameters())}")

        b3=self.pool3(b2)

        print(f"pool3 shape {b3.shape} paramter {sum(p.numel() for p in self.pool3.parameters())}")



        y = torch.cat([y,b1,b2,b3],dim=1)

        print(f"concat shape {y.shape}")

        y = self.conv2(y)

        visulaization(b3, y)

        print(f"conv2 shape {y.shape} paramter {sum(p.numel() for p in self.conv2.parameters())}")

        return y

def visulaization(layer1,layer2=None):
    fig, axes = plt.subplots(4, 4, figsize=(10, 5))
    for i in range(8):
        axes[i // 4, i % 4].imshow(
            layer1[0, i].detach().numpy()
        )
        axes[i // 4, i % 4].set_title(f"FM {i + 1}")
        axes[i // 4, i % 4].axis("off")
        if layer2 is not None:
            axes[(i // 4) + 2, i % 4].imshow(
                layer2[0, i].detach().numpy()
            )
            axes[(i // 4) + 2, i % 4].set_title(f"FM2 {i + 1}")
            axes[(i // 4) + 2, i % 4].axis("off")
    plt.tight_layout()
    plt.show()
    print("All feature maps saved.")



def own_SPPF_block():

    x = torch.randn(1,512,20,20)

    model = custom_SPPF()

    y = model(x)

    print(f"parameter {sum(p.numel()  for p in model.parameters())}")

    print(y.shape)

def onechannel_exp():
    pool1 = nn.MaxPool2d(kernel_size=5,stride=1,padding=2)
    pool2 = nn.MaxPool2d(kernel_size=5, stride=1, padding=2)
    pool3 = nn.MaxPool2d(kernel_size=5, stride=1, padding=2)

    x = torch.zeros((1,1,20,20))
    x[0,0,10,10]=10

    p1 = pool1(x)
    p2 = pool2(p1)
    p3 = pool3(p2)

    print(p1.shape)
    print(p2.shape)
    print(p3.shape)
    fig, axes = plt.subplots(1,4,figsize=(10,5))
    axes[0].imshow(x[0, 0].detach().numpy())
    axes[1].imshow(p1[0,0].detach().numpy())
    axes[2].imshow(p2[0,0].detach().numpy())
    axes[3].imshow(p3[0,0].detach().numpy())
    plt.show()


train()
#validation()
# inference()
# train_yolo_s()
# testc2f_implemenatation()
# own_SPPF_block()
# onechannel_exp()
