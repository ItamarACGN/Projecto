#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "symnmf.h"

double calculateDistance(struct Vector *p, struct Vector *q, int d) {
    double sum = 0.0;
    int i;
    double diff; /* Declared at the top */
    
    for (i = 0; i < d; i++) {
        diff = p->values[i] - q->values[i];
        sum += diff * diff;
    }
    return sum; 
}

/* Initialize a new matrix with zeros */
Matrix* matrix_create(int rows, int cols) {
    Matrix *mat = (Matrix*)malloc(sizeof(Matrix));
    if (mat == NULL) {
        fprintf(stderr, "Failed to allocate memory for Matrix struct.\n");
        return NULL;
    }
    
    mat->rows = rows;
    mat->cols = cols;
    /* calloc initializes the memory to zero */
    mat->data = (double*)calloc(rows * cols, sizeof(double)); 
    
    if (mat->data == NULL) {
        fprintf(stderr, "Failed to allocate memory for Matrix data.\n");
        free(mat);
        return NULL;
    }
    
    return mat;
}

/* Safely free the matrix memory */
void matrix_free(Matrix *mat) {
    if (mat != NULL) {
        if (mat->data != NULL) {
            free(mat->data);
        }
        free(mat);
    }
}

Matrix* matrix_multiply(const Matrix *A, const Matrix *B) {
    Matrix *result;
    int i, j, k;
    double sum, a_val, b_val;

    /* Check if dimensions are compatible for multiplication */
    if (A->cols != B->rows) {
        fprintf(stderr, "Dimension mismatch: %dx%d cannot be multiplied with %dx%d.\n", 
                A->rows, A->cols, B->rows, B->cols);
        return NULL;
    }
    
    /* Create the resulting matrix (A->rows x B->cols) */
    result = matrix_create(A->rows, B->cols);
    if (result == NULL) return NULL;
    
    /* Standard O(n^3) matrix multiplication */
    for (i = 0; i < A->rows; i++) {
        for (j = 0; j < B->cols; j++) {
            sum = 0.0;
            for (k = 0; k < A->cols; k++) {
                /* A[i][k] * B[k][j] */
                a_val = A->data[i * A->cols + k];
                b_val = B->data[k * B->cols + j];
                sum += a_val * b_val;
            }
            result->data[i * result->cols + j] = sum;
        }
    }
    
    return result;
}

Matrix* matrix_transpose(const Matrix *A) {
    Matrix *result;
    int i, j;

    /* The new matrix flips the rows and columns */
    result = matrix_create(A->cols, A->rows);
    if (result == NULL) return NULL;
    
    for (i = 0; i < A->rows; i++) {
        for (j = 0; j < A->cols; j++) {
            /* result[j][i] = A[i][j] */
            result->data[j * result->cols + i] = A->data[i * A->cols + j];
        }
    }
    
    return result;
}

/* Set a value at (row, col) */
void matrix_set(Matrix *mat, int row, int col, double value) {
    if (row >= 0 && row < mat->rows && col >= 0 && col < mat->cols) {
        mat->data[row * mat->cols + col] = value;
    }
}

/* Get a value at (row, col) */
double matrix_get(const Matrix *mat, int row, int col) {
    if (row >= 0 && row < mat->rows && col >= 0 && col < mat->cols) {
        return mat->data[row * mat->cols + col];
    }
    return 0.0; /* Or handle out-of-bounds error */
}

/* Matrix Addition: C = A + B */
Matrix* matrix_add(const Matrix *A, const Matrix *B) {
    Matrix *result;
    int total_elements;
    int i;

    if (A->rows != B->rows || A->cols != B->cols) {
        fprintf(stderr, "Dimension mismatch: %dx%d cannot be added to %dx%d.\n", 
                A->rows, A->cols, B->rows, B->cols);
        return NULL;
    }
    
    result = matrix_create(A->rows, A->cols);
    if (result == NULL) return NULL;
    
    total_elements = A->rows * A->cols;
    
    /* Single loop optimization thanks to the 1D array structure */
    for (i = 0; i < total_elements; i++) {
        result->data[i] = A->data[i] + B->data[i];
    }
    
    return result;
}

