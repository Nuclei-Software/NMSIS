#!/bin/env bash
LOGROOT=${LOGROOT:-gen}
DRYRUN=${DRYRUN:-0}
RUNON=${RUNON-nothing}
TOOL_VER=${TOOL_VER:-2022.01}

SCRIPTDIR=$(dirname $(readlink -f $BASH_SOURCE))

DEVTOOL_ENV=${DEVTOOL_ENV:-/home/share/devtools/env.sh}

TOOL_VER=$TOOL_VER source $DEVTOOL_ENV

source $SCRIPTDIR/setup.sh
LOGROOT=$(readlink -f $LOGROOT)

if [ ! -d $LOGROOT ] ; then
    mkdir -p $LOGROOT
fi

function describe_repo {
    local repodir=${1}
    local repodesc=${2:-repogit.txt}
    repodir=$(readlink -f $repodir)
    if [ -d ${repodir}/.git  ] ; then
        pushd ${repodir}
        echo "Git Repo $repodir Information:" >> ${repodesc}
        gitver=$(git describe --tags --always --abbrev=10 --dirty)
        gitslog=$(git log --oneline -1)
        echo "git describe version: $gitver" >> ${repodesc}
        echo "git shortlog: $gitslog" >> ${repodesc}
        git submodule >> ${repodesc}
        popd
    else
        echo "$repodir not a git repo" >> ${repodesc}
    fi
}

function describe_build {
    logfile=$1
    echo -n "Build Date: " > $logfile
    date >> $logfile
    echo "Nuclei GNU Toolchain Version:" >> $logfile
    riscv64-unknown-elf-gcc -v >> $logfile 2>&1
}

function record_buildinfo {
    BUILDTXT=$LOGROOT/build.txt

    describe_build $BUILDTXT
    describe_repo $NUCLEI_SDK_ROOT $BUILDTXT
    describe_repo $NUCLEI_SDK_NMSIS/.. $BUILDTXT
}

function changelinkscript {
    echo "Change evalsoc linker script to 512K"
    sed -i "s/64K/512K/g" $NUCLEI_SDK_ROOT/SoC/evalsoc/Board/nuclei_fpga_eval/Source/GCC/gcc_evalsoc_ilm.ld
}

record_buildinfo

changelinkscript

NSDK_RUNNER_PY="$NUCLEI_SDK_ROOT/tools/scripts/nsdk_cli/nsdk_runner.py"

pushd $NUCLEI_SDK_NMSIS
for lib in nmsis_dsp nmsis_nn
do
    RUNNER_CMD="python3 $NSDK_RUNNER_PY --appyaml $SCRIPTDIR/$lib.yaml --logdir $LOGROOT/$lib --runon $RUNON --cfgloc $SCRIPTDIR"
    echo $RUNNER_CMD
    if [[ $DRYRUN == 0 ]] ; then
        eval $RUNNER_CMD
    fi
done
popd
