from sklearn.datasets import make_blobs
import matplotlib.pyplot as plt
import numpy as np

n_samples_1 = 1000000
n_samples_2 = 1000000
centers = [[0.5, -1.0], [1.0, -5.0]]
clusters_std = [0.4, 0.6]
data, labels = make_blobs(
    n_samples=[n_samples_1, n_samples_2],
    centers=centers,
    cluster_std=clusters_std,
    random_state=np.random.randint(0, 2 ** 32 - 1),
    shuffle=False,
)

fig, ax = plt.subplots()

colours = ["red", "blue"]
label_name = ["A", "B"]
for label in range(0, 2):
    ax.scatter(data[labels==label, 0], data[labels==label, 1],
               c=colours[label], s=10, label=label_name[label])


ax.set(xlabel='X',
       ylabel='Y',
       title='dataset')


ax.legend(loc='upper right')

plt.savefig('data.png')

np.savetxt('data.txt', data)
np.savetxt('labels.txt', labels, fmt='%i')