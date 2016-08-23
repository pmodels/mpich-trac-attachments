#!/bin/bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HGR/External/mpich2/1.0.7/lib/$MAKEFILE_PLATFORM
exec ./testslave
