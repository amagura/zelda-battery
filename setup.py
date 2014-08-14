from distutils.core import setup
from Cython.build import cythonize

setup(
  name = 'zelda-battery',
  ext_modules = cythonize('zbat.py'),
)
