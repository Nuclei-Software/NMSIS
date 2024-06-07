#!/bin/env bash
SCRIPTDIR=$(dirname $(readlink -f $BASH_SOURCE))
NSDK_ROOT=${NSDK_ROOT:-${SCRIPTDIR}/../../../../../nuclei-sdk}
NMSIS_ROOT=${NMSIS_ROOT:-${SCRIPTDIR}/../../../}

ILMSZ=${ILMSZ:-512K}
DLMSZ=${DLMSZ:-512K}
DEVENVSH=${DEVENVSH:-/home/share/devtools/env.sh}

[ -f ${DEVENVSH} ] && source $DEVENVSH

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

LDSCRIPT=$NUCLEI_SDK_ROOT/SoC/evalsoc/Board/nuclei_fpga_eval/Source/GCC/gcc_evalsoc_ilm.ld
echo "Change ILM size from 64K to $ILMSZ in $LDSCRIPT"
sed -i "s/ORIGIN = 0x80000000, LENGTH = 64K/ORIGIN = 0x80000000, LENGTH = $ILMSZ/g" $LDSCRIPT
echo "Change DLM size from 64K to $DLMSZ in $LDSCRIPT"
sed -i "s/ORIGIN = 0x90000000, LENGTH = 64K/ORIGIN = 0x90000000, LENGTH = $DLMSZ/g" $LDSCRIPT
# change ilm/dlm size from 64K to 1M for evalsoc from Nuclei SDK 0.6.0
EVALSOC_MEMORY=$NUCLEI_SDK_ROOT/SoC/evalsoc/Board/nuclei_fpga_eval/Source/GCC/evalsoc.memory
if [ -f $EVALSOC_MEMORY ] ; then
    echo "You are using Nuclei SDK >= 0.6.0"
    echo "Change ILM memory size to $ILMSZ in $EVALSOC_MEMORY"
    sed -i "s/\(ILM_MEMORY_SIZE\).*/\1 = $ILMSZ;/" $EVALSOC_MEMORY
    echo "Change DLM memory size to $DLMSZ in $EVALSOC_MEMORY"
    sed -i "s/\(DLM_MEMORY_SIZE\).*/\1 = $DLMSZ;/" $EVALSOC_MEMORY
fi
