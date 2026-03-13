#include "symnmf.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

double initVector(struct Vector *v, int d) {
    v->values = (double *)malloc(d * sizeof(double));
    if (v->values == NULL) {
        error_and_exit();
    }
    v->clusterID = -1;
    return 0;
}

Matrix* sym(struct Vector *v, int n, int d){
    int i,j;
    Matrix *result;
    result = matrix_create(n, n);
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
    Matrix *result, *symMatrix;
    result = matrix_create(n, n);
    symMatrix = sym(v, n, d);
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
    Matrix *normMatrix, *temp, *ddgMatrix, *symMatrix;
    symMatrix = sym(v, n, d);
    if (symMatrix == NULL) return NULL;
    
    ddgMatrix = ddg(v, n, d);
    if (ddgMatrix == NULL) {
        matrix_free(symMatrix);
        return NULL;
    }
    for(i = 0; i < n; i++){
        ddgMatrix->data[i * n + i] = 1.0 / sqrt(ddgMatrix->data[i * n + i]);
    }
    temp = matrix_multiply(ddgMatrix, symMatrix);
    if (temp == NULL) {
        matrix_free(symMatrix);
        matrix_free(ddgMatrix);
        return NULL;
    }
    normMatrix = matrix_multiply(temp, ddgMatrix);
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

Matrix * update_H (Matrix * curr_H, Matrix * W){
    int i,j;
    double beta,val;
    Matrix * next_H, * WH, * denominator, * temp_transpose;
    next_H = matrix_create(curr_H -> rows, curr_H -> cols);
    beta = 0.5;
    WH = matrix_multiply(W, curr_H);
    temp_transpose = matrix_transpose(curr_H);
    denominator = matrix_multiply(curr_H, temp_transpose);
    matrix_free(temp_transpose);
    Matrix * temp_denom = denominator;
    denominator = matrix_multiply(temp_denom, curr_H);
    matrix_free(temp_denom);

    for (i = 0; i < curr_H -> rows; i++){
        for (j = 0; j < curr_H -> cols; j++){
            val = 1 - beta + beta * (matrix_get(WH, i, j) / matrix_get(denominator, i, j));
            matrix_set(next_H, i, j, matrix_get(curr_H, i, j) * val);
        }
    }
    matrix_free(WH);
    matrix_free(denominator);
    return next_H;
}

Matrix * optimize_H(Matrix * H, Matrix * W, int max_iters, double epsilon){
    int iter;
    Matrix *curr_H, *next_H, *diff_matrix;
    double diff;
    curr_H = H;
    for (iter = 0; iter < max_iters; iter++){
        next_H = update_H(curr_H, W);
        diff_matrix = matrix_subtract(next_H, curr_H);
        diff = matrix_hilbert_schmidt_norm(diff_matrix);
        matrix_free(diff_matrix);
        if (diff < epsilon){
            matrix_free(next_H);
            break;
        }
        curr_H = next_H;
    }
    matrix_print(curr_H);
    return curr_H;
}

void error_and_exit() {
    printf("An Error Has Occurred\n");
    exit(1);
}

void find_rows_cols(const char *file_name, int *rows_out, int *cols_out) {
    int rows = 0, cols = 0, c;
    double val;
    FILE *f = fopen(file_name, "r");
    if (f == NULL) {
        error_and_exit();
    }
    while (fscanf(f, "%lf", &val) == 1) {
        if (rows == 0) {
            cols++;
        }
        c = fgetc(f);
        if (c == '\n') {
            rows++;
        } else if (c == EOF) {
            rows++;
            break;
        }
    }
    fclose(f);
    *rows_out = rows;
    *cols_out = cols;
}

struct Vector* read_file(const char *file_name, int *n_out, int *d_out) {
    int rows = 0,cols = 0,i,j;
    double val;
    FILE *f;
    struct Vector *v;
    f = fopen(file_name, "r");
    if (f == NULL)error_and_exit();
    find_rows_cols(file_name, &rows, &cols);
    *n_out = rows;
    *d_out = cols;

    v = (struct Vector*)malloc(rows * sizeof(struct Vector));
    if (v == NULL) {
        fclose(f);
        error_and_exit();
    }

    for (i = 0; i < rows; i++) {
        v[i].values = (double*)malloc(cols * sizeof(double));
        
        if (v[i].values == NULL) {
            for (j = 0; j < i; j++) {
                free(v[j].values);
            }
            free(v);
            fclose(f);
            error_and_exit();
        }

        for (j = 0; j < cols; j++) {
            fscanf(f, "%lf", &val);
            v[i].values[j] = val;
            fgetc(f); 
        }
    }
    fclose(f);
    return v;
}

/* Process vectors based on goal and execute corresponding action */
Matrix* execute_goal(struct Vector *v, int n, int d, const char *goal) {
    if(strcmp(goal, "sym") == 0) {
        return sym(v, n, d);
    } else if(strcmp(goal, "ddg") == 0) {
        return ddg(v, n, d);
    } else if(strcmp(goal, "norm") == 0) {
        return norm(v, n, d);
    } else {
        printf("An Error Has Occurred\n");
        return NULL;
    }
}

int main(int argc, char *argv[]) {
    char *file_name;
    int n, d;
    int i;
    struct Vector *v;
    Matrix *result;

    if (argc != 3) {
        printf("An Error Has Occurred\n");
        return 1;
    }

    file_name = argv[2];

    v = read_file(file_name, &n, &d);

    result = execute_goal(v, n, d, argv[1]);
    
    if (result != NULL) {
        matrix_print(result);
        matrix_free(result);
    }
    
    for (i = 0; i < n; i++) {
        free(v[i].values);
    }
    free(v);
    return 0;
}
