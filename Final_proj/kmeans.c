#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int rows;
    int cols;
    double *data;
} Matrix;

// Initialize a new matrix with zeros
Matrix* matrix_create(int rows, int cols) {
    Matrix *mat = (Matrix*)malloc(sizeof(Matrix));
    if (mat == NULL) {
        fprintf(stderr, "Failed to allocate memory for Matrix struct.\n");
        return NULL;
    }
    
    mat->rows = rows;
    mat->cols = cols;
    // calloc initializes the memory to zero
    mat->data = (double*)calloc(rows * cols, sizeof(double)); 
    
    if (mat->data == NULL) {
        fprintf(stderr, "Failed to allocate memory for Matrix data.\n");
        free(mat);
        return NULL;
    }
    
    return mat;
}

// Safely free the matrix memory
void matrix_free(Matrix *mat) {
    if (mat != NULL) {
        if (mat->data != NULL) {
            free(mat->data);
        }
        free(mat);
    }
}

Matrix* matrix_multiply(const Matrix *A, const Matrix *B) {
    // Check if dimensions are compatible for multiplication
    if (A->cols != B->rows) {
        fprintf(stderr, "Dimension mismatch: %dx%d cannot be multiplied with %dx%d.\n", 
                A->rows, A->cols, B->rows, B->cols);
        return NULL;
    }
    
    // Create the resulting matrix (A->rows x B->cols)
    Matrix *result = matrix_create(A->rows, B->cols);
    if (result == NULL) return NULL;
    
    // Standard O(n^3) matrix multiplication
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < A->cols; k++) {
                // A[i][k] * B[k][j]
                double a_val = A->data[i * A->cols + k];
                double b_val = B->data[k * B->cols + j];
                sum += a_val * b_val;
            }
            result->data[i * result->cols + j] = sum;
        }
    }
    
    return result;
}

Matrix* matrix_transpose(const Matrix *A) {
    // The new matrix flips the rows and columns
    Matrix *result = matrix_create(A->cols, A->rows);
    if (result == NULL) return NULL;
    
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->cols; j++) {
            // result[j][i] = A[i][j]
            result->data[j * result->cols + i] = A->data[i * A->cols + j];
        }
    }
    
    return result;
}

// Set a value at (row, col)
void matrix_set(Matrix *mat, int row, int col, double value) {
    if (row >= 0 && row < mat->rows && col >= 0 && col < mat->cols) {
        mat->data[row * mat->cols + col] = value;
    }
}

// Get a value at (row, col)
double matrix_get(const Matrix *mat, int row, int col) {
    if (row >= 0 && row < mat->rows && col >= 0 && col < mat->cols) {
        return mat->data[row * mat->cols + col];
    }
    return 0.0; // Or handle out-of-bounds error
}

// Matrix Addition: C = A + B
Matrix* matrix_add(const Matrix *A, const Matrix *B) {
    if (A->rows != B->rows || A->cols != B->cols) {
        fprintf(stderr, "Dimension mismatch: %dx%d cannot be added to %dx%d.\n", 
                A->rows, A->cols, B->rows, B->cols);
        return NULL;
    }
    
    Matrix *result = matrix_create(A->rows, A->cols);
    if (result == NULL) return NULL;
    
    int total_elements = A->rows * A->cols;
    
    // Single loop optimization thanks to the 1D array structure
    for (int i = 0; i < total_elements; i++) {
        result->data[i] = A->data[i] + B->data[i];
    }
    
    return result;
}

// Matrix Subtraction: C = A - B
Matrix* matrix_subtract(const Matrix *A, const Matrix *B) {
    if (A->rows != B->rows || A->cols != B->cols) {
        fprintf(stderr, "Dimension mismatch: %dx%d cannot be subtracted from %dx%d.\n", 
                A->rows, A->cols, B->rows, B->cols);
        return NULL;
    }
    
    Matrix *result = matrix_create(A->rows, A->cols);
    if (result == NULL) return NULL;
    
    int total_elements = A->rows * A->cols;
    
    for (int i = 0; i < total_elements; i++) {
        result->data[i] = A->data[i] - B->data[i];
    }
    
    return result;
}

#include <math.h>

// Calculate the Hilbert-Schmidt (Frobenius) norm of the matrix
double matrix_hilbert_schmidt_norm(const Matrix *A) {
    if (A == NULL || A->data == NULL) return 0.0;
    
    double sum_of_squares = 0.0;
    int total_elements = A->rows * A->cols;
    
    for (int i = 0; i < total_elements; i++) {
        double val = A->data[i];
        sum_of_squares += val * val;
    }
    
    return sqrt(sum_of_squares);
}