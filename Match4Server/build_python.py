from distutils.core import setup, Extension
import fnmatch
import os

source_files = [];
for file in os.listdir('.'):
    if fnmatch.fnmatch(file, '*.cpp'):
        source_files.append(file)

module1 = Extension('match4server',
                    sources = source_files)

setup (name = 'Match4ServerModule',
       version = '1.0',
       description = 'Match4Server',
       ext_modules = [module1])