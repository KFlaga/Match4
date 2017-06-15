#include "c:\Python35\include\Python.h"
#include "ServerModule.h"

typedef Match4::ServerHandle ServerHandle;

static ServerHandle getServerHandle(PyObject* args)
{
	PyObject* handleCapsule;
	if (!PyArg_ParseTuple(args, "O", &handleCapsule))
	{
		return 0;
	}

	ServerHandle handle;
	if (!(handle = (size_t)PyCapsule_GetPointer(handleCapsule, "server_handle")))
	{
		return 0;
	}

	return handle;
}

// Creates new game server and returns handle to it
// self: NULL
// args: NULL
// returns: { (Capsule)server_handle }
static PyObject* createServer(PyObject* self, PyObject* args)
{
	ServerHandle handle = Match4::createServer();

	PyObject* handleCapsule = PyCapsule_New((void*)handle, "server_handle", NULL);
	return handleCapsule;
}

// Destroys game server with given handle
// self: NULL
// args: { (Capsule)server_handle }
// returns: None
static PyObject* destroyServer(PyObject* self, PyObject* args)
{
	ServerHandle handle = getServerHandle(args);
	if (handle == 0)
	{
		return NULL;
	}
	Match4::destroyServer(handle);

	Py_INCREF(Py_None);
	return Py_None;
}

// Awaits for next message (block thread while awaiting) and returns it as python tuple
// self: NULL
// args: { (Capsule)server_handle }
// return: { (int)type,(int)data0,(int)data1,(int)data2 }
static PyObject* awaitResponse(PyObject* self, PyObject* args)
{
	ServerHandle handle = getServerHandle(args);
	if (handle == 0)
	{
		return NULL;
	}
	int msg[4] = {0,0,0,0};
	Match4::awaitResponse(handle, &msg[0]);

	PyObject* msgTuple = Py_BuildValue("(iiii)", msg[0], msg[1], msg[2], msg[3]);
	return msgTuple;
}

// Converts python tuple to message and sends it to server
// self: NULL
// args: { (Capsule)server_handle, (int)type,(int)data0,(int)data1,(int)data2 }
// return: None
static PyObject* sendMessage(PyObject* self, PyObject* args)
{
	PyObject* handleCapsule;
	int msg[4];
	if (!PyArg_ParseTuple(args, "Oiiii", &handleCapsule, msg, msg + 1, msg + 2, msg + 3))
	{
		return 0;
	}

	ServerHandle handle;
	if (!(handle = (size_t)PyCapsule_GetPointer(handleCapsule, "server_handle")))
	{
		return 0;
	}

	Match4::sendMessage(handle, msg);

	Py_INCREF(Py_None);
	return Py_None;
}

// Converts python string to message type
// self: NULL
// args: { (string)type_name }
// return: { (int)type }
static PyObject* getMessageType(PyObject* self, PyObject* args)
{
	const char* str;
	if (!PyArg_ParseTuple(args, "s", &str))
	{
		return 0;
	}

	int type = Match4::getMessageType(str);
	PyObject* res = Py_BuildValue("i", type);
	return res;
}

// List of methods in module
static PyMethodDef moduleMethods[] =
{
	{ "createServer",  createServer, METH_VARARGS, "" },
	{ "destroyServer",  destroyServer, METH_VARARGS, "" },
	{ "sendMessage",  sendMessage, METH_VARARGS, "" },
	{ "awaitResponse",  awaitResponse, METH_VARARGS, "" },
	{ "getMessageType",  getMessageType, METH_VARARGS, "" },
	{NULL, NULL, 0, NULL}        /* Sentinel */
};

// Definition of module
static struct PyModuleDef match4servermodule =
{
	PyModuleDef_HEAD_INIT,
	"match4server",   /* name of module */
	NULL, /* module documentation, may be NULL */
	-1,       /* size of per-interpreter state of the module,
			  or -1 if the module keeps state in global variables. */
	moduleMethods
};

// Initialization function for module
PyMODINIT_FUNC
PyInit_match4server(void)
{
	return PyModule_Create(&match4servermodule);
}