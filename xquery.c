#include <Python.h> /* it must include Python.h at first */
#include <stdlib.h>
#include <string.h>

extern char * execute(const char *, const char *);

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
    PyObject* ret = PyUnicode_DecodeUTF8(buf, strlen(buf), NULL);
    free(buf);
    return ret;
  }
}

static PyMethodDef  XQueryMethods[] = {
  {"execute", xquery_execute, METH_VARARGS, 
   "simplexquery.execute(xquery_code[, context_xml]) -> unicode"},
  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initsimplexquery(void)
{
  PyObject * m = Py_InitModule("simplexquery", XQueryMethods);
  if (m == NULL) return;
}
