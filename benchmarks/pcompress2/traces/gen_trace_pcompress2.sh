#!/bin/bash
#$ -N pcompress2_trace_gen_analyzer
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

BENCHMARK=pcompress2
BINARY=bin/$arch/pcompress2

TIMESTAMP=$(date +%Y%m%dT%H%M%S)
OUT_DIR=$TRACES_DIR/$BENCHMARK/$TIMESTAMP
echo $OUT_DIR
DATA_DIR=$(pwd)

RUN_COMP="Y"
RUN_DECOMP="Y"
RUN_BOTH="Y"

echo "Running pcompress2: gem5"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"
if [ $RUN_BOTH=="Y" ]; then
$GEM5_TRACE_GENERATOR_DIR/build/$ARCH/$GEM5_BUILD \
--redirect-stdout \
--redirect-stderr \
--outdir=$OUT_DIR/both \
$GEM5_TRACE_GENERATOR_DIR/configs/example/se.py \
--cpu-type=detailed \
--caches \
--l1d_size='1024kB' \
--cmd=$SPM_BENCHMARKS_DIR/$BENCHMARK/$BINARY \
--option="-L 1" &
fi

if [ $RUN_COMP=="Y" ]; then
$GEM5_TRACE_GENERATOR_DIR/build/$ARCH/$GEM5_BUILD \
--redirect-stdout \
--redirect-stderr \
--outdir=$OUT_DIR/comp \
$GEM5_TRACE_GENERATOR_DIR/configs/example/se.py \
--cpu-type=detailed \
--caches \
--l1d_size='1024kB' \
--cmd=$SPM_BENCHMARKS_DIR/$BENCHMARK/$BINARY \
--option="-c $DATA_DIR/large.in" &
fi
if [ $RUN_DECOMP=="Y" ]; then
$GEM5_TRACE_GENERATOR_DIR/build/$ARCH/$GEM5_BUILD \
--redirect-stdout \
--redirect-stderr \
--outdir=$OUT_DIR/decomp \
$GEM5_TRACE_GENERATOR_DIR/configs/example/se.py \
--cpu-type=detailed \
--caches \
--l1d_size='1024kB' \
--cmd=$SPM_BENCHMARKS_DIR/$BENCHMARK/$BINARY \
--option="-d $DATA_DIR/large.in.compr" &
fi
wait

module load python/2.7.10

echo "Running pcompress2: trace_preprocessor"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"
if [ $RUN_BOTH == "Y" ]; then
pypy $MEM_TRACE_ANALYZER_DIR/trace_preprocessor.py $OUT_DIR/both/simout &
fi
if [ $RUN_COMP == "Y" ]; then
pypy $MEM_TRACE_ANALYZER_DIR/trace_preprocessor.py $OUT_DIR/comp/simout &
fi
if [ $RUN_DECOMP == "Y" ]; then
pypy $MEM_TRACE_ANALYZER_DIR/trace_preprocessor.py $OUT_DIR/decomp/simout &
fi
wait

echo "Running pcompress2: trace_analyzer"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"

PAGE_SIZE=32
EPOCH_DURATION=1000000
if [ $RUN_BOTH == "Y" ]; then
pypy $MEM_TRACE_ANALYZER_DIR/trace_analyzer.py -i $OUT_DIR/both/simout_ -o $OUT_DIR/both/report.txt -p $PAGE_SIZE -e $EPOCH_DURATION &
fi

if [ $RUN_COMP == "Y" ]; then
pypy $MEM_TRACE_ANALYZER_DIR/trace_analyzer.py -i $OUT_DIR/comp/simout_ -o $OUT_DIR/comp/report.txt -p $PAGE_SIZE -e $EPOCH_DURATION &
fi
if [ $RUN_DECOMP == "Y" ]; then
pypy $MEM_TRACE_ANALYZER_DIR/trace_analyzer.py -i $OUT_DIR/decomp/simout_ -o $OUT_DIR/decomp/report.txt -p $PAGE_SIZE -e $EPOCH_DURATION &
fi
wait

echo "Done!"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"

