#include <Python.h> /* it must include Python.h at first */
#include <stdlib.h>
#include <string.h>

extern char * execute(const char *, const char *);
extern int execute_all(const char *, const char *, 
		       void (void*, const char*), void*);

static PyObject*
xquery_execute(PyObject* self, PyObject* args)
{
  char * xquery = NULL;
  char * context_xml = NULL;
  if (!PyArg_ParseTuple(args, "es|es", 
			"utf-8", &xquery, 
			"utf-8", &context_xml)) {
    return NULL;
  }
  {
    char * buf = execute(xquery, context_xml);
    if (!buf) Py_RETURN_NONE; 
    PyObject* ret = PyUnicode_DecodeUTF8(buf, strlen(buf), NULL);
    free(buf);
    return ret;
  }
}

static void
append_pylist(void * pylist, const char * buf) 
{
  PyObject * list = pylist;
  PyObject * ret = PyUnicode_DecodeUTF8(buf, strlen(buf), NULL);
  PyList_Append(list, ret);
}

static PyObject*
xquery_execute_all(PyObject* self, PyObject* args)
{
  char * xquery = NULL;
  char * context_xml = NULL;
  if (!PyArg_ParseTuple(args, "es|es", 
			"utf-8", &xquery, 
			"utf-8", &context_xml)) {
    return NULL;
  }
  {
    PyObject * pylist = PyList_New(0);
    int ret = execute_all(xquery, context_xml, &append_pylist, pylist);
    if (ret) return pylist;
    Py_DECREF(pylist);
    PyErr_SetString(PyExc_ValueError, "invalid args");
    return NULL;
  }
}

static PyMethodDef  XQueryMethods[] = {
  {"execute", xquery_execute, METH_VARARGS, 
   "simplexquery.execute(xquery_code[, context_xml]) -> unicode"},
  {"execute_all", xquery_execute_all, METH_VARARGS, 
   "simplexquery.execute_all(xquery_code[, context_xml]) -> [unicode]"},
  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initsimplexquery(void)
{
  PyObject * m = Py_InitModule("simplexquery", XQueryMethods);
  if (m == NULL) return;
}
