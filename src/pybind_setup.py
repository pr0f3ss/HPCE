from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import os
import shutil
import pybind11

# Compiles hpce and pgn_reader .so files with
# python3 pybind_setup.py build_ext --inplace

# Custom build_ext to add include dirs for Pybind11
class BuildExt(build_ext):
    """Custom build_ext to add include dirs for Pybind11"""
    def build_extensions(self):
        for ext in self.extensions:
            ext.include_dirs.append(pybind11.get_include())
        build_ext.build_extensions(self)

# Define the extension module for hpce (including pgn_reader.cpp)
hpce_module = Extension(
    'hpce',  
    sources=['hpce.cpp', 'pgn_reader.cpp'],
    include_dirs=[pybind11.get_include()],
    language='c++',
    extra_compile_args=['-std=c++17', '-O3'],
)

# Setup the module
setup(
    name='hpce',
    version='1.0',
    description='Python bindings for HPCE',
    ext_modules=[hpce_module],
    cmdclass={'build_ext': BuildExt},  # Use custom build class
    zip_safe=False,
)

# Move compiled .so files to hpce_model directory
build_dir = "hpce_model"
if not os.path.exists(build_dir):
    os.makedirs(build_dir)

for file in os.listdir():
    if file.endswith(".so") and (file.startswith("pgn_reader") or file.startswith("hpce")):
        shutil.move(file, os.path.join(build_dir, file))
