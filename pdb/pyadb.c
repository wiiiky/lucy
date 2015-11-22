/*
 * Copyright (C) 2015 Wiky L
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */
#include <Python.h>
#include "wrapper.h"


static PyObject *py_start_server(PyObject *self, PyObject *args, PyObject *keywds) {
    unsigned short port = DEFAULT_ADB_PORT;
    static char *kwlist[] = {"port", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|H", kwlist, &port)) {
        return NULL;
    }
    if(!start_server(port)) {
        Py_RETURN_FALSE;
    }
    Py_RETURN_TRUE;
}

static PyObject *py_kill_server(PyObject *self, PyObject *args, PyObject *keywds) {
    unsigned short port = DEFAULT_ADB_PORT;
    static char *kwlist[] = {"port", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|H", kwlist, &port)) {
        return NULL;
    }
    if(!kill_server(port)) {
        Py_RETURN_FALSE;
    }
    Py_RETURN_TRUE;
}

static PyObject *py_install_apk(PyObject *self, PyObject *args, PyObject *keywds) {
    const char *path=NULL;
    int reinstall=0;
    unsigned short port=DEFAULT_ADB_PORT;
    static char *kwlist[] = {"path", "reinstall", "port", NULL};
    if(!PyArg_ParseTupleAndKeywords(args, keywds, "s|pH", kwlist, &path, &reinstall, &port)) {
        return NULL;
    }
    if(!install_apk(path, reinstall, port)) {
        Py_RETURN_FALSE;
    }
    Py_RETURN_TRUE;
}

static PyObject *py_forward_tcp(PyObject *self, PyObject *args, PyObject *keywds) {
    unsigned short local;
    unsigned short remote;
    unsigned short port=DEFAULT_ADB_PORT;
    int rebind=0;
    static char *kwlist[] = {"local","remote","rebind", "port",NULL};
    if(!PyArg_ParseTupleAndKeywords(args, keywds, "HH|pH", kwlist, &local, &remote, &rebind, &port)) {
        return NULL;
    }
    if(!forward_tcp(local, remote, rebind, port)) {
        Py_RETURN_FALSE;
    }
    Py_RETURN_TRUE;
}

static PyObject *py_forward_remove_tcp(PyObject *self, PyObject *args, PyObject *keywds) {
    unsigned short local;
    unsigned short port=DEFAULT_ADB_PORT;
    static char *kwlist[] = {"local", "port",NULL};
    if(!PyArg_ParseTupleAndKeywords(args,keywds, "H|H",kwlist, &local,&port)) {
        return NULL;
    }
    if(!forward_remove_tcp(local, port)) {
        Py_RETURN_FALSE;
    }
    Py_RETURN_TRUE;
}

static PyObject *py_forward_list(PyObject *self, PyObject *args, PyObject *keywds) {
    unsigned short port = DEFAULT_ADB_PORT;
    static char *kwlist[] = {"port", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|H", kwlist, &port)) {
        return NULL;
    }
    char *list = forward_list(port);
    return Py_BuildValue("s", list);
}

static PyMethodDef ADBMethods[] = {
    {
        "start_server", (PyCFunction)py_start_server, METH_VARARGS | METH_KEYWORDS,
        "启动adb守护进程"
    },
    {
        "kill_server", (PyCFunction)py_kill_server, METH_VARARGS | METH_KEYWORDS,
        "关闭adb守护进程"
    },
    {
        "install_apk", (PyCFunction)py_install_apk, METH_VARARGS | METH_KEYWORDS,
        "安装APK"
    },
    {
        "forward_tcp", (PyCFunction)py_forward_tcp, METH_VARARGS | METH_KEYWORDS,
        "重定向TCP端口"
    },
    {
        "forward_remove_tcp", (PyCFunction)py_forward_remove_tcp, METH_VARARGS | METH_KEYWORDS,
        "删除TCP端口重定向"
    },
    {
        "forward_list", (PyCFunction)py_forward_list, METH_VARARGS | METH_KEYWORDS,
        "返回端口重定向的列表"
    },
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef ADBModule = {
    PyModuleDef_HEAD_INIT,
    "pyadb",
    NULL,
    -1,
    ADBMethods
};

PyMODINIT_FUNC
PyInit_pyadb(void) {
    return PyModule_Create(&ADBModule);
}

