#!/bin/bash

GEM5_PATH=/data/users/dongjos2/gem5
MEM_TRACE_ANALYZER_PATH=/pub/dongjos2/majid/mem-trace-analyzer

CUR_DIR=$(pwd)
echo $CUR_DIR

RUN_LARGE="N"
RUN_SMALL="Y"

echo "Running patricia: gem5"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"


if [ $RUN_LARGE == "Y" ]; then
$GEM5_PATH/build/ARM/gem5.opt \
--redirect-stdout \
--redirect-stderr \
--outdir=$CUR_DIR/trace_large \
$GEM5_PATH/configs/example/se.py \
--cpu-type=detailed \
--caches \
--cmd=/pub/dongjos2/majid/test_benchmarks/benchmarks/patricia/patricia \
--options="/pub/dongjos2/majid/test_benchmarks/benchmarks/patricia/large.udp > /pub/dongjos2/majid/test_benchmarks/benchmarks/patricia/output_large.txt"&
fi

if [ $RUN_SMALL == "Y" ]; then
$GEM5_PATH/build/ARM/gem5.opt \
--redirect-stdout \
--redirect-stderr \
--outdir=$CUR_DIR/trace_small \
$GEM5_PATH/configs/example/se.py \
--caches \
--cpu-type=detailed \
--cmd=/pub/dongjos2/majid/test_benchmarks/benchmarks/patricia/patricia \
--options="/pub/dongjos2/majid/test_benchmarks/benchmarks/patricia/small.udp > /pub/dongjos2/majid/test_benchmarks/benchmarks/patricia/output_small.txt"&
fi
wait



echo "Running patricia: trace_preprocessor"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"
if [ $RUN_LARGE == "Y" ]; then
$MEM_TRACE_ANALYZER_PATH/trace_preprocessor.py  $CUR_DIR/trace_large/simout &
fi
if [ $RUN_SMALL == "Y" ]; then
$MEM_TRACE_ANALYZER_PATH/trace_preprocessor.py  $CUR_DIR/trace_small/simout &
fi

echo "Running patricia: trace_analyzer"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"

PAGE_SIZE=256
EPOCH_DURATION=1000000

if [ $RUN_LARGE == "Y" ]; then
$MEM_TRACE_ANALYZER_PATH/trace_analyzer.py -i $CUR_DIR/trace_large/simout_ -o $CUR_DIR/trace_large/report.txt -p $PAGE_SIZE -e $EPOCH_DURATION &
fi

if [ $RUN_SMALL == "Y" ]; then
$MEM_TRACE_ANALYZER_PATH/trace_analyzer.py -i $CUR_DIR/trace_small/simout_ -o $CUR_DIR/trace_small/report.txt -p $PAGE_SIZE -e $EPOCH_DURATION &
fi
echo "Done!"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"

