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


RUN_COMP="Y"
RUN_DECOMP="Y"

echo "Running pcompress2: gem5"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"

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
--option="-c /pub/dongjos2/majid/git/test_benchmarks/benchmarks/pcompress2/test_file.in" &
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
--option="-d /pub/dongjos2/majid/git/test_benchmarks/benchmarks/pcompress2/test_file.in.compr" &
fi
wait

module load python/2.7.10

echo "Running pcompress2: trace_preprocessor"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"
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

PAGE_SIZE=4
EPOCH_DURATION=1000000

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

