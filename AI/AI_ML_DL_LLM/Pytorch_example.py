import torch
import torch.nn as nn
from torch.utils.data import dataset
from torch.utils.data import dataloader

class customDataSet(dataset):
    def __init__(self,X,y):
        self.X = X
        self.y = y

    def __getitem__(self, item):
        return (self.X[item],self.y[item])

class Mymodel(nn.module):
    def __init__(self,X,y):
        super.__init__()
        self.fc1 = nn.linear(2,4)
        self.relu = ReLU()
        self.fc2 = nn.linear(4,1)

    def forward(self):
        x = self.fc1

        x = ReLU(x)

        x = self.fc2(x)
        return x


def sample():
    model = nn.Sequential(
        nn.Linear(2, 4),
        nn.ReLU(),
        nn.Linear(4, 1)
    )

    x = torch.tensor([[2.0, 3.0]])

    y = model(x)

    print(y)

def day5():
    X = torch.tensor([[4.0,3.0]])
    y = 5.0
    mymodel = Mymodel(X,y)

    mymodel.forward()

