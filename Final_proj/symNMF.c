# include "symnmf.h"


Matrix * update_H (Matrix * curr_H, Matrix * W){
    Matrix * next_H = matrix_create(curr_H -> rows, curr_H -> cols);
    double beta = 0.5;
    Matrix * WH = matrix_multiply(W, curr_H);
    Matrix * denominator = matrix_multiply( curr_H, matrix_transpose(WH));
    denominator = matrix_multiply(denominator, curr_H);

    for (int i = 0; i < curr_H -> rows; i++){
        for (int j = 0; j < curr_H -> cols; j++){
            double val = 1 - beta + beta * (matrix_get(WH, i, j) / matrix_get(denominator, i, j));
            matrix_set(next_H, i, j, val);
        }
    }
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



