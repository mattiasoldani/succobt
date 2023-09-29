#!/bin/bash

# # # # # # # # # #
# Original tool by Valerio (https://baltig.infn.it/insulab/tbtools)
# Script created by Mattia initially for the NanoCal 2023 beamtest @ T9
# # # # # # # # # #
# Remove all the previously generated output files for a specific run
# # # # # # # # # #
# Only 1 input argument, i.e. the run number
# # # # # # # # # #

# I/O paths (set)
root_out_dir="/afs/cern.ch/user/m/msoldani/data_out/splitted_prel"
root_merged_dir="/afs/cern.ch/user/m/msoldani/data_out/merged_prel"
if [ ! -d $root_out_dir ];then
    echo "ERROR: directory "$root_out_dir" not present."
    exit 1
fi
if [ ! -d $root_merged_dir ];then
    echo "ERROR: directory "$root_merged_dir" not present."
    exit 1
fi

# input arguments - 1, i.e. the run number
if [ $# -ne 1 ];then
    echo "ERROR: use 1 argument (run number)"
    exit 1
fi
nrun=$( printf "%6.6d" "$1")

rm -rv $root_out_dir/*$nrun*
rm -rv $root_merged_dir/*$nrun*
