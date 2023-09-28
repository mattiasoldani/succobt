#!/bin/bash

# # # # # # # # # #
# Original tool by Valerio (https://baltig.infn.it/insulab/tbtools)
# Edited by Mattia and Gemma initially for the NanoCal 2023 beamtest @ T9
# # # # # # # # # #
# Automatically check for new spills to cook - to be left running on an online machine
# # # # # # # # # #
# No input arguments, just settings in here
# # # # # # # # # #

# I/O paths (set)
root_in_dir="/afs/cern.ch/user/m/msoldani/data_in/H2ROOT"  # in this case, new input ROOT files are sought
if [ ! -d $root_in_dir ];then
    echo "ERROR: directory "$root_in_dir" not present."
    exit 1
fi

# endless loop in which new data are looked for
sleepseconds=1
while [ 1 ] ; do
    last_run=$(ls -1t $root_in_dir/ | head -n 1 |  awk '{c=split($1,a,"/");split(a[c],b,".");split(b[1],d,"_");split(d[1],e,"run");printf("%d",e[2])}')
    echo "updating run "$last_run
    ./update_single_run_NOASCII.sh $last_run  # core operations are performed in here
    echo "sleeping "$sleepseconds" seconds"
    sleep $sleepseconds
done


    
