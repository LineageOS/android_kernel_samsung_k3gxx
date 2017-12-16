#!/bin/bash

LOG_DIR=~/build/merge_log

echo -e "\n"
read -p "Enter Version to merge: " version

merge()
{
    git merge v${version}
    git status
}

echo "" >> ${LOG_DIR}/merge-${version}.log && merge | tee ${LOG_DIR}/merge-${version}.log
