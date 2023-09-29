#!/bin/bash

# # # # # # # # # #
# Script created by Mattia initially for the 2021 beamtests @ CERN H2
# # # # # # # # # #
# Manually reprocess desired runs from scratch - attention, all previously created files are deleted!
# # # # # # # # # #
# No input arguments, just settings in here
# # # # # # # # # #

DATAPATH=$HOME/data_in/ASCII_MICHELA/ascii_daq_sshfs/
OUTPATH=$HOME/data_out/ascii_merged/
if [ ! -d $DATAPATH ];then
    echo "ERROR: directory "$DATAPATH" not present."
    exit 1
fi
if [ ! -d $OUTPATH ];then
    echo "ERROR: directory "$OUTPATH" not present."
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

k=0 
for j in "${jarray[@]}" ; do
	run="run$j""_"
	output="$OUTPATH$run""000001.dat"
	rm -f "$output" ; touch "$output"
	for i in $(seq -f "%06g" 1 2000) ; do 
		input="$DATAPATH$run$i.dat"  # path from ~
		if [ -f "$input" ]; then  # if input file exists...
			if [ -s $input ] ; then  # if input file is not empty...
				echo "adding run $j - file $i"
				# set rule for concatenation here:
                # ---------------
                # awk 'NR % 1 == 1' $input >> $output  # set nr. of rows (successive to 1st one) to strip  -- e.g. for 3 rows: awk 'NR % 4 == 1' $input >> $output
				# alternatively, if no lines have to be stripped, use: cat $input >> $output
                # ---------------
				cat $input >> $output
				k=$(expr $k + 1)
			fi
			
		else
			echo "adding run $j - files over"
		fi
	done
done

if [ $k -eq 0 ] ; then
	echo "no stripped files added this time"
else
	echo "$k iterations done this time"
fi

echo "--"