/* Matrix Subtraction: C = A - B */
Matrix* matrix_subtract(const Matrix *A, const Matrix *B) {
    Matrix *result;
    int total_elements;
    int i;

    if (A->rows != B->rows || A->cols != B->cols) {
        fprintf(stderr, "Dimension mismatch: %dx%d cannot be subtracted from %dx%d.\n", 
                A->rows, A->cols, B->rows, B->cols);
        return NULL;
    }
    
    result = matrix_create(A->rows, A->cols);
    if (result == NULL) return NULL;
    
    total_elements = A->rows * A->cols;
    
    for (i = 0; i < total_elements; i++) {
        result->data[i] = A->data[i] - B->data[i];
    }
    
    return result;
}

/* Calculate the Hilbert-Schmidt (Frobenius) norm of the matrix */
double matrix_hilbert_schmidt_norm(const Matrix *A) {
    /* Declare all variables at the top of the scope */
    double sum_of_squares = 0.0;
    int total_elements;
    int i;
    double val;

    if (A == NULL || A->data == NULL) return 0.0;
    
    total_elements = A->rows * A->cols;
    
    for (i = 0; i < total_elements; i++) {
        val = A->data[i];
        sum_of_squares += val * val;
    }
    
    return sum_of_squares;
}

/* print matrix formatted for the final submission */
void matrix_print(const Matrix *mat) {
    /* Declare variables at the top */
    int i, j;

    if (mat == NULL || mat->data == NULL) {
        return;
    }    
    
    for (i = 0; i < mat->rows; i++) {
        for (j = 0; j < mat->cols; j++) {
            /* Assignment requires 4 decimal places */
            printf("%.4f", matrix_get(mat, i, j)); 
            
            /* Assignment requires comma separation, except for the last element */
            if (j < mat->cols - 1) {
                printf(",");
            }
        }
        printf("\n");
    }
}
/* Left Multiplication: C = D * A 
 * Multiplying a diagonal matrix on the LEFT scales the ROWS of A.
 */
Matrix* diagonal_multiply_matrix(const Matrix *Diag, const Matrix *A) {
    Matrix *result;
    int i, j;
    double diag_val;

    /* Check if dimensions are compatible */
    if (Diag->cols != A->rows) {
        fprintf(stderr, "Dimension mismatch for diagonal multiplication.\n");
        return NULL;
    }
    
    result = matrix_create(A->rows, A->cols);
    if (result == NULL) return NULL;
    
    /* O(n^2) optimization: Only multiply by the diagonal elements */
    for (i = 0; i < A->rows; i++) {
        diag_val = Diag->data[i * Diag->cols + i]; /* Get D[i][i] */
        for (j = 0; j < A->cols; j++) {
            result->data[i * result->cols + j] = diag_val * A->data[i * A->cols + j];
        }
    }
    
    return result;
}

/* Right Multiplication: C = A * D 
 * Multiplying a diagonal matrix on the RIGHT scales the COLUMNS of A.
 */
Matrix* matrix_multiply_diagonal(const Matrix *A, const Matrix *Diag) {
    Matrix *result;
    int i, j;
    double diag_val;

    /* Check if dimensions are compatible */
    if (A->cols != Diag->rows) {
        fprintf(stderr, "Dimension mismatch for diagonal multiplication.\n");
        return NULL;
    }
    
    result = matrix_create(A->rows, A->cols);
    if (result == NULL) return NULL;
    
    /* O(n^2) optimization: Only multiply by the diagonal elements */
    for (j = 0; j < A->cols; j++) {
        diag_val = Diag->data[j * Diag->cols + j]; /* Get D[j][j] */
        for (i = 0; i < A->rows; i++) {
            result->data[i * result->cols + j] = A->data[i * A->cols + j] * diag_val;
        }
    }
    
    return result;
}
