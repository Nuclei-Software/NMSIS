function run_nn_example() {
    local target=${2:-"run"}
    echo "Run nn library examples in $1 directory"
    make -B $MAKE_OPTIONS $target -C $1
}

function run_all_nn_examples() {
    local testdir=$1
    local target=${2:-"run"}

    local count=0
    local makefiles=($(find $testdir -type f -name "Makefile"))
    local totalcount=${#makefiles[@]}

    local failed_cases=""
    for makefile in ${makefiles[@]} ; do
        testcase=$(dirname $makefile)
        RUN_CMD="run_nn_example $testcase"
        if eval $RUN_CMD ; then
            echo "Run test example in $testcase successfully"
            testval=$((count++))
        else
            echo "Run test example in $testcase failed"
            failed_cases="$failed_cases\r\n    $testcase"
        fi
    done

    echo ""
    echo "Successfully build $count out of $totalcount test cases"
    if [ "$failed_cases" != "" ] ; then
        echo "=====Failed testcases(DSP_ENABLE=$DSP_ENABLE) list as below:====="
        echo -e "$failed_cases\r\n"
        exit 1
    fi
}
