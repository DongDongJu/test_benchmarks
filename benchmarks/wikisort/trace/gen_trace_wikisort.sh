#!/bin/bash
#$ -N wikisort_trace_gen_analyzer
#$ -q drg,drgPQ,pub64
#$ -pe openmp 3
#$ -ckpt blcr
#$ -l kernel=blcr
#$ -r y

source $HOME/.bashrc
source $HOME/.bash_profile

arch="ARM"
ARCH=$(echo $arch | tr 'a-z' 'A-Z')
GEM5_BUILD=gem5.fast

BENCHMARK=wikisort
BINARY=bin/$arch/wikisort

TIMESTAMP=$(date +%Y%m%dT%H%M%S)
OUT_DIR=$TRACES_DIR/$BENCHMARK/$TIMESTAMP
echo $OUT_DIR


RUN="Y"

echo "Running wikisort: gem5"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"


if [ $RUN== "Y" ]; then
$GEM5_PATH/build/ARM/gem5.opt \
--redirect-stdout \
--redirect-stderr \
--outdir=$CUR_DIR/trace_large \
$GEM5_PATH/configs/example/se.py \
--cpu-type=arm_detailed \
--caches \
--cmd=/pub/dongjos2/majid/git/test_benchmarks/benchmarks/wikisort/wikisort &
fi
wait

module load python/2.7.10

echo "Running patricia: trace_preprocessor"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"
if [ $RUN == "Y" ]; then
$MEM_TRACE_ANALYZER_PATH/trace_preprocessor.py  $CUR_DIR/trace_large/simout &
fi
wait
 
echo "Running patricia: trace_analyzer"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"

PAGE_SIZE=8
EPOCH_DURATION=1000000

if [ $RUN== "Y" ]; then
$MEM_TRACE_ANALYZER_PATH/trace_analyzer.py -i $CUR_DIR/trace_large/simout_ -o $CUR_DIR/trace_large/report.txt -p $PAGE_SIZE -e $EPOCH_DURATION &
fi
wait

echo "Done!"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"

