CONFIG=${CONFIG:-n300,ux900}
LOGROOT=${LOGROOT:-logs}
BACKUP=${BACKUP:-Backups}

SCRIPTDIR=$(dirname $(readlink -f $BASH_SOURCE))

function help() {
    echo "You can easily setup environment by execute command below"
    echo "source ${SCRIPTDIR}/Scripts/Configs/fpga/setup.sh"
    echo "Make sure nuclei-sdk folder is lay with the same level as NMSIS project folder"
}

if [ "x${NUCLEI_SDK_ROOT}" == "x" ] ; then
    echo "Please export NUCLEI_SDK_ROOT variable to correct path"
    exit 1
fi

if [ "x${NUCLEI_SDK_NMSIS}" == "x" ] ; then
    echo "Please export NUCLEI_SDK_NMSIS variable to correct path"
    exit 1
fi

if [ -d $LOGROOT ] ; then
    mkdir -p $BACKUP
    backupfile=$BACKUP/fpgarun_$(date -u +"%Y%m%d-%H%M%S").tar.gz
    echo "Backup old logs to $backupfile"
    tar -czf $backupfile $LOGROOT
    rm -rf $LOGROOT
fi

function run_nmsis {
    local lib=${1:-nmsis_nn}

    local logdir=${LOGROOT}/${lib}

    mkdir -p $logdir

    local runcmd="python3 $NUCLEI_SDK_ROOT/tools/scripts/nsdk_cli/nsdk_runner.py --appyaml ${NUCLEI_SDK_NMSIS}/Scripts/Configs/fpga/${lib}.yaml --logdir ${logdir} --runon fpga --config ${CONFIG}"
    echo $runcmd
    eval $runcmd | tee $logdir/run.log
}

run_nmsis nmsis_nn
run_nmsis nmsis_dsp
