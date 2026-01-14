# define PY_SSIZE_T_CLEAN
# include <Python.h>       /* MUST include <Python.h>, this implies inclusion of the following standard headers:                       <stdio.h>, <string.h>, <errno.h>, <limits.h>, <assert.h> and <stdlib.h> (if available). */
# include <math.h>         /* include <Python.h> has to be before any standard headers are included */
#include <stdlib.h>

# include "cap.h"


static PyObject* geo_capi(PyObject *self, PyObject *args)
{
    double z;
    int n;

    if(!PyArg_ParseTuple(args, "di", &z, &n)) {
        return NULL;
    }

    return Py_BuildValue("d", geo_c(z, n));
}
void initVector(struct Vector *v, int d) {
    v->values = (double *)malloc(d * sizeof(double));
    v->clusterID = -1;
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
        points[i] = point;
        for (int j = 0; j < dim; j++)
        {
            PyObject* item = PyList_GetItem(point_obj, j);
            double val = PyFloat_AsDouble(item);
            points[i].values[j] = val;
        }
    }
    return points;
}

static PyMethodDef capiMethods[] = {
    {"geo",                   
      (PyCFunction) geo_capi,
       function and returns static PyObject*  */
      METH_VARARGS,         
      PyDoc_STR("A geometric series up to n. sum_up_to_n(z^n)")},
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
