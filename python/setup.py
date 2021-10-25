#! /usr/bin/env python

from setuptools import setup

__version__ = '1.0.0'

setup(
    name='PeakFinder',
    version=__version__,
    description='A library for identifying peaks in line data.',
    url='https://github.com/msimms/PeakFinder',
    packages=['peakfinder',],
    author='Mike Simms',
    author_email='mike@mikesimms.net',
    license='MIT',
    install_requires=[],
    python_requires='>=3.5'
)
