# define PY_SSIZE_T_CLEAN
# include <Python.h>       /* MUST include <Python.h>, this implies inclusion of the following standard headers:
                             <stdio.h>, <string.h>, <errno.h>, <limits.h>, <assert.h> and <stdlib.h> (if available). */
# include <math.h>         /* include <Python.h> has to be before any standard headers are included */
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
