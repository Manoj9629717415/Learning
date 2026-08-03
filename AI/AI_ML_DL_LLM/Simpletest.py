import torch
import matplotlib.pyplot as plt

def test1():
    logits = torch.tensor([-10, -5, -2, 0, 2, 5, 10], dtype=torch.float32)
    print(torch.sigmoid(logits))

def test2():
    logits = torch.arange(-10, 11, 1, dtype=torch.float32)
    probabilities = torch.sigmoid(logits)
    print(logits)
    print(probabilities)
    plt.plot(logits,probabilities)
    plt.show()

def test3():
    logits = torch.linspace(-10,10,100)
    p = torch.sigmoid(logits)

    grad = p * (1-p)

    plt.plot(p)
    plt.plot(grad)
    plt.show()


def test4():
    pred = torch.tensor([0.1, 0.5, 0.9])

    target = torch.tensor([1.0, 1.0, 1.0])

    target1 = torch.tensor([0.0, 0.0, 0.0])

    gradient = pred - target

    gradient1 = pred - target1

    print(f"target= 1  gradient = {gradient}")
    print(f"target= 0  gradient = {gradient1}")

def backward_example():
    x = torch.tensor([2.0, 3.0])

    w = torch.tensor([1.0, 2.0], requires_grad=True)

    b = torch.tensor(0.5, requires_grad=True)

    z = (x * w).sum() + b

    p = torch.sigmoid(z)

    target = torch.tensor(1.0)

    loss = torch.nn.functional.binary_cross_entropy(p, target)

    loss.backward()
    print(z)

    print(p)

    print(loss)

    print("dw =", w.grad)

    print("db =", b.grad)

def back_example2():
    w = torch.tensor(2.0, requires_grad=True)
    x = torch.tensor(3.0)

    y = w * x
    z = y + 4
    loss = z ** 2

    loss.backward()

    print("y =", y.item())
    print("z =", z.item())
    print("Loss =", loss.item())
    print("Gradient =", w.grad.item())

def test5():
    predictions = [
        {"name": "A", "iou": 0.90, "conf": 0.30},
        {"name": "B", "iou": 0.75, "conf": 0.82},
        {"name": "C", "iou": 0.60, "conf": 0.95},
        {"name": "D", "iou": 0.40, "conf": 0.99},
    ]

    for p in  predictions:
        p["score"] = p["iou"] * p["conf"]

    predictions = sorted(predictions,key= lambda x:x["score"],reverse=True)

    print(predictions)

#test1()
#test2()
#test3()
# test4()
# backward_example()
# back_example2()
test5()