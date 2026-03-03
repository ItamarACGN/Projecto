# define PY_SSIZE_T_CLEAN
# include <Python.h>       /* MUST include <Python.h>, this implies inclusion of the following standard headers:
                             <stdio.h>, <string.h>, <errno.h>, <limits.h>, <assert.h> and <stdlib.h> (if available). */
# include <math.h>         /* include <Python.h> has to be before any standard headers are included */
# include "symnmf.h"

static PyObject* list_of_vectors_to_pyobject(struct Vector* vectors, int k, int D) {
    /*
    converts list of vectors structs to a py object
    */
    PyObject* py_list;
    int i, j;
    PyObject* py_value;
    py_list = PyList_New(k);
    for (i = 0; i < k; i++) {
        PyObject* py_vector = PyList_New(D);
        for (j = 0; j < D; j++) {
            py_value = PyFloat_FromDouble(vectors[i].values[j]);
            PyList_SetItem(py_vector, j, py_value);
        }
        PyList_SetItem(py_list, i, py_vector);
    }
    return py_list;
}

static struct Vector* pyList_to_vectors (PyObject* points_obj)
{
    /* 
    Convert a Python list of lists to a C array of Vector structs 
    */
    int i, j, size, dim;
    double val;
    Py_ssize_t py_size;
    struct Vector *points;
    PyObject* item, point_obj;
    py_size = PyList_Size(points_obj);
    size = (int)py_size;
    dim = 0;
    points = malloc(sizeof(struct Vector) * size);
    if (!points)
    {
        return NULL;
    }
    for (i = 0; i < size; i++){
        point_obj = PyList_GetItem(points_obj, i);
        dim = PyList_Size(point_obj);
        initVector(&points[i], dim);
        for (j = 0; j < dim; j++)
        {
            item = PyList_GetItem(point_obj, j);
            val = PyFloat_AsDouble(item);
            points[i].values[j] = val;
        }
    }
    return points;
}

static PyObject* matrix_to_pyobject(struct Matrix* matrix) {
    /* Convert a C Matrix struct to a Python list of lists */
    PyObject* py_value, py_row;
    PyObject* py_matrix;
    int i, j;
    
    py_matrix = PyList_New(matrix -> rows);
    for (i = 0; i < matrix -> rows; i++) {
        py_row = PyList_New(matrix -> cols);
        for (j = 0; j < matrix -> cols; j++) {
            //converts each row to a list of floats
            py_value = PyFloat_FromDouble(matrix_get(matrix, i, j));
            PyList_SetItem(py_row, j, py_value);
        }
        PyList_SetItem(py_matrix, i, py_row);
    }
    return py_matrix;
}

static Matrix* py_list_to_matrix(PyObject* python_list) {
    /* Convert a Python list of lists to a C Matrix struct */
    int i, j;
    int rows, cols;
    PyObject *row_obj, *item;
    Matrix *matrix;

    rows = (int)PyList_Size(python_list);

    row_obj = PyList_GetItem(python_list, 0);
    cols = (int)PyList_Size(row_obj);

    matrix = matrix_create(rows, cols);
    //making sure memory allocation was successful
    if (matrix == NULL) {
        return NULL;
    }

    for (i = 0; i < rows; i++) {
        row_obj = PyList_GetItem(python_list, i);
        for (j = 0; j < cols; j++) {
            item = PyList_GetItem(row_obj, j);
            double val = PyFloat_AsDouble(item);
            matrix_set(matrix, i, j, val);
        }
    }

    return matrix;
}

void free_vectors(struct Vector* vectors, int K) {
    for (int i = 0; i < K; i++) {
        free(vectors[i].values);
    }
    free(vectors);
}

static PyObject* execute_goal_capi(PyObject *self, PyObject *args)
{
    /* Wrapper function for the execute_goal function */
    PyObject *data;
    int n, d;
    const char* goal;
    if(!PyArg_ParseTuple(args, "Oiis", &data, &n, &d, &goal)) {
        return NULL;
    }
    struct Vector* data_vector = pyList_to_vectors(data);
    //memory allocation check
    if (data_vector == NULL) {
        return NULL;
    }
    struct Matrix* result_matrix = execute_goal(data_vector, n, d);
    //memory allocation check   
    if (result_matrix == NULL) {
        free_vectors(data_vector, n);
        return NULL;
    }
    PyObject* result = matrix_to_pyobject(result_matrix);
    free_vectors(data_vector, n);
    matrix_free(result_matrix);
    return result;
}

static PyObject* optimize_H_capi(PyObject *self, PyObject *args)
{
    /* Wrapper function for the optimize_H function */
    PyObject *H_obj, *W_obj;
    int max_iters;
    double epsilon;
    if(!PyArg_ParseTuple(args, "OOid", &H_obj, &W_obj, &max_iters, &epsilon)) {
        return NULL;
    }
    struct Matrix* H = py_list_to_matrix(H_obj);
    struct Matrix* W = py_list_to_matrix(W_obj);
    //memory allocation check
    if (H == NULL || W == NULL) {
        matrix_free(H);
        matrix_free(W);
        return NULL;
    }
    struct Matrix* result_matrix = optimize_H(H, W, max_iters, epsilon);
    //memory allocation check   
    if (result_matrix == NULL) {
        matrix_free(H);
        matrix_free(W);
        return NULL;
    }
    PyObject* result = matrix_to_pyobject(result_matrix);
    matrix_free(H);
    matrix_free(W);
    matrix_free(result_matrix);
    return result;
}

static PyMethodDef capiMethods[] = 
{
    {"execute_goal",                   
    (PyCFunction) execute_goal_capi,
    METH_VARARGS,         
    PyDoc_STR("A C function that implements execute_goal given the data and goal.")},
    {"optimize_H",                   
    (PyCFunction) optimize_H_capi,
    METH_VARARGS,         
    PyDoc_STR("A C function that implements optimize_H given H, W, max_iters and epsilon.")},
    {NULL, NULL, 0, NULL}     
};


static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "symnmfmodule",
    NULL,
    -1,  
    capiMethods
};

PyMODINIT_FUNC PyInit_symnmfmodule(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (!m) {
        return NULL;
    }
    return m;
}
