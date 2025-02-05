from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import pybind11

# Run:
# python3 pgn_reader_setup.py build_ext --inplace
# To get .so file

# Custom build_ext to add include dirs for Pybind11
class BuildExt(build_ext):
    """Custom build_ext to add include dirs for Pybind11"""
    def build_extensions(self):
        for ext in self.extensions:
            ext.include_dirs.append(pybind11.get_include())
        build_ext.build_extensions(self)

# Define the extension module
pgn_reader_module = Extension(
    'pgn_reader',
    sources=['pgn_reader.cpp'],
    include_dirs=[pybind11.get_include()],
    language='c++',
    extra_compile_args=['-std=c++17', '-O3'], 
)

# Setup the module
setup(
    name='pgn_reader',
    version='1.0',
    description='Python bindings for HPCE PGN Reader',
    ext_modules=[pgn_reader_module],
    cmdclass={'build_ext': BuildExt},  # Use custom build class
    zip_safe=False,
)
