#!/bin/env bash
SCRIPTDIR=$(dirname $(readlink -f $BASH_SOURCE))
NSDK_ROOT=${NSDK_ROOT:-${SCRIPTDIR}/../../../../../nuclei-sdk}
NMSIS_ROOT=${NMSIS_ROOT:-${SCRIPTDIR}/../../../}

if [ "x${NSDK_ROOT}" == "x" ] ; then
    echo "Please export NSDK_ROOT variable to correct path"
    exit 1
fi

if [ "x${NMSIS_ROOT}" == "x" ] ; then
    echo "Please export NMSIS_ROOT variable to correct path"
    echo "such as export NMSIS_ROOT=/path/to/NMSIS/NMSIS"
    exit 1
fi

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

LDSCRIPT=$NUCLEI_SDK_ROOT/SoC/demosoc/Board/nuclei_fpga_eval/Source/GCC/gcc_demosoc_ilm.ld
echo "Change ILM size from 64K to 512K in $LDSCRIPT"

sed -i "s/64K/512K/g" $LDSCRIPT
