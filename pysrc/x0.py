
from ctypes import *;


import ctypes

testlib = ctypes.CDLL('/home/phillip/PycharmProjects/mcfc-xx/src/testlib.so')
testlib.do_mc(99)

print("frodo livesx");