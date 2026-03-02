#ifndef CAP_H_
#define CAP_H_

struct Vector {
    double *values;  
    int clusterID;   
};

typedef struct {
    int rows;
    int cols;
    double *data;
} Matrix;

/* Matrix functions */
Matrix* matrix_create(int rows, int cols);
void matrix_free(Matrix *mat);
Matrix* matrix_multiply(const Matrix *A, const Matrix *B);
Matrix* matrix_transpose(const Matrix *A);
void matrix_set(Matrix *mat, int row, int col, double value);
double matrix_get(const Matrix *mat, int row, int col);
Matrix* matrix_add(const Matrix *A, const Matrix *B);
Matrix* matrix_subtract(const Matrix *A, const Matrix *B);
double matrix_hilbert_schmidt_norm(const Matrix *A);

/* initialize a Vector's storage */
struct Vector* fit(struct Vector *data, struct Vector *centroids, int N, int D, int K, int max_iters, double epsilon);
//void initVector(struct Vector *v, int d);

#endif /* CAP_H_ */
