#!/usr/bin/env python
import re
from distutils.core import setup, Extension

setup (name = "cedar-python",
       py_modules = ['cedar'],
       ext_modules = [Extension ('_cedar',
                                 ['cedar_wrap.cxx'],
                                 include_dirs=['..', '../..', '../../src'])])
