from setuptools import setup, Extension
import pybind11

# Define the extension module
chess_engine_module = Extension(
    'hpce_pgn_reader',
    sources=['pgn_reader.cpp'],
    include_dirs=[pybind11.get_include()],
    language='c++',
    extra_compile_args=['-std=c++11'],  # Adjust C++ standard as needed
)

# Setup the module
setup(
    name='hpce_pgn_reader',
    version='1.0',
    description='Python bindings for HPCE',
    ext_modules=[chess_engine_module],
)