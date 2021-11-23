ENVSCRIPT=$(readlink -f ${BASH_SOURCE[0]})
ENVSCRIPT_DIR=$(dirname $ENVSCRIPT)

MAKEFILE_CORE_FILE=$ENVSCRIPT_DIR/Makefile.core
NMSIS_RUNTIMEOUT=${NMSIS_RUNTIMEOUT:-6m}

export DEFAULT_SUPPORTED_CORES=$(cat $MAKEFILE_CORE_FILE | grep SUPPORTED | cut -d "=" -f 2)

##
# This function will run selected applications with fixed core configuration
# para 1: application directory, by default, it will be "."
# para 2: make target, by default, it will be "run"
# You can pass ARCH_EXT=p to enable dsp run or ARCH_EXT=v to enable vector
# or ARCH_EXT=pv to enable dsp and vector
# eg.
# CORE=nx600f ARCH_EXT=pv nmsis_run_app .
# CORE=n307 ARCH_EXT= nmsis_run_app .
##
function nmsis_run_app() {
    local appdir=${1:-"."}
    local target=${2:-"run"}
    echo "Run application with make options($MAKE_OPTIONS) in $1 directory"
    appdirshow=$appdir
    if [ "$appdir" == "." ] ; then
        appdirshow=$(basename $(readlink -f $appdir))
    fi
    echo "CSV, APP=$appdirshow, CORE=$CORE, ARCH_EXT=$ARCH_EXT, BENCH_UNIT=$BENCH_UNIT"
    runcmd="timeout --foreground -s SIGKILL $NMSIS_RUNTIMEOUT make -B $MAKE_OPTIONS $target -C $appdir"
    echo $runcmd
    eval $runcmd
}

##
# This function will run all the applications with fixed core configuration
# para 1: application directory root, by default, it will be "."
# para 2: make target, by default, it will be "run"
# You can pass ARCH_EXT=p to enable dsp run or ARCH_EXT=v to enable vector
# or ARCH_EXT=pv to enable dsp and vector
# eg.
# CORE=nx600f ARCH_EXT=pv nmsis_run_apps .
# CORE=n307 ARCH_EXT= nmsis_run_apps .
##
function nmsis_run_apps() {
    local appdir=${1:-"."}
    local target=${2:-"run"}
    local ignore_pattern=$FIND_IGNORE

    local count=0
    local find_cmd="find $appdir -type f -name \"Makefile\" $ignore_pattern"
    echo $find_cmd
    local makefiles=($(eval $find_cmd))
    local totalcount=${#makefiles[@]}

    local failed_cases=""
    for makefile in ${makefiles[@]} ; do
        appcase=$(dirname $makefile)
        RUN_CMD="nmsis_run_app $appcase $target"
        if eval $RUN_CMD ; then
            echo "Run application in $appcase successfully"
            appval=$((count++))
        else
            echo "Run application in $appcase failed"
            failed_cases="$failed_cases\r\n    $appcase"
        fi
    done

    echo ""
    echo "Successfully build and run $count out of $totalcount application cases(ARCH_EXT=$ARCH_EXT)"
    if [ "$failed_cases" != "" ] ; then
        echo "=====Failed appcases(ARCH_EXT=$ARCH_EXT) list as below:====="
        echo -e "$failed_cases\r\n"
        return 1
    fi
    return 0
}

##
# This function will run all the applications with different core configurations
# para 1: core_configurations, such as "n307 nx600", by default, it will use
#         variable value of DEFAULT_SUPPORTED_CORES
# para 2: application directory root, by default it will be "."
# para 3: make target, by default will be "run"
# You can pass ARCH_EXT=p to enable dsp run or ARCH_EXT=v to enable vector
# or ARCH_EXT=pv to enable dsp and vector
# Beware of the cores that vector supported
# eg.
# ARCH_EXT=pv nmsis_run_all_apps "nx600f nx600fd"
# ARCH_EXT= nmsis_run_all_apps "n307 nx600"
##
function nmsis_run_all_apps () {
    local supported_cores=${1:-$DEFAULT_SUPPORTED_CORES}
    local approot=${2:-"."}
    local target=${3:-"run"}

    supported_cores=(${supported_cores//,/ })

    echo "Run for these cores: ${supported_cores[@]}"
    sleep 3
    local count=0
    local totalcount=${#supported_cores[@]}
    local failed_cases=""
    for core in ${supported_cores[@]} ; do
        RUN_CMD="CORE=$core ARCH_EXT=$ARCH_EXT nmsis_run_apps $approot $target"
        if eval $RUN_CMD ; then
            echo "Run all applications in $approot successfully with CORE=$core"
            appval=$((count++))
        else
            echo "Run all applications in $approot failed with CORE=$core"
            failed_cases="$failed_cases\r\n    CORE=$core"
        fi
    done

    echo ""
    echo "Successfully build and run $count out of $totalcount core configurations(ARCH_EXT=$ARCH_EXT)"
    if [ "$failed_cases" != "" ] ; then
        echo "=====Failed core configurations(ARCH_EXT=$ARCH_EXT) list as below:====="
        echo -e "$failed_cases\r\n"
        return 1
    fi
    return 0
}
