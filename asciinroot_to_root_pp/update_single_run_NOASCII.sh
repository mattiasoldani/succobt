#!/bin/bash

# # # # # # # # # #
# Original tool by Valerio (https://baltig.infn.it/insulab/tbtools)
# Edited by Mattia and Gemma initially for the NanoCal 2023 beamtest @ T9
# # # # # # # # # #
# For every spill of a given single run, create output single-spill and merged ROOT files if they don't exist
# # # # # # # # # #
# Only 1 input argument, i.e. the run number
# # # # # # # # # #

# I/O paths (set)
ascii_dir="/afs/cern.ch/user/m/msoldani/data_in/ASCII_MICHELA/ascii_daq_sshfs"
root_in_dir="/afs/cern.ch/user/m/msoldani/data_in/H2ROOT"
root_out_dir="/afs/cern.ch/user/m/msoldani/data_out/splitted_prel"
root_merged_dir="/afs/cern.ch/user/m/msoldani/data_out/merged_prel"
if [ ! -d $ascii_dir ];then
    echo "ERROR: directory "$ascii_dir" not present."
    exit 1
fi
if [ ! -d $root_in_dir ];then
    echo "ERROR: directory "$root_in_dir" not present."
    exit 1
fi
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

# final (merged) ROOT file name
root_merged_file=$(awk -v run=$nrun 'BEGIN{printf("run%6.6d.root",run)}')

# loop on spills through input ASCII files
new_file_count=0;
for file in $(ls $ascii_dir"/run"$nrun"_"*.dat); do

    # input ROOT file name (set)
    file2_0="${file/$ascii_dir/$root_in_dir}"  
    file2_1="${file2_0/_00/_0}"  # needed to correct for the spill nr. size mismatch between input ASCII and ROOT
    file2="${file2_1/.dat/.root}"  
    
    # if all input files (both ASCII and ROOT) are available
    if [[ -e $file && -e $file2 ]] ; then
    
        # output new-spill ROOT file
        nspill=$(echo $file | awk '{c=split($1,a,"/");split(a[c],b,".");dd=split(b[1],d,"_");print d[dd]+0}')
        root_file=$(awk -v run=$nrun -v spill=$nspill 'BEGIN{printf("run%6.6d_%6.6d.root",run,spill)}')
        
        # check output new-spill file existence - it will only be created if it doesn't exist already
        if [ ! -e $root_out_dir/$root_file ];then
            new_file_count=$(echo $new_file_count + 1 | bc)
             
            # run output ROOT file creation
            root -l -b -q 'asciinroot_to_root_pp_NOASCII.cc+('\"$file\"','\"$root_out_dir/$root_file\"','\"$file2\"')'

            # append each new-spill file to the final (merged) one
            # (un)comment the line below (and that only) to disable (enable) generation of merged files
            #hadd -a -k $root_merged_dir/$root_merged_file $root_out_dir/$root_file
        fi
        
    fi
    
done

# success flag
if [ $new_file_count -gt 0 ];then
    echo $new_file_count" files appended to "$root_merged_dir/$root_merged_file 
fi
