import numpy as np
import matplotlib.pyplot as plt

X, y = np.loadtxt('data.txt'), np.loadtxt('labels.txt')

def plot(data, labels):
    fig, ax = plt.subplots()

    colours = ["red", "blue"]
    label_name = ["A", "B"]
    for label in [0, 1]:
        ax.scatter(data[labels==label, 0], data[labels==label, 1],
                   c=colours[label], s=10, label=label_name[label])

    ax.set(xlabel='X',
           ylabel='Y',
           title='HESS')

    ax.legend(loc='upper right')

    plt.savefig('hess.png')

import sys

c_opt = eval(sys.argv[1])
w_opt = eval(sys.argv[2])

hess_pred = np.asarray([int(np.dot(w_opt, x) + c_opt >= 0) for x in X])

plot(X, hess_pred)

from sklearn import metrics
from sklearn import svm

clf = svm.SVC(kernel='linear')
clf.fit(X, y)
y_pred = clf.predict(X)

print("Accuracy SVC:",metrics.accuracy_score(y, y_pred))
print("Accuracy HESS:",metrics.accuracy_score(y, hess_pred))