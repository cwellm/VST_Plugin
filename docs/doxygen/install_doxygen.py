from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
import requests
from requests import HTTPError
import zipfile
import argparse

import conan.cli.cli as c

import os
import shutil
import subprocess
from pathlib import Path

from conan.tools.files import download, unzip, check_sha1

DOXYGEN_VERSION = "1.9.6"

class DOXYGENInstallation:

    def __init__(self, doxygen_version):
        self.__doxygen_version = doxygen_version
        self.__version_string = "_".join(self.__doxygen_version.split("."))
        self.__this_folder = str(Path(__file__).parent.resolve())
        # The download folder is not customizable, but given by the directory name in the .zip file.
        self.__doxygen_download_folder = "/".join([self.__this_folder, "doxygen-Release_" + self.__version_string])
        self.__doxygen_gen_folder = self.__doxygen_download_folder + "/build"
        self.__doxygen_binary_dir = self.__doxygen_gen_folder + "/bin"

    def generate(self):

        # configure and install the JUCE package
        print("Generating Doxygen build tree...")
        subprocess.run(["cmake", "-S", self.__doxygen_download_folder, "-B", self.__doxygen_gen_folder])

    def build(self):
        print(f"Installing Doxygen version {self.__doxygen_version}")
        if os.path.exists(Path(self.__doxygen_binary_dir)):
            print("Apparently, the binary has already been built. Aborting build.")
            print(f"In case this message is unexpected, please remove {self.__doxygen_gen_folder} and try again.")
            return

        subprocess.run(["cmake", "--build", self.__doxygen_gen_folder])

    def get_source(self):

        zip_name = f"{self.__this_folder}/doxygen-Release_{self.__version_string}.zip"
        # Immutable source .zip
        if not os.path.exists(Path(self.__doxygen_download_folder)):
            print("Downloading Doxygen source...")
            print("Version string: ", self.__version_string)
            r = requests.get("https://github.com/doxygen/doxygen/archive/refs/tags/"
                             f"Release_{self.__version_string}.zip", allow_redirects=True)
            response_code = r.status_code
            if (response_code > 399):
                raise HTTPError(f"Error while trying to download Doxygen Release file: Error code {response_code}")
            with open(zip_name, "wb") as file:
                file.write(r.content)
            with zipfile.ZipFile(zip_name, "r") as zip_file:
                zip_file.extractall(self.__this_folder)

    def get_binary_dir(self):
        return self.__doxygen_binary_dir

def execute(doxygen_version=DOXYGEN_VERSION):
    installer = DOXYGENInstallation(doxygen_version)
    installer.get_source()
    installer.generate()
    installer.build()

    return installer.get_binary_dir()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
                    prog='Doxygen Installer',
                    description='Installs Doxygen, including the CLI to generate documentation.'
            )
    parser.add_argument('-t', '--tag', required=False, help="Specifies the Doxygen version to be installed, by its "
                        f"GitHub tag. Defaults to {DOXYGEN_VERSION}.", default=DOXYGEN_VERSION)

    args = parser.parse_args()
    execute(args.tag)
