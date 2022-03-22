#!/bin/env bash
SCRIPTDIR=$(dirname $(readlink -f $BASH_SOURCE))
NSDK_ROOT=${NSDK_ROOT:-${SCRIPTDIR}/../../../../../nuclei-sdk}
NMSIS_ROOT=${NMSIS_ROOT:-${SCRIPTDIR}/../../../}

NSDK_ROOT=$(readlink -f ${NSDK_ROOT})
NMSIS_ROOT=$(readlink -f ${NMSIS_ROOT})

echo "Export NUCLEI_SDK_ROOT and NUCLEI_SDK_NMSIS"
export NUCLEI_SDK_ROOT=$NSDK_ROOT
export NUCLEI_SDK_NMSIS=$NMSIS_ROOT
unset NSDK_ROOT NMSIS_ROOT

echo "NUCLEI_SDK_ROOT=$NUCLEI_SDK_ROOT"
echo "NUCLEI_SDK_NMSIS=$NUCLEI_SDK_NMSIS"

echo "Only copy elf and map file when do bench"
export SDK_COPY_OBJECTS="elf,map"