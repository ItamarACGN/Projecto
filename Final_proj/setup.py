from setuptools import Extension, setup

module = Extension("capi_demo1", sources=['kmeans.c','genmodule.c'])
setup(name='capi_demo1',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])