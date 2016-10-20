#!/bin/bash
#$ -N pcompress2_gem5_spm_run
#$ -q drg,drgPQ,pub64
#$ -pe openmp 2
#$ -ckpt blcr
#$ -l kernel=blcr
#$ -r y

source $HOME/.bashrc
source $HOME/.bash_profile

arch="arm"
ARCH=$(echo $arch | tr 'a-z' 'A-Z')
GEM5_BUILD=gem5.opt

BENCHMARK=pcompress2
BINARY=bin/$arch/pcompress2

DUMMY=$SPM_BENCHMARKS_DIR/dummy/bin/$arch/dummy_spm

TIMESTAMP=$(date +%Y%m%dT%H%M%S)
OUT_DIR=$TRACES_DIR/$BENCHMARK/GEM5_SPM_run/$TIMESTAMP
echo $OUT_DIR

COMP="Y"
DECOMP="Y"

echo "Running $BENCHMARK: gem5"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"

if [ $COMP == "Y" ]; then
$SPM_GEM5_DIR/build/$ARCH/$GEM5_BUILD \
--redirect-stdout \
--redirect-stderr \
--outdir=$OUT_DIR/comp \
--debug-flags=GOV \
--debug-file=simdebug \
$SPM_GEM5_DIR/configs/spm_based_mesh/mesh_spm_se.py \
--cpu-type=arm_detailed \
--cmd="$SPM_BENCHMARKS_DIR/$BENCHMARK/$BINARY\
;$DUMMY\
;$DUMMY\
;$DUMMY\
;$DUMMY\
;$DUMMY\
;$DUMMY\
;$DUMMY\
;$DUMMY" \
--options \
"-c /pub/dongjos2/majid/git/test_benchmarks/benchmarks/pcompress2/test_file.in\
;\
;\
;\
;\
;\
;\
;\
;" \
-n 9 \
--topology=Mesh \
--mesh-rows=3 \
--caches \
--spm-page-size=32 \
--l1dspm-size="4096kB" \
--gov-type="Local" \
--guest-slot-selection-policy="LeastRecentlyUsed" \
--guest-slot-relocation-policy="ClosestFreeSlot" &
fi

if [ $DECOMP == "Y" ]; then
$SPM_GEM5_DIR/build/$ARCH/$GEM5_BUILD \
--redirect-stdout \
--redirect-stderr \
--outdir=$OUT_DIR/decomp \
--debug-flags=GOV \
--debug-file=simdebug \
$SPM_GEM5_DIR/configs/spm_based_mesh/mesh_spm_se.py \
--cpu-type=arm_detailed \
--cmd="$SPM_BENCHMARKS_DIR/$BENCHMARK/$BINARY\
;$DUMMY\
;$DUMMY\
;$DUMMY\
;$DUMMY\
;$DUMMY\
;$DUMMY\
;$DUMMY\
;$DUMMY" \
--options \
"-d /pub/dongjos2/majid/git/test_benchmarks/benchmarks/pcompress2/test_file.in.compr\
;\
;\
;\
;\
;\
;\
;\
;" \
-n 9 \
--topology=Mesh \
--mesh-rows=3 \
--caches \
--spm-page-size=32 \
--l1dspm-size="4096kB" \
--gov-type="Local" \
--guest-slot-selection-policy="LeastRecentlyUsed" \
--guest-slot-relocation-policy="ClosestFreeSlot" &
fi
wait

echo "Done!"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"
