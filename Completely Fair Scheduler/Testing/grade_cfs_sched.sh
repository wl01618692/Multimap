#!/bin/bash

# Safer execution
# -e: exit immediately if a command fails
# -E: Safer -e option for traps
# -u: fail if a variable is used unset
# -o pipefail: exit immediately if command in a pipe fails
#set -eEuo pipefail
# -x: print each command before executing (great for debugging)
#set -x

# Convenient values
SCRIPT_NAME=$(basename $BASH_SOURCE)

# Default program values
EXEC="${PWD}/cfs_sched"
TEST_CASE="all"

#
# Logging helpers
#
log() {
    echo -e "${*}"
}
info() {
    log "Info: ${*}"
}
warning() {
    log "Warning: ${*}"
}
error() {
    log "Error: ${*}"
}
die() {
    error "${*}"
    exit 1
}

#
# Line comparison
#
select_line() {
    # 1: string
    # 2: line to select
    echo "$(echo "${1}" | sed "${2}q;d")"
}

fail() {
    # 1: got
    # 2: expected
    log "Fail: got '${1}' but expected '${2}'"
}

pass() {
    # got
    log "Pass: ${1}"
}

compare_lines() {
    # 1: output
    # 2: expected
    # 3: score (output)
    declare -a output_lines=("${!1}")
    declare -a expect_lines=("${!2}")
    local __score=$3
    local partial="0"

    # Amount of partial credit for each correct output line
    local step=$(bc -l <<< "1.0 / ${#expect_lines[@]}")

    # Compare lines, two by two
    for i in ${!output_lines[*]}; do
        # Exact matching for this tester
        if [[ "${output_lines[${i}]}" == "${expect_lines[${i}]}" ]]; then
            pass "${output_lines[${i}]}"
            partial=$(bc <<< "${partial} + ${step}")
        else
            fail "${output_lines[${i}]}" "${expect_lines[${i}]}" ]]
        fi
    done

    # Return final score
    eval ${__score}="'${partial}'"
}

#
# Run generic test case
#
run_test() {
    #1: cli arguments
    local args=("${@}")

    # These are global variables after the test has run so clear them out now
    unset STDOUT STDERR RET

    # Create temp files for getting stdout and stderr
    local outfile=$(mktemp)
    local errfile=$(mktemp)

    bash -c "${EXEC} ${args[*]}" \
        >${outfile} 2>${errfile}

    # Get the return status, stdout and stderr of the test case
    RET="${?}"
    STDOUT=$(cat "${outfile}")
    STDERR=$(cat "${errfile}")

    # Clean up temp files
    rm -f "${outfile}"
    rm -f "${errfile}"
}

#
# Test cases
#
TEST_CASES=()

## Error management args 1 (no arg)
TEST_CASES+=("err_no_arg")
err_no_arg() {
    log "--- Running test case: ${FUNCNAME} ---"
    run_test ""

    local line_array=()
    line_array+=("$(select_line "${STDERR}" "1")")
    line_array+=("Return code: '${RET}'")
    local corr_array=()
    corr_array+=("Usage: ${EXEC} <task_file.dat>")
    corr_array+=("Return code: '1'")

    local score
    compare_lines line_array[@] corr_array[@] score
    log "${score}"
}

## Error management files (wrong file) + error ret code
TEST_CASES+=("err_wrong_task_file")
err_wrong_task_file() {
    log "--- Running test case: ${FUNCNAME} ---"
    run_test "wrong_task_file.dat"

    local line_array=()
    line_array+=("$(select_line "${STDERR}" "1")")
    line_array+=("Return code: '${RET}'")
    local corr_array=()
    corr_array+=("Error: cannot open file wrong_task_file.dat")
    corr_array+=("Return code: '1'")

    local score
    compare_lines line_array[@] corr_array[@] score
    log "${score}"
}

## Result validation: one tasks starting from beginning
TEST_CASES+=("task_one_beg")
task_one_beg() {
    log "--- Running test case: ${FUNCNAME} ---"
	rm -f *.dat
    echo -e "A 0 5\n" > task.dat
    run_test "task.dat"

    local line_array=()
    line_array+=("$(select_line "${STDOUT}" "1")")
    line_array+=("$(select_line "${STDOUT}" "2")")
    line_array+=("$(select_line "${STDOUT}" "3")")
    line_array+=("$(select_line "${STDOUT}" "5")")
    local corr_array=()
    corr_array+=("0 [1]: A")
    corr_array+=("1 [1]: A")
    corr_array+=("2 [1]: A")
    corr_array+=("4 [1]: A*")

    local score
    compare_lines line_array[@] corr_array[@] score
    log "${score}"
}

## Result validation: one tasks not starting from beginning
TEST_CASES+=("task_one_mid")
task_one_mid() {
    log "--- Running test case: ${FUNCNAME} ---"
	rm -f *.dat
    echo -e "A 2 5\n" > task.dat
    run_test "task.dat"

    local line_array=()
    line_array+=("$(select_line "${STDOUT}" "1")")
    line_array+=("$(select_line "${STDOUT}" "2")")
    line_array+=("$(select_line "${STDOUT}" "6")")
    line_array+=("$(select_line "${STDOUT}" "7")")
    local corr_array=()
    corr_array+=("0 [0]: _")
    corr_array+=("1 [0]: _")
    corr_array+=("5 [1]: A")
    corr_array+=("6 [1]: A*")

    local score
    compare_lines line_array[@] corr_array[@] score
    log "${score}"
}

## Result validation: two tasks, no overlap
TEST_CASES+=("task_2_nooverlap")
task_2_nooverlap() {
    log "--- Running test case: ${FUNCNAME} ---"
	rm -f *.dat
    echo -e "A 0 3\nB 4 2" > task.dat
    run_test "task.dat"

    local line_array=()
    line_array+=("$(select_line "${STDOUT}" "1")")
    line_array+=("$(select_line "${STDOUT}" "3")")
    line_array+=("$(select_line "${STDOUT}" "4")")
    line_array+=("$(select_line "${STDOUT}" "5")")
    line_array+=("$(select_line "${STDOUT}" "6")")
    local corr_array=()
    corr_array+=("0 [1]: A")
    corr_array+=("2 [1]: A*")
    corr_array+=("3 [0]: _")
    corr_array+=("4 [1]: B")
    corr_array+=("5 [1]: B*")

    local score
    compare_lines line_array[@] corr_array[@] score
    log "${score}"
}

## Result validation: two tasks, overlap
TEST_CASES+=("task_2_overlap")
task_2_overlap() {
    log "--- Running test case: ${FUNCNAME} ---"
	rm -f *.dat
    echo -e "A 1 4\nB 2 3" > task.dat
    run_test "task.dat"

    local line_array=()
    line_array+=("$(select_line "${STDOUT}" "1")")
    line_array+=("$(select_line "${STDOUT}" "2")")
    line_array+=("$(select_line "${STDOUT}" "3")")
    line_array+=("$(select_line "${STDOUT}" "4")")
    line_array+=("$(select_line "${STDOUT}" "8")")
    local corr_array=()
    corr_array+=("0 [0]: _")
    corr_array+=("1 [1]: A")
    corr_array+=("2 [2]: B")
    corr_array+=("3 [2]: B")
    corr_array+=("7 [1]: A*")

    local score
    compare_lines line_array[@] corr_array[@] score
    log "${score}"
}

## Result validation: three tasks, overlap (same as assignment example)
TEST_CASES+=("task_3_example")
task_3_example() {
    log "--- Running test case: ${FUNCNAME} ---"
	rm -f *.dat
    echo -e "A 1 3\nB 2 4\nC 2 3" > task.dat
    run_test "task.dat"

    local line_array=()
    line_array+=("$(select_line "${STDOUT}" "1")")
    line_array+=("$(select_line "${STDOUT}" "2")")
    line_array+=("$(select_line "${STDOUT}" "3")")
    line_array+=("$(select_line "${STDOUT}" "4")")
    line_array+=("$(select_line "${STDOUT}" "8")")
    line_array+=("$(select_line "${STDOUT}" "9")")
    line_array+=("$(select_line "${STDOUT}" "10")")
    line_array+=("$(select_line "${STDOUT}" "11")")
    local corr_array=()
    corr_array+=("0 [0]: _")
    corr_array+=("1 [1]: A")
    corr_array+=("2 [3]: B")
    corr_array+=("3 [3]: C")
    corr_array+=("7 [3]: B")
    corr_array+=("8 [3]: C*")
    corr_array+=("9 [2]: A*")
    corr_array+=("10 [1]: B*")

    local score
    compare_lines line_array[@] corr_array[@] score
    log "${score}"
}

## Result validation: three tasks, non-sorted, overlap
TEST_CASES+=("task_3_chaos")
task_3_chaos() {
    log "--- Running test case: ${FUNCNAME} ---"
	rm -f *.dat
    echo -e "N 0 5\nB 0 6\nV 3 3" > task.dat
    run_test "task.dat"

    local line_array=()
    line_array+=("$(select_line "${STDOUT}" "1")")
    line_array+=("$(select_line "${STDOUT}" "2")")
    line_array+=("$(select_line "${STDOUT}" "3")")
    line_array+=("$(select_line "${STDOUT}" "5")")
    line_array+=("$(select_line "${STDOUT}" "7")")
    line_array+=("$(select_line "${STDOUT}" "9")")
    line_array+=("$(select_line "${STDOUT}" "11")")
    line_array+=("$(select_line "${STDOUT}" "13")")
    local corr_array=()
    corr_array+=("0 [2]: B")
    corr_array+=("1 [2]: N")
    corr_array+=("2 [2]: N")
    corr_array+=("4 [3]: V")
    corr_array+=("6 [3]: N")
    corr_array+=("8 [3]: B")
    corr_array+=("10 [2]: N")
    corr_array+=("12 [1]: B")

    local score
    compare_lines line_array[@] corr_array[@] score
    log "${score}"
}


#
# Main functions
#
parse_argvs() {
    local OPTIND opt

    while getopts "h?s:t:" opt; do
        case "$opt" in
        h|\?)
            echo "${SCRIPT_NAME}: [-s <exec_path>] [-t <test_case>]" 1>&2
            exit 0
            ;;
        s)  EXEC="$(readlink -f ${OPTARG})"
            ;;
        t)  TEST_CASE="${OPTARG}"
            ;;
        esac
    done
}

check_vals() {
    # Check exec path
    [[ -x ${EXEC} ]] || \
        die "Cannot find executable '${EXEC}'"

    # Check test case
    [[ " ${TEST_CASES[@]} all " =~ " ${TEST_CASE} " ]] || \
        die "Cannot find test case '${TEST_CASE}'"
}

grade() {
    # Make testing directory
    local tmpdir=$(mktemp -d --tmpdir=.)
    cd ${tmpdir}

    # Run test case(s)
    if [[ "${TEST_CASE}" == "all" ]]; then
        # Run all test cases
    for t in "${TEST_CASES[@]}"; do
        ${t}
            log "\n"
    done
    else
        # Run specific test case
        ${TEST_CASE}
    fi

    # Cleanup testing directory
    cd ..
    rm -rf "${tmpdir}"
}

parse_argvs "$@"
check_vals
grade
