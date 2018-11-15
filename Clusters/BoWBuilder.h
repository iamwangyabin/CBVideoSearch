#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define ClusterNum 1000
#define Dimension 128

float euclid_dist(int numdims, float *coord1, float *coord2);
int find_nearest_cluster(int numClusters, int numCoords, float *object, float **clusters);
void getVector(int numdims, int numClusters, float **sifts, int *vector, float **clusters);
