"""#####################################################################
# File name: setup.py
# Version: v1.0
# Dev: GitHub@Rr42
# Description:
#  A setuptools based setup module for the 8x11 LED Raspberry Pi Python
#   display library.
#####################################################################"""

# Always prefer setuptools over distutils
from setuptools import setup

setup(
    name='led8x11displaylib',
    version='0.0.1',
    description='Python library for Raspberry Pi to drive the 8x11 LED display',
    url='https://github.com/Rr42/LED-display-8x11',
    author='Ramana R',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Operating System :: POSIX :: Linux',
        'License :: OSI Approved :: MIT License',
        'Intended Audience :: Developers',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3 :: Only',
        'Topic :: Software Development',
        'Topic :: Home Automation',
        'Topic :: System :: Hardware'
    ],
    license='MIT',
    keywords='Raspberry Pi LED display',
    packages=['led8x11displaylib'],
    python_requires='>=3.5, <3.7',
    install_requires=['RPi.GPIO'])