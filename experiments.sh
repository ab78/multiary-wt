#!/bin/bash
set -e
RUN=./wt_test
DATA_DIR=data
OUTPUT_DIR=results
SWIT_EXT=.swit.bwt
TEXT_EXT=MB.bwt
RUNS=3
MAX_ARITY=8 # 16
STRUCTS=('ab-rrr' 'fc-rrr' 'simple' 'multi-01rrr')

for t in {0..3}
do
    struct=${STRUCTS[$t]}

    for (( arity=2; arity<=MAX_ARITY; arity*=2 ))
    do
        mkdir -p $OUTPUT_DIR/$struct/arity$arity/
        
        # Ascii files
        for file in $DATA_DIR/*$TEXT_EXT
        do
            OUT_FILE=`echo $file | sed 's/data\///'`
            OUT_FILE=$OUTPUT_DIR/$struct/arity$arity/$OUT_FILE
            
            for (( i=1; i<=RUNS; i++ ))
            do
                echo $struct - $file - RUN NUMBER $i...
                touch $OUT_FILE.$i
                $RUN -f $file -a $arity >> $OUT_FILE.$i
            done
        done

        # Integer Files
        for file in $DATA_DIR/*$SWIT_EXT
        do
            OUT_FILE=`echo $file | sed 's/data\///'`
            OUT_FILE=$OUTPUT_DIR/$struct/arity$arity/$OUT_FILE
            for (( i=1; i<=RUNS; i++ ))
            do
                touch $OUT_FILE.$i
                $RUN -f $file -a $arity -i >> $OUT_FILE.$i
            done
        done
        
        # Don't alter arity for Claude's
        if [ $t = 1 ]; then
            break
        fi
    done

done