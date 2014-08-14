from distutils import ccompiler
from fabricate import *
from Cython.Build import cythonize

sources = [ 'zbat' ]

def build():
  compile()
  link()

def compile():
  for src in sources:
    run('cython2', '-p -t -v -a --embed', src);

print ccompiler.show_compilers()
main()
