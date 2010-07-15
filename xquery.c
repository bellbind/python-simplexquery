#include <Python.h> /* it must include Python.h at first */
#include <stdlib.h>
#include <string.h>

typedef char * (* resolver_t)(void *, const char *);

extern char * execute(const char *, const char *,
                      resolver_t, void *);
extern int execute_all(const char *, const char *, 
                       void (void *, const char *), void *,
                       resolver_t, void *);

static void
append_pylist(void * pylist, const char * buf) 
{
  PyObject * list = pylist;
  PyObject * ret = PyUnicode_DecodeUTF8(buf, strlen(buf), NULL);
  PyList_Append(list, ret);
}

static char *
call_resolver(void * resolver, const char * uri) 
{
  PyObject * callable = resolver;
  PyObject * uniuri = PyUnicode_DecodeUTF8(uri, strlen(uri), NULL);
  PyObject * args = PyTuple_Pack(1, uniuri);
  PyObject * strret = PyObject_CallObject(callable, args);
  PyObject * rets = PyTuple_Pack(1, strret);
  char * cpret = NULL;
  int ok = PyArg_ParseTuple(rets, "es", "utf-8", &cpret);
  if (ok) {
    size_t len = strlen(cpret);
    char * ret = malloc(len + 1);
    strcpy(ret, cpret);
    return ret;    
  }
  return NULL;
}

static int
execute_parse_params(PyObject * args, PyObject * kwargs,
                     char ** pxquery, char ** pcontext_xml, 
                     PyObject ** presolver, resolver_t * presolver_func)
{
  static char * kwlist[] = {"xquery_code", "context_xml", "resolver", NULL};
  int ok = PyArg_ParseTupleAndKeywords(args, kwargs, "es|esO", kwlist, 
                                       "utf-8", pxquery, 
                                       "utf-8", pcontext_xml,
                                       presolver);
  if (!ok) {
    return 0;
  }
  if (*presolver) {
    *presolver_func = &call_resolver;
  }
  return 1;
}

static PyObject *
xquery_execute(PyObject * self, PyObject * args, PyObject * kwargs)
{
  char * xquery = NULL;
  char * context_xml = NULL;
  PyObject * resolver = NULL;
  resolver_t resolver_func = NULL;
  int ok = execute_parse_params(args, kwargs,
                                &xquery, &context_xml, 
                                &resolver, &resolver_func);
  if (ok) {
    char * buf = execute(xquery, context_xml, 
                         resolver_func, resolver);
    if (buf) {
      PyObject * ret = PyUnicode_DecodeUTF8(buf, strlen(buf), NULL);
      free(buf);
      return ret;
    }
  }
  Py_RETURN_NONE;
}


static PyObject *
xquery_execute_all(PyObject * self, PyObject * args, PyObject* kwargs)
{
  char * xquery = NULL;
  char * context_xml = NULL;
  PyObject * resolver = NULL;
  resolver_t resolver_func = NULL;
  int ok = execute_parse_params(args, kwargs,
                                &xquery, &context_xml, 
                                &resolver, &resolver_func);
  if (ok) {
    PyObject * pylist = PyList_New(0);
    int ret = execute_all(xquery, context_xml, &append_pylist, pylist,
                          resolver_func, resolver);
    if (ret) return pylist;
    Py_DECREF(pylist);
    PyErr_SetString(PyExc_ValueError, "invalid args");
  }
  return NULL;  
}

static PyMethodDef  XQueryMethods[] = {
  {"execute", (PyCFunction) xquery_execute, 
   METH_VARARGS | METH_KEYWORDS, 
   "execute(xquery_code[, context_xml][, resolver]) -> unicode"
  },
  {"execute_all", (PyCFunction) xquery_execute_all, 
   METH_VARARGS | METH_KEYWORDS, 
   "execute_all(xquery_code[, context_xml][, resolver]) -> [unicode]"
  },
  {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef XQueryModule = {
  PyModuleDef_HEAD_INIT,
  "simplexquery",
  NULL,
  -1,
  XQueryMethods
};

PyMODINIT_FUNC
PyInit_simplexquery(void)
{
  PyObject * m = PyModule_Create(&XQueryModule);
  if (m == NULL) return NULL;
  return m;
}

#else

PyMODINIT_FUNC
initsimplexquery(void)
{
  PyObject * m = Py_InitModule("simplexquery", XQueryMethods);
  if (m == NULL) return;
}

#endif
