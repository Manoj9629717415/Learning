from cProfile import label

from sklearn.linear_model import LinearRegression
import numpy as np
import matplotlib.pyplot as plt


def real_estate():
    size = np.array([
        [1000],
        [1200],
        [1500],
        [1800]
    ])
    price = np.array([200000,
                      240000,
                      300000,
                      360000])
    model = LinearRegression()
    model.fit(size, price)
    price_ = model.predict([[2000]])
    print(price_)

def jobmarket():
    experience = np.array([[1],[2],[3],[4],[5]])
    salary = np.array([30,35,40,50,60])

    model = LinearRegression()
    model.fit(experience,salary)
    predicted_sralary = model.predict([[7]])
    predict_linear = model.predict(experience)

    print(f" salary for 7 yeras {predicted_sralary} weighted {model.coef_} bias {model.intercept_}")

    plt.scatter(experience,salary,label ="Actual data")
    plt.plot(experience,predict_linear)
    plt.show()


if __name__ == "__main__":
    #real_estate()
    jobmarket()
