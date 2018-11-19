#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <algorithm>

#include "common_include.h"
#include "loadFile.h"

struct clusterdata{
    int id;
    float data[Dimension];
};

struct imgword{
    string name;
    int word[Cluster_num];
};

float euclid_dist(int numdims, float *coord1, float *coord2);
int find_nearest_cluster(int numClusters, int numCoords, float *object, float **clusters);
void getVector(int numdims, int numClusters, float **sifts, int *vector, float **clusters);
