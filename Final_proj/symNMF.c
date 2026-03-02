#include "symnmf.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Matrix* sym(struct Vector *v, int n, int d){
    Matrix *result = matrix_create(n, n);
    if (result == NULL) return NULL;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double dist = calculateDistance(&v[i], &v[j], d);
            matrix_set(result, i, j, exp(-dist/2));
        }
    }
    return result;
}

Matrix* ddg(struct Vector *v, int n, int d){
    Matrix *result = matrix_create(n, n);
    if (result == NULL) return NULL;
    
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < n; j++) {
            double dist = calculateDistance(&v[i], &v[j], d);
            sum += dist;
        }
        matrix_set(result, i, i, sum);
    }
    return result;
}

Matrix* norm(struct Vector *v, int n, int d){
    Matrix *symMatrix = sym(v, n, d);
    if (symMatrix == NULL) return NULL;
    
    Matrix *ddgMatrix = ddg(v, n, d);
    if (ddgMatrix == NULL) {
        matrix_free(symMatrix);
        return NULL;
    }
    for(int i = 0; i < n; i++){
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