#include "BoWBuilder.h"
#define ClusterNum 100




void getVector(int numdims, int numClusters, float **sifts, int *vector, float **clusters){
    int i, index;
    for(i=0;i<numClusters;i++){
        index=find_nearest_cluster(numClusters,numdims, sifts[i], clusters);
        vector[index] += 1;
    }
}

/* square of Euclid distance between two multi-dimensional points            */
/*                          两个多维点的欧式距离平方                           */
float euclid_dist(int    numdims,  /* no. dimensions */
                    float *coord1,   /* [numdims] */
                    float *coord2)   /* [numdims] */
{
    int i;
    float ans=0.0;

    for (i=0; i<numdims; i++)
        ans += (coord1[i]-coord2[i]) * (coord1[i]-coord2[i]);

    return(ans);
}

/*----< find_nearest_cluster() >---------------------------------------------*/
/*                          找到聚类的最近点                                  */
int find_nearest_cluster(int     numClusters, /* no. clusters */
                         int     numCoords,   /* no. coordinates */
                         float  *object,      /* [numCoords] */
                         float **clusters)    /* [numClusters][numCoords] */
{
    int   index, i;
    float dist, min_dist;

    /* find the cluster id that has min distance to object */
    index    = 0;
    min_dist = euclid_dist(numCoords, object, clusters[0]);

    for (i=1; i<numClusters; i++) {
        dist = euclid_dist(numCoords, object, clusters[i]);
        /* no need square root */
        if (dist < min_dist) { /* find the min and its array index */
            min_dist = dist;
            index    = i;
        }
    }
    return(index);
}

