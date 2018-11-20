import os
import kdtree
import numpy as np
import pickle

class Item(object):
    def __init__(self, data, name):
        self.name = name
        self.data = data
    def __len__(self):
        return len(self.data)
    def __getitem__(self, i):
        return self.data[i]
    def __repr__(self):
        return 'Item({})'.format(self.name)


tree = kdtree.create(dimensions=200)
with open("word.txt","r") as infile:
    for line in infile.readlines():
        name = line.split()[0]
        data = line.split()[1:]
        results = list(map(int, data))
        # npdata = np.array(results)
        # Zmean,Zmax,Zmin=npdata.mean(axis=0),npdata.max(axis=0),npdata.min(axis=0)
        # Z=(npdata-Zmean)/(Zmax-Zmin)
        k=Item(results,name)
        tree.add(k)


# f = open('database.txt', 'wb')
# pickle.dump(tree, f)
# f.close()

k="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 500 0 0 0 0 0 4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 508 0 0 0 58 0 0 0 0 0 0 0 0 0 0 0 0 0 4 0 0 0 24 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
l=k.split()
result = list(map(int, l))

tree.search_nn(result)
# k=5
# tree.search_knn(result, k, dist=None)

with open("word2.txt","r") as infile:
    for line in infile.readlines():
        name = line.split()[0]
        data = line.split()[1:]
        result3 = list(map(int, data))

npdata = np.array(result)
Zmax,Zmin=npdata.max(axis=0),npdata.min(axis=0)
Z=(npdata-Zmin)/(Zmax-Zmin)
L=Z.tolist()

npdata = np.array(result)
Zmean,Zmax,Zmin=npdata.mean(axis=0),npdata.max(axis=0),npdata.min(axis=0)
Z=(npdata-Zmean)/(Zmax-Zmin)
m=Z.tolist()


        tree.add(k)