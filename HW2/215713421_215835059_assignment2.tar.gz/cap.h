#ifndef CAP_H_
#define CAP_H_

struct Vector {
    double *values;  
    int clusterID;   
};

/* initialize a Vector's storage */
struct Vector* fit(struct Vector *data, struct Vector *centroids, int N, int D, int K, int max_iters, double epsilon);
//void initVector(struct Vector *v, int d);

#endif /* CAP_H_ */
