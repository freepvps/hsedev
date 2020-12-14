#!/bin/bash

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
PURPLE='\033[0;35m'
NOCOLOR='\033[0m'

EXECUTION_DIR="$(pwd)"
REPO_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

function log_debug {
    echo -e ${PURPLE}$@${NOCOLOR}
}

function log_error {
    echo -e ${RED}$@${NOCOLOR}
}

function log_info {
    echo -e ${GREEN}$@${NOCOLOR}
}

function call {
    log_debug $@
    $@
    return $?
}

function ensure {
    $@
    if [ $? -ne 0 ]; then
        log_error "execution error on: $@"
        exit $?
    fi
}

function create {
    TARGET_DIR="$REPO_DIR/src/apps/$1"
    ensure mkdir -p $TARGET_DIR
    ensure cp $REPO_DIR/src/templates/app/* $TARGET_DIR
}

function run {
    TARGET_DIR="$REPO_DIR/src/apps/$1"
    ensure mkdir -p $TARGET_DIR/build
    cd $TARGET_DIR/build
    ensure cmake ..
    ensure make -j16
    ensure ./app
}

function print_help {
    echo "Usage: $0 command [args...]"
    echo "Commands:"
    echo "  create NAME"
    echo "  run NAME"
}

date
if [[ $# -eq 0 ]] ; then
    print_help
else
    $@
    log_info "complete execution command: $@"
fi