import subprocess

from docs.doxygen.install_doxygen import execute as doxygen_installation


doxygen_dir = doxygen_installation()
subprocess.run([f"{doxygen_dir}/doxygen", "Doxyfile"])