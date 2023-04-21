from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
import requests
import zipfile

import conan.cli.cli as c

import os
import shutil
import subprocess
from pathlib import Path

from conan.tools.files import download, unzip, check_sha1

JUCE_VERSION = "7.0.5"

class JUCEInstallation:

    def __init__(self, juce_version):
        self.__juce_version = juce_version
        self.__this_folder = str(Path(__file__).parent.resolve())
        self.__juce_download_folder = self.__this_folder + f"/JUCE-{self.__juce_version}"
        self.__juce_gen_folder = self.__juce_download_folder + "/build"

    SRC_FOLDER = "src"

    def generate(self, installation_directory):

        # configure and install the JUCE package
        print("Generating JUCE build tree...")
        subprocess.run(["cmake", "-S", self.__juce_download_folder, "-B", self.__juce_gen_folder, 
                        f"-DCMAKE_INSTALL_PREFIX={installation_directory}"])

    def build(self):
        print(f"Installing JUCE version {self.__juce_version}")
        subprocess.run(["cmake", "--build", self.__juce_gen_folder, "--target=install"])

    def get_source(self):
        zip_name = f"{self.__this_folder}/juce.zip"
        # Immutable source .zip
        if not os.path.exists(Path(self.__juce_download_folder)):
            print("Downloading JUCE source...")
            r = requests.get("https://github.com/juce-framework/JUCE/archive/refs/tags/"\
                             f"{self.__juce_version}.zip")
            with open(zip_name, "wb") as file:
                file.write(r.content)
            with zipfile.ZipFile(zip_name, "r") as zip_file:
                zip_file.extractall(self.__this_folder)

def execute(juce_version, installation_directory):
    installer = JUCEInstallation(juce_version)
    installer.get_source()
    installer.generate(installation_directory)
    installer.build()
