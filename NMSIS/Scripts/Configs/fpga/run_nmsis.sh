CONFIG=${CONFIG:-n300,ux900}
LOGROOT=${LOGROOT:-logs}
BACKUP=${BACKUP:-Backups}
CFGSET=${CFGSET:-mini}
BITSET=${BITSET:-latest}

SCRIPTDIR=$(dirname $(readlink -f $BASH_SOURCE))
CFGLOC=
FPGALOC=

function env_help() {
    echo "You can easily setup environment by execute command below"
    echo "source ${SCRIPTDIR}/Scripts/Configs/fpga/setup.sh"
    echo "Make sure nuclei-sdk folder is lay with the same level as NMSIS project folder"
}

if [ "x${NUCLEI_SDK_ROOT}" == "x" ] ; then
    echo "Please export NUCLEI_SDK_ROOT variable to correct path"
    env_help
    exit 1
fi

if [ "x${NUCLEI_SDK_NMSIS}" == "x" ] ; then
    echo "Please export NUCLEI_SDK_NMSIS variable to correct path"
    env_help
    exit 1
fi

function backup_logs {
    if [ -d $LOGROOT ] ; then
        mkdir -p $BACKUP
        local backupfile=$BACKUP/fpgarun_$(date -u +"%Y%m%d-%H%M%S").tar.gz
        echo "Backup old logs to $backupfile"
        tar -czf $backupfile $LOGROOT
        rm -rf $LOGROOT
    fi
}

function setup_suite {
    local cfgloc=${1:-mini}
    local fpgaloc=${2:-latest}
    local configpath=${SCRIPTDIR}/configs/${cfgloc}
    local fpgapath=${SCRIPTDIR}/bitstreams/$fpgaloc

    if [ ! -d $configpath ] ; then
        echo "Can't find proper test suite $cfgloc located in $configpath"
        echo "Please pass correct CFGSET value, such as CFGSET=mini or CFGSET=full"
        exit 1
    fi
    if [ ! -d $fpgapath ] ; then
        echo "Can't find proper test suite $fpgaloc located in $fpgapath"
        echo "Please pass correct FPGASET value, such as FPGASET=latest or FPGASET=202206"
        exit 1
    fi
    CFGLOC=$configpath
    FPGALOC=$fpgapath
}

function run_nmsis {
    local lib=${1:-nmsis_nn}

    local logdir=${LOGROOT}/${lib}

    if [ "x$CFGLOC" == "x" ] || [ "x$FPGALOC" == "x" ] ; then
        echo "Can't find any fpga location and config location, please check"
        return
    fi

    mkdir -p $logdir

    local runcmd="python3 $NUCLEI_SDK_ROOT/tools/scripts/nsdk_cli/nsdk_runner.py --appyaml ${NUCLEI_SDK_NMSIS}/Scripts/Configs/fpga/${lib}.yaml --logdir ${logdir} --runon fpga --config ${CONFIG} --cfgloc ${CFGLOC} --fpgaloc ${FPGALOC}"
    echo $runcmd
    eval $runcmd | tee $logdir/run.log
}

function gen_report {
    local logdir=${1:-logs}

    local runcmd="python3 $NUCLEI_SDK_ROOT/tools/scripts/nsdk_cli/nsdk_report.py --logdir ${logdir} --split --run"
    echo $runcmd
    eval $runcmd | tee $logdir/run.log
}

backup_logs
setup_suite ${CFGSET} ${FPGASET}

run_nmsis nmsis_nn
run_nmsis nmsis_dsp
gen_report ${LOGROOT}
