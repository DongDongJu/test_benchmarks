#!/bin/bash
#$ -N minimp3_trace_gen_analyzer
#$ -q drg,drgPQ,pub64
#$ -pe openmp 2
#$ -ckpt blcr
#$ -l kernel=blcr
#$ -r y

source $HOME/.bashrc
source $HOME/.bash_profile

arch="x86"
ARCH=$(echo $arch | tr 'a-z' 'A-Z')
GEM5_BUILD=gem5.opt

BENCHMARK=minimp3
BINARY=minimp3

TRACES_DIR=/pub/dongjos2/majid/test_run

TIMESTAMP=$(date +%Y%m%dT%H%M%S)
OUT_DIR=$TRACES_DIR/$BENCHMARK/$TIMESTAMP
echo $OUT_DIR
DATA_DIR=$(pwd)

GEM5_TRACE_GENERATOR_DIR=/data/users/dongjos2/x862/gem5
MEM_TRACE_ANALYZER_DIR=/pub/dongjos2/majid/mem-trace-analyzer
RUN_SMALL="Y"


echo "Running minimp3: gem5"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"
if [ $RUN_SMALL=="Y" ]; then
$GEM5_TRACE_GENERATOR_DIR/build/$ARCH/$GEM5_BUILD \
--redirect-stdout \
--redirect-stderr \
--outdir=$OUT_DIR/small \
$GEM5_TRACE_GENERATOR_DIR/configs/example/se.py \
--cpu-type=detailed \
--caches \
--cmd=$TRACES_DIR/$BENCHMARK/$BINARY \
--option=" /pub/dongjos2/majid/test_run/minimp3/1.mp3" &
fi
wait

module load python/2.7.10

echo "Running pcompress2: trace_preprocessor"
if [ $RUN_SMALL == "Y" ]; then
pypy $MEM_TRACE_ANALYZER_DIR/trace_preprocessor.py $OUT_DIR/small/simout &
fi
wait

echo "Running pcompress2: trace_analyzer"

PAGE_SIZE=32
EPOCH_DURATION=1000000
if [ $RUN_SMALL == "Y" ]; then
pypy $MEM_TRACE_ANALYZER_DIR/trace_analyzer.py -i $OUT_DIR/small/simout_ -o $OUT_DIR/small/report.txt -p $PAGE_SIZE -e $EPOCH_DURATION &
fi
wait

echo "Done!"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"
