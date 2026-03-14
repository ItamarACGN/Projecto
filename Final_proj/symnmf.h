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

/*vector functions*/
double initVector(struct Vector *v, int d); /*vector initialization - memory allocation*/

/* Matrix functions */
double calculateDistance(struct Vector *p, struct Vector *q, int d); /* Compute Euclidean distance between two vectors */
void matrix_print(const Matrix *mat); /* Print matrix to stdout in comma-separated format */
Matrix* matrix_create(int rows, int cols); /* Allocate and initialize a matrix with zeros */
void matrix_free(Matrix *mat); /* Free matrix memory */
Matrix* matrix_multiply(const Matrix *A, const Matrix *B); /* Multiply two matrices A*B */
Matrix* matrix_transpose(const Matrix *A); /* Transpose a matrix */
void matrix_set(Matrix *mat, int row, int col, double value); /* Set value at (row, col) */
double matrix_get(const Matrix *mat, int row, int col); /* Get value at (row, col) */
Matrix* matrix_add(const Matrix *A, const Matrix *B); /* Add two matrices A+B */
Matrix* matrix_subtract(const Matrix *A, const Matrix *B); /* Subtract two matrices A-B */
double matrix_hilbert_schmidt_norm(const Matrix *A); /* Compute Frobenius norm of a matrix */

/* initialize a Vector's storage */
struct Vector* fit(struct Vector *data, struct Vector *centroids, int N, int D, int K, int max_iters, double epsilon); /* Run k-means clustering algorithm */

/* Symnmf functions */

Matrix* execute_goal(struct Vector *v, int n, int d, const char *goal); /* Process vectors based on goal and execute corresponding action */
Matrix* ddg(struct Vector *v, int n, int d); /* Compute the degree matrix */
Matrix* sym(struct Vector *v, int n, int d); /* Compute the similarity matrix*/
Matrix* norm(struct Vector *v, int n, int d); /* Compute the normalized similarity matrix */
Matrix* optimize_H(Matrix * H, Matrix * W, int max_iters, double epsilon); /* Optimize H matrix using multiplicative update rules */
void error_and_exit(void); /*prints error messege and quirs - helper*/
Matrix* diagonal_multiply_matrix(const Matrix *Diag, const Matrix *A);
Matrix* matrix_multiply_diagonal(const Matrix *A, const Matrix *Diag);
#endif /* CAP_H_ */
