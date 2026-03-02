#include "symnmf.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


Matrix* sym(struct Vector *v, int n, int d){
    int i,j;
    Matrix *result = matrix_create(n, n);
    if (result == NULL) return NULL;
    
    for ( i = 0; i < n; i++) {
        for ( j = 0; j < n; j++) {
            double dist = calculateDistance(&v[i], &v[j], d);
            if(i!=j) {
                matrix_set(result, i, j, exp(-dist/2));
            } else {
                matrix_set(result, i, j, 0.0);
            }
        }
    }
    return result;
}

Matrix* ddg(struct Vector *v, int n, int d){
    int i,j;
    Matrix *result = matrix_create(n, n);
    Matrix *symMatrix = sym(v, n, d);
    if (result == NULL || symMatrix == NULL) {
        matrix_free(result);
        matrix_free(symMatrix);
        return NULL;
    }
    for ( i = 0; i < n; i++) {
        double sum = 0.0;
        for ( j = 0; j < n; j++) {
            sum += matrix_get(symMatrix, i, j);
        }
        matrix_set(result, i, i, sum);
    }
    matrix_free(symMatrix);
    return result;
}

Matrix* norm(struct Vector *v, int n, int d){
    int i;
    Matrix *symMatrix = sym(v, n, d);
    if (symMatrix == NULL) return NULL;
    
    Matrix *ddgMatrix = ddg(v, n, d);
    if (ddgMatrix == NULL) {
        matrix_free(symMatrix);
        return NULL;
    }
    for(i = 0; i < n; i++){
        ddgMatrix->data[i * n + i] = 1.0 / sqrt(ddgMatrix->data[i * n + i]);
    }
    Matrix *temp = matrix_multiply(ddgMatrix, symMatrix);
    if (temp == NULL) {
        matrix_free(symMatrix);
        matrix_free(ddgMatrix);
        return NULL;
    }
    Matrix *normMatrix = matrix_multiply(temp, ddgMatrix);
    if (normMatrix == NULL) {
        matrix_free(symMatrix);
        matrix_free(ddgMatrix);
        matrix_free(temp);
        return NULL;
    }

    
    matrix_free(temp);
    matrix_free(symMatrix);
    matrix_free(ddgMatrix);

    return normMatrix;
}


int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "An error has occurred\n");
        exit(1);
    }

}


Matrix * update_H (Matrix * curr_H, Matrix * W){
    Matrix * next_H = matrix_create(curr_H -> rows, curr_H -> cols);
    double beta = 0.5;
    Matrix * WH = matrix_multiply(W, curr_H);
    Matrix * denominator = matrix_multiply( curr_H, matrix_transpose(curr_H));
    denominator = matrix_multiply(denominator, curr_H);

    for (int i = 0; i < curr_H -> rows; i++){
        for (int j = 0; j < curr_H -> cols; j++){
            double val = 1 - beta + beta * (matrix_get(WH, i, j) / matrix_get(denominator, i, j));
            matrix_set(next_H, i, j, matrix_get(curr_H, i, j) * val);
        }
    }
    matrix_free(WH);
    matrix_free(denominator);
    return next_H;
}

Matrix * optimize_H(Matrix * H, Matrix * W, int max_iters, double epsilon){
    Matrix * curr_H = H;
    for (int iter = 0; iter < max_iters; iter++){
        Matrix * next_H = update_H(curr_H, W);
        double diff = matrix_hilbert_schmidt_norm(matrix_subtract(next_H, curr_H));
        if (diff < epsilon){
            matrix_free(next_H);
            break;
        }
        matrix_free(curr_H);
        curr_H = next_H;
    }
    matrix_print(curr_H);
    return curr_H;
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "An error has occurred\n");
        exit(1);
    }

}

