# define CAP_H_
# define CAP_H_

struct Vector {
    double *values;  
    int clusterID;   
};

struct Vector* fit(struct Vector *data, struct Vector *centroids, int N, int D, int K, int max_iters);
# endif