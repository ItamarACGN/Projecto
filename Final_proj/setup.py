from setuptools import Extension, setup

module = Extension("symnmfmodule", sources=['symnmf.c','matrix.c', 'symnmfmodule.c'])
setup(name='symnmfmodule',
     version='1.0',
     description='Python wrapper for symnmf C functions',
     ext_modules=[module])