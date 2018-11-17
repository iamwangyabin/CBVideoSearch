

int find_nearest_cluster(int     numClusters, /* no. clusters */
                         int     numCoords,   /* no. coordinates */
                         float  *object,      /* [numCoords] */
                         float **clusters);    /* [numClusters][numCoords] */


int omp_kmeans(int     is_perform_atomic, /* in: */
               float **objects,           /* in: [numObjs][numCoords] */
               int     numCoords,         /* no. coordinates */
               int     numObjs,           /* no. objects */
               int     numClusters,       /* no. clusters */
               float   threshold,         /* % objects change membership */
               int    *membership,        /* out: [numObjs] */
               float **clusters);        /* out: [numClusters][numCoords] */
