#!/bin/bash
#$ -N memspeed_trace_gen_analyzer
#$ -q drg,drgPQ,pub64
#$ -pe openmp 2
#$ -ckpt blcr
#$ -l kernel=blcr
#$ -r y

source $HOME/.bashrc
source $HOME/.bash_profile

arch="ARM"
ARCH=$(echo $arch | tr 'a-z' 'A-Z')
GEM5_BUILD=gem5.fast

BENCHMARK=memspeed
BINARY=bin/$arch/memspeed

TIMESTAMP=$(date +%Y%m%dT%H%M%S)
OUT_DIR=$TRACES_DIR/$BENCHMARK/$TIMESTAMP
echo $OUT_DIR
DATA_DIR=$(pwd)

RUN_SMALL="Y"
RUN_LARGE="Y"

echo "Running memspeed: gem5"
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
--l1d_size='1024kB' \
--cmd=$SPM_BENCHMARKS_DIR/$BENCHMARK/$BINARY \
--option="1" &
fi

if [ $RUN_LARGE=="Y" ]; then
$GEM5_TRACE_GENERATOR_DIR/build/$ARCH/$GEM5_BUILD \
--redirect-stdout \
--redirect-stderr \
--outdir=$OUT_DIR/large \
$GEM5_TRACE_GENERATOR_DIR/configs/example/se.py \
--cpu-type=detailed \
--caches \
--l1d_size='1024kB' \
--cmd=$SPM_BENCHMARKS_DIR/$BENCHMARK/$BINARY \
--option="1024" &
fi
wait

module load python/2.7.10

echo "Running memspeed: trace_preprocessor"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"
if [ $RUN_SMALL == "Y" ]; then
pypy $MEM_TRACE_ANALYZER_DIR/trace_preprocessor.py $OUT_DIR/small/simout &
fi
if [ $RUN_LARGE == "Y" ]; then
pypy $MEM_TRACE_ANALYZER_DIR/trace_preprocessor.py $OUT_DIR/large/simout &
fi
wait

echo "Running memspeed: trace_analyzer"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"

PAGE_SIZE=32
EPOCH_DURATION=1000000
if [ $RUN_SMALL == "Y" ]; then
pypy $MEM_TRACE_ANALYZER_DIR/trace_analyzer.py -i $OUT_DIR/small/simout_ -o $OUT_DIR/small/report.txt -p $PAGE_SIZE -e $EPOCH_DURATION &
fi

if [ $RUN_LARGE == "Y" ]; then
pypy $MEM_TRACE_ANALYZER_DIR/trace_analyzer.py -i $OUT_DIR/large/simout_ -o $OUT_DIR/large/report.txt -p $PAGE_SIZE -e $EPOCH_DURATION &
fi
wait

echo "Done!"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"

