#!/bin/bash
#$ -N wikisort_gem5_spm_run
#$ -q drg,drgPQ,pub64
#$ -pe openmp 2
#$ -ckpt blcr
#$ -l kernel=blcr
#$ -r y

source $HOME/.bashrc
source $HOME/.bash_profile

arch="ARM"
ARCH=$(echo $arch | tr 'a-z' 'A-Z')
GEM5_BUILD=gem5.opt

BENCHMARK=wikisort
BINARY=bin/$arch/wikisort

DUMMY=$SPM_BENCHMARKS_DIR/dummy/bin/$arch/dummy_spm

TIMESTAMP=$(date +%Y%m%dT%H%M%S)
OUT_DIR=$TRACES_DIR/$BENCHMARK/GEM5_SPM_run/$TIMESTAMP
echo $OUT_DIR

RUN="Y"

echo "Running $BENCHMARK: gem5"
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"

if [ $RUN == "Y" ]; then
$SPM_GEM5_DIR/build/$ARCH/$GEM5_BUILD \
--redirect-stdout \
--redirect-stderr \
--outdir=$OUT_DIR/smoothing \
--debug-flags=GOV \
--debug-file=simdebug \
$SPM_GEM5_DIR/configs/spm_based_mesh/mesh_spm_se.py \
--cpu-type=detailed \
--cmd="$SPM_BENCHMARKS_DIR/$BENCHMARK/$BINARY\
;$DUMMY\
;$DUMMY\
;$DUMMY\
;$DUMMY\
;$DUMMY\
;$DUMMY\
;$DUMMY\
;$DUMMY" \
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