#!/bin/sh
# BHEADER ####################################################################
#
# Copyright (c) 2018, Lawrence Livermore National Security, LLC.
# Produced at the Lawrence Livermore National Laboratory.
# LLNL-CODE-745247. All Rights reserved. See file COPYRIGHT for details.
#
# This file is part of smoothG. For more information and source code
# availability, see https://www.github.com/llnl/smoothG.
#
# smoothG is free software; you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License (as published by the Free
# Software Foundation) version 2.1 dated February 1999.
#
#################################################################### EHEADER #


# This is the path to the root of the git repo
# the BASE_DIR should contain smoothG_config.h.in
BASE_DIR=${PWD}

# this is where we actually build binaries and so forth
BUILD_DIR=${BASE_DIR}/build

EXTRA_ARGS=$@

# Force a reconfigure
rm CMakeCache.txt
rm -rf CMakeFiles

# Build Options
EXTERN_DIR=${PWD}/extern
METIS_DIR=$EXTERN_DIR/metis
HYPRE_DIR=$EXTERN_DIR/hypre
SUITESPARSE_DIR=$EXTERN_DIR/SuiteSparse
BUILD_TYPE=Debug

mkdir -p $BUILD_DIR
cd $BUILD_DIR

CC=mpicc CXX=mpic++ cmake \
    -DLINALGCPP_ENABLE_MPI=Yes \
    -DLINALGCPP_ENABLE_METIS=Yes \
    -DLINALGCPP_ENABLE_SUITESPARSE=Yes \
    -DHypre_INC_DIR=$HYPRE_DIR/include \
    -DHypre_LIB_DIR=$HYPRE_DIR/lib \
    -DMETIS_DIR=$METIS_DIR \
    -DSUITESPARSE_INCLUDE_DIR_HINTS=$SUITESPARSE_DIR/include \
    -DSUITESPARSE_LIBRARY_DIR_HINTS=$SUITESPARSE_DIR/lib \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    $BASE_DIR \
    $EXTRA_ARGS

make -j 3
