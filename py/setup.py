from setuptools import setup, Extension
from Cython.Build import cythonize

ext_modules = [
    Extension(
        "digisim", ["digisim.pyx"],
        include_dirs=["../include"],
        libraries=["digisim"],
        library_dirs=["./build"]
    )
]

setup(
    name='Digisim Bindings',
    ext_modules=cythonize(ext_modules),
)
