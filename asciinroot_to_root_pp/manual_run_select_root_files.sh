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
root_out_dir="/afs/cern.ch/user/m/msoldani/data_out"
if [ ! -d $root_out_dir ];then
    echo "ERROR: directory "$root_out_dir" not present."
    exit 1
fi

# run list (set)
# ---------------
# to manually select runs:
# jarray=(  # set run numbers here
# 500187 500191 500202
# )
# alternatively, to process all runs, you can try: 
# RUNSTRL=4
# RUNSTRR=9
# jarray=($(ls $DATAPATH | grep 000001 | cut -c$RUNSTRL-$RUNSTRR))
# ---------------
jarray=(  # set run numbers here
700380
700381
700382
700383
700384
700385
700387
700386
700387
700388
700389
)

# loop on all the requested runs
for j in "${jarray[@]}" ; do

    # delete previously created files
    echo "1st of all, deleting already existing files..."
    rm $root_out_dir/merged/*$j*
    rm $root_out_dir/splitted/*$j*

    # work on the run - core operations are performed in here
    ./update_single_run.sh $j

    echo "- - - - -"
    echo " - - - - "
    echo "- - - - -"
    echo " - - - - "
    echo "- - - - -"
done
