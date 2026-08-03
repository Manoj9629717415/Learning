import torch.nn as nn
import torch
import matplotlib.pyplot as plt
from torchvision import datasets, transforms
import torchvision.models as models
import os
from PIL import Image

from torchvision.models import ResNet

from TraintestData import customresnet18

dir = r"C:\Users\AppConnect\Downloads\GTSRB-Training_fixed\GTSRB\Training"

class VisulaizeModels(nn.Module):
    def __init__(self):
        super().__init__()

        self.model = resnet18(weights="DEFAULT")

    def forward(self,image):
        x = image.unsqueeze(0)

        layer1_conv = self.conv1(x)


def visulazie_layers_image():
    transform = transforms.Compose([
        transforms.Resize((224, 224)),
        transforms.ToTensor(),
        transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
    ])
    # ----------------------------
    # Load ONE image from GTSRB
    # ----------------------------
    transform = transforms.Compose([
        transforms.Resize((64, 64)),
        transforms.ToTensor()
    ])
    dataset = datasets.GTSRB(
        root=dir,
        split="train",
        transform=transform
    )
    image, label = dataset[0]
    print("Image shape:", image.shape)  # (3,64,64)
    # Add batch dimension
    x = image.unsqueeze(0)
    # ----------------------------
    # Tiny CNN
    # ----------------------------
    conv1 = nn.Conv2d(3, 8, kernel_size=3)
    relu1 = nn.ReLU()
    pool1 = nn.MaxPool2d(2)
    conv2 = nn.Conv2d(8, 16, kernel_size=3)
    relu2 = nn.ReLU()
    pool2 = nn.MaxPool2d(2)
    # ----------------------------
    # Forward pass
    # ----------------------------
    f1 = conv1(x)
    a1 = relu1(f1)
    p1 = pool1(a1)
    f2 = conv2(p1)
    a2 = relu2(f2)
    p2 = pool2(a2)
    print("Conv1 output:", f1.shape)
    print("Conv2 output:", f2.shape)
    # ----------------------------
    # Create folders
    # ----------------------------
    os.makedirs("feature_maps/conv1", exist_ok=True)
    os.makedirs("feature_maps/conv2", exist_ok=True)
    # ----------------------------
    # Save Conv1 Feature Maps
    # Shape: (1,8,H,W)
    # ----------------------------
    for i in range(f1.shape[1]):
        fmap = f1[0, i].detach().numpy()

        plt.figure(figsize=(4, 4))
        plt.imshow(fmap, cmap="viridis")
        plt.axis("off")
        plt.title(f"Conv1 Feature Map {i + 1}")

        plt.savefig(f"feature_maps/conv1/map_{i + 1}.png",
                    bbox_inches="tight")
        plt.close()
    # ----------------------------
    # Save Conv2 Feature Maps
    # Shape: (1,16,H,W)
    # ----------------------------
    for i in range(f2.shape[1]):
        fmap = f2[0, i].detach().numpy()

        plt.figure(figsize=(4, 4))
        plt.imshow(fmap, cmap="viridis")
        plt.axis("off")
        plt.title(f"Conv2 Feature Map {i + 1}")

        plt.savefig(f"feature_maps/conv2/map_{i + 1}.png",
                    bbox_inches="tight")
        plt.close()

    fig, axes = plt.subplots(4, 4, figsize=(10, 5))
    for i in range(8):
        axes[i // 4, i % 4].imshow(
            f1[0, i].detach().numpy()
        )
        axes[i // 4, i % 4].set_title(f"FM {i + 1}")
        axes[i // 4, i % 4].axis("off")

        axes[(i // 4) + 2, i % 4].imshow(
            f2[0, i].detach().numpy()
        )
        axes[(i // 4) + 2, i % 4].set_title(f"FM2 {i + 1}")
        axes[(i // 4) + 2, i % 4].axis("off")
    plt.tight_layout()
    plt.show()
    print("All feature maps saved.")

def restnet_layers_visu():
    model = models.resnet18(weights="DEFAULT")

    weights = model.conv1.weight.data.cpu()

    fig, axes = plt.subplots(8, 8, figsize=(12, 12))

    for i, ax in enumerate(axes.flat):
        kernel = weights[i]

        # Convert (3,7,7) -> (7,7,3)
        kernel = kernel.permute(1, 2, 0).numpy()

        # Normalize for display
        # kernel = (kernel - kernel.min()) / (
        #         kernel.max() - kernel.min() + 1e-8
        # )

        ax.imshow(kernel)
        ax.axis("off")
        ax.set_title(f"K{i}")

    plt.tight_layout()
    plt.show()

def layer(input_channel,outputchannel,kernal_size,x):
    conv1 = nn.Conv2d(in_channels=input_channel,out_channels=outputchannel,kernel_size=kernal_size)
    relu = nn.ReLU()
    maxpool2d = nn.MaxPool2d(2)

    y = conv1(x)

    y = relu(y)

    y = maxpool2d(y)

    return y



def each_layer_visu():
    transform = transforms.Compose([
        transforms.Resize((224, 224)),
        transforms.ToTensor(),
        transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
    ])

    image = Image.open(r"C:\Users\AppConnect\Downloads\GTSRB-Training_fixed\GTSRB\Training\00037\00000_00001.ppm").convert("RGB")
    image = transform(image)
    image = image.unsqueeze(0)

    layer1 = layer(3,64,3,image)

    layer2 = layer(64,128,3,layer1)

    layer3 = layer(128,256,3,layer2)

    layer4 = layer(256,512,3,layer3)

    print(layer1.shape)
    print(layer2.shape)
    print(layer3.shape)
    print(layer4.shape)

    fig,axs = plt.subplots(2,2)

    axs[0][0].imshow(layer1[0][0].detach().numpy())

    axs[0][1].imshow(layer2[0][0].detach().numpy())

    axs[1][0].imshow(layer3[0][0].detach().numpy())

    axs[1][1].imshow(layer4[0][0].detach().numpy())

    plt.show()

activations = {}

def get_activation(name):
    def hook(model, input, output):
        activations[name] = output.detach().cpu()
    return hook


def visualize_feature_maps(feature_tensor,
                           layer_name,
                           num_maps=16):

    feature_tensor = feature_tensor[0]

    scores = feature_tensor.mean(dim=(1,2))

    top_indices = torch.topk(scores, num_maps).indices

    fig, axes = plt.subplots(4, 4, figsize=(20,20))

    for i,idx in enumerate(top_indices):

        fmap = feature_tensor[idx].cpu().numpy()

        ax = axes[i // 4, i % 4]

        ax.imshow(fmap, cmap="viridis")
        ax.set_title(
            f"Ch {idx.item()}\n{scores[idx]:.3f}"
        )
        ax.axis("off")

    plt.suptitle(f"Top {num_maps} Activations - {layer_name}")
    plt.tight_layout()
    plt.show()

def resnet_pre_trained_layer_visu():
    model = models.resnet18(weights="DEFAULT")

    model.eval()

    model.conv1.register_forward_hook(get_activation("conv1"))
    model.layer1.register_forward_hook(get_activation("layer1"))
    model.layer2.register_forward_hook(get_activation("layer2"))
    model.layer3.register_forward_hook(get_activation("layer3"))
    model.layer4.register_forward_hook(get_activation("layer4"))

    transform = transforms.Compose([
        transforms.Resize((224, 224)),
        transforms.ToTensor(),
        transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
    ])

    image = Image.open(r"C:\Users\AppConnect\Downloads\GTSRB-Training_fixed\GTSRB\Training\00037\00000_00001.ppm").convert(
        "RGB")
    image = transform(image)
    image = image.unsqueeze(0)

    with torch.no_grad():
        _ = model(image)

    for name, feat in activations.items():
        print(name, feat.shape)

    #visualize_feature_maps(activations["conv1"],"conv1")
    #visualize_feature_maps(activations["layer1"],"layer1")
    #visualize_feature_maps(activations["layer2"],"layer2")
    visualize_feature_maps(activations["layer3"],"layer3")
    #visualize_feature_maps(activations["layer4"],"layer4",64)

def resnet_local_trained_layer_visu():
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    model = customresnet18(43)
    model.to(device)

    model.load_state_dict(torch.load("unfrozen_All_layer_traffic_sign.pth"))

    model.eval()

    model.model.conv1.register_forward_hook(get_activation("conv1"))
    model.model.layer1.register_forward_hook(get_activation("layer1"))
    model.model.layer2.register_forward_hook(get_activation("layer2"))
    model.model.layer3.register_forward_hook(get_activation("layer3"))
    model.model.layer4.register_forward_hook(get_activation("layer4"))

    image = Image.open(r"C:\Users\AppConnect\Downloads\GTSRB-Training_fixed\GTSRB\Training\00017\00000_00001.ppm").convert("RGB")

    transform = transforms.Compose([
        transforms.Resize((224, 224)),
        transforms.ToTensor(),
        transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
    ])

    image = transform(image)
    image = image.unsqueeze(0)

    with torch.no_grad():
        image = image.to(device)
        _ = model(image)

    for name, feat in activations.items():
        print(name, feat.shape)

    # visualize_feature_maps(activations["conv1"],"conv1")
    visualize_feature_maps(activations["layer1"],"layer1")
    # visualize_feature_maps(activations["layer2"],"layer2")
    # visualize_feature_maps(activations["layer3"],"layer3")
    #visualize_feature_maps(activations["layer4"],"layer4")

#visulazie_layers_image()
#restnet_layers_visu()
#each_layer_visu()
# resnet_pre_trained_layer_visu()
resnet_local_trained_layer_visu()