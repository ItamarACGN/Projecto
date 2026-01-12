#include <stdio.h>
#include <float.h>
#include <stdlib.h>

double epsilon = 0.001;
int default_max_iters = 400;
int bound_max_iters = 800;


struct Vector {
    double *values;  
    int clusterID;   
};

void initVector(struct Vector *v, int d) {
    v->values = (double *)malloc(d * sizeof(double));
    v->clusterID = -1;
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

void initialize_centroids(struct Vector *data, struct Vector *centroids, int D, int K) {
   int i, j;
   for(i = 0; i < K; i++) {
       for(j = 0; j < D; j++) {
           centroids[i].values[j] = data[i].values[j];
       }
   }
}

/* if data is 2 dimentional should it be int** data? */

void update_clasters(struct Vector *data, struct Vector *centroids, int N, int D, int K) {
   int i, j;
   for(i = 0; i < N; i++) {
       double min_distance = __DBL_MAX__;
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

struct Vector* solve(struct Vector *data, int N, int D, int K, int max_iters) {
   int i, j, iter;
   struct Vector* centroids = (struct Vector*) malloc(K * sizeof(struct Vector));
   struct Vector* old_centroids; /* Moved declaration to top */

   for(i = 0; i < K; i++) {
       initVector(&centroids[i], D);
   }
   initialize_centroids(data, centroids, D, K);
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
void escape() {
    printf("An Error Has Occurred\n");
    exit(1);
}
int main(int argc, char *argv[]) {
    struct Vector *points = NULL;
    struct Vector *centroids = NULL; /* Moved declaration to top */
    int capacity = 10;
    int N = 0;
    int d = 0;
    int max_iters = default_max_iters;
    int K = 0;
    
    /* Temporary variables for reading */
    double val;
    char sep;
    int current_idx = 0;
    int i, j;
    int max_d = 10; /* Moved declaration to top */
    int flag = 0;   /* Moved declaration to top */
    
    /* Allocation for the main array */
    points = (struct Vector*)malloc(capacity * sizeof(struct Vector));

    /* 1. Read the first vector separately to determine d */
    points[0].values = NULL; 
    points[0].clusterID = -1;
    
    points[0].values = (double*)malloc(max_d * sizeof(double));
    while (scanf("%lf%c", &val, &sep) == 2) {
        d++;
        if(d == max_d){
            max_d *= 2;
            points[0].values = (double*)realloc(points[0].values, max_d * sizeof(double));
        }
        points[0].values[d-1] = val;

        if (sep == '\n') break; 
    }
    N++;

    while (1) {
        if (N >= capacity) {
            capacity *= 2;
            points = (struct Vector*)realloc(points, capacity * sizeof(struct Vector));
        }

        points[N].values = (double*)malloc(d * sizeof(double));
        points[N].clusterID = -1;

        for (current_idx = 0; current_idx < d; current_idx++) {
            if (scanf("%lf%c", &val, &sep) != 2) {
                free(points[N].values);
                flag = 1;
                break;
            }
            points[N].values[current_idx] = val;
        }
        if(flag)break;
        N++;
    }    
    if (argc > 1) {
         K = atoi(argv[1]);
        if (K <= 1 || K >= N) {
            printf("Incorrect number of clusters!\n");
            for (i = 0; i < N; i++) {
               free(points[i].values);
            }
            free(points);
            return 1;
        }
    }else{
         for (i = 0; i < N; i++) {
            free(points[i].values);
         }
         free(points);
         escape();
         return 1;
    }
    if(argc > 2){
         max_iters = atoi(argv[2]);
        if(max_iters <= 1 || max_iters >= bound_max_iters){
            printf("Incorrect maximum iteration!\n");
            for (i = 0; i < N; i++) {
               free(points[i].values);
            }
            free(points);
            return 1;
        }
    }
    
    centroids = solve(points, N, d, K, max_iters);
    
    for (i = 0; i < K; i++) {
        for (j = 0; j < d; j++) {
            printf("%.4f", centroids[i].values[j]);
            if(j < d - 1){
                printf(",");
            }
        }
        
        printf("\n");
        free(centroids[i].values);
    }
    free(centroids);
    for (i = 0; i < N; i++) {
        free(points[i].values);
    }
    free(points);

    return 0;
}