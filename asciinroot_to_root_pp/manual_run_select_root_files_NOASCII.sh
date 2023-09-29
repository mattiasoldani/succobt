#!/bin/bash

# # # # # # # # # #
# Original tool by Valerio (https://baltig.infn.it/insulab/tbtools)
# Script created by Mattia initially for the NanoCal 2023 beamtest @ T9
# # # # # # # # # #
# Manually reprocess desired runs from scratch - attention, all previously created files are deleted!
# # # # # # # # # #
# No input arguments, just settings in here
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

# run list (set)
# ---------------
# to manually select runs:
# jarray=(  # set run numbers here
# 100000 100001 100002
# )
# alternatively, to process all runs, you can try: 
# RUNSTRL=4
# RUNSTRR=9
# jarray=($(ls $DATAPATH | grep 000001 | cut -c$RUNSTRL-$RUNSTRR))
# ---------------
jarray=(  # set run numbers here
100000
)

# loop on all the requested runs
for j in "${jarray[@]}" ; do

    # delete previously created files
    echo "1st of all, deleting already existing files..."
    rm $root_out_dir/*$j*
    rm $root_merged_dir/*$j*

    # work on the run - core operations are performed in here
    ./update_single_run_NOASCII.sh $j

    echo "- - - - -"
    echo " - - - - "
    echo "- - - - -"
    echo " - - - - "
    echo "- - - - -"
done
