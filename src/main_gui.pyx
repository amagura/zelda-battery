cdef extern from "power.h":
    struct py_power:
        pass

cdef extern from "power.h":
    py_power py_getpwr()

cdef:
    a = py_getpwr()
print a.charge
