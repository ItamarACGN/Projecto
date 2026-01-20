#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "cap.h"

double epsilon;
int default_max_iters = 400;
int bound_max_iters = 800;


void escape() {
    printf("An Error Has Occurred\n");
    exit(1);
}



double calculateDistance(struct Vector *p, struct Vector *q, int d) {
    double sum = 0.0;
    int i;
    for (i = 0; i < d; i++) {
        double diff = p->values[i] - q->values[i];
        sum += diff * diff;
    }
    return sum; 
}
double initVector(struct Vector *v, int d) {
    v->values = (double *)malloc(d * sizeof(double));
    if (v->values == NULL) {
        escape();
    }
    v->clusterID = -1;
    return 0;
}
/* if data is 2 dimentional should it be int** data? */

void update_clasters(struct Vector *data, struct Vector *centroids, int N, int D, int K) {
   int i, j;
   for(i = 0; i < N; i++) {
    double min_distance = DBL_MAX;
       int closest_centroid = -1;
       for(j = 0; j < K; j++) {
           double distance = calculateDistance(&data[i], &centroids[j], D);
           if(distance < min_distance) {
               min_distance = distance;
               closest_centroid = j;
           }
       }
      data[i].clusterID = closest_centroid;
   }
}

void update_centroids(struct Vector *data, struct Vector *centroids, int N, int D, int K) {
   int i, j;
   int* counts = (int*) calloc(K, sizeof(int));
   for(i = 0; i < K; i++) {
       for(j = 0; j < D; j++) {
           centroids[i].values[j] = 0.0;
       }
   }
   for(i = 0; i < N; i++) {
       int clusterID = data[i].clusterID;
       counts[clusterID]++;
   }
   for(i = 0; i < K; i++){
         if(counts[i] == 0){
            counts[data[0].clusterID]--;
            counts[i]++;
            data[0].clusterID = i;
         }
   }
   for(i = 0; i < N; i++) {
       int clusterID = data[i].clusterID;
       for(j = 0; j < D; j++) {
           centroids[clusterID].values[j] += data[i].values[j];
       }
   }
   for(i = 0; i < K; i++) {
       if(counts[i] > 0) {
           for(j = 0; j < D; j++) {
               centroids[i].values[j] /= counts[i];
           }
       }else{
         for(j = 0; j < D; j++) {
               centroids[i].values[j] = data[0].values[j];
           }
       }
   }
   free(counts);
}

int has_converged(struct Vector *old_centroids, struct Vector *centroids, int K, int D) {
   int i;
   for(i = 0; i < K; i++) {
       double distance = calculateDistance(&old_centroids[i], &centroids[i], D);
       if(distance > epsilon * epsilon) {
           return 0;
       }
   }
   return 1;
}

struct Vector* fit(struct Vector *data, struct Vector *centroids, int N, int D, int K, int max_iters, double epsilon_val) {
   int i, j, iter;
   struct Vector* old_centroids; /* Moved declaration to top */
   epsilon = epsilon_val;

   for(iter = 1; iter < max_iters; iter++) {
       update_clasters(data, centroids, N, D, K);
       
       /* copy old centroids for convergence check */
       old_centroids = (struct Vector*) malloc(K * sizeof(struct Vector));
       for(i = 0; i < K; i++) {
           initVector(&old_centroids[i], D);
           for(j = 0; j < D; j++) {
               old_centroids[i].values[j] = centroids[i].values[j];
           }
       }
       update_centroids(data, centroids, N, D, K);
         if(has_converged(old_centroids, centroids, K, D)) {
            for(i = 0; i < K; i++) {
                  free(old_centroids[i].values);
            }
            free(old_centroids);
            break;
         }
         for(i = 0; i < K; i++) {
            free(old_centroids[i].values);
         }
         free(old_centroids);
   }
   return centroids;
}
