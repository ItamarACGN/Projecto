# define PY_SSIZE_T_CLEAN
# include <Python.h>       /* MUST include <Python.h>, this implies inclusion of the following standard headers:
                             <stdio.h>, <string.h>, <errno.h>, <limits.h>, <assert.h> and <stdlib.h> (if available). */
# include <math.h>         /* include <Python.h> has to be before any standard headers are included */
# include "cap.h"

static PyObject* list_of_vectors_to_pyobject(struct Vector* vectors, int K, int D) {
    PyObject* py_list = PyList_New(K);
    for (int i = 0; i < K; i++) {
        PyObject* py_vector = PyList_New(D);
        for (int j = 0; j < D; j++) {
            PyObject* py_value = PyFloat_FromDouble(vectors[i].values[j]);
            PyList_SetItem(py_vector, j, py_value);
        }
        PyList_SetItem(py_list, i, py_vector);
    }
    return py_list;
}



static struct Vector* pyList_to_vectors (PyObject* points_obj)
{
    Py_ssize_t py_size = PyList_Size(points_obj);
    int size = (int)py_size;
    int dim = 0;
    
    struct Vector *points = malloc(sizeof(struct Vector) * size);
    if (!points)
    {
        return NULL;
    }
    
    for (int i = 0; i < size; i++){
        PyObject* point_obj = PyList_GetItem(points_obj, i);
        dim = PyList_Size(point_obj);

        initVector(&points[i], dim);
        
        for (int j = 0; j < dim; j++)
        {
            PyObject* item = PyList_GetItem(point_obj, j);
            double val = PyFloat_AsDouble(item);
            points[i].values[j] = val;
        }
    }
    return points;
}

void free_vectors(struct Vector* vectors, int K) {
    for (int i = 0; i < K; i++) {
        free(vectors[i].values);
    }
    free(vectors);
}
static PyObject* kmeans_capi(PyObject *self, PyObject *args)
{
    PyObject *data;
    PyObject *centroids;
    int N, D, K, max_iters;

    if(!PyArg_ParseTuple(args, "iiiiOO", &N, &D, &K, &max_iters, &data, &centroids)) {
        return NULL;
    }
    struct Vector* data_vector = pyList_to_vectors(data);
    struct Vector* centroids_vector = pyList_to_vectors(centroids);
    struct Vector* result_centroids = fit(data_vector, centroids_vector, N, D, K, max_iters);

    PyObject* result = list_of_vectors_to_pyobject(result_centroids, K, D);
    free_vectors(data_vector, N);
    free_vectors(centroids_vector, K);
    return result;
}


static PyMethodDef capiMethods[] = {
    {"kmeans",                   
      (PyCFunction) kmeans_capi,
       /*function and returns static PyObject*  */
      METH_VARARGS,         
      PyDoc_STR("A C function that implements kmeans given the starting centroids.")},
    {NULL, NULL, 0, NULL}     
};


static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "capi_demo1",
    NULL,
    -1,  
    capiMethods
};

PyMODINIT_FUNC
PyInit_capi_demo1(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (!m) {
        return NULL;
    }
    return m;
}
