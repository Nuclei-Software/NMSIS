#!/bin/env bash
REBUILD=${REBUILD:-1}
NUCLEI_DSP=${NUCLEI_DSP:-N3}

echo "Generate DSP/NN Library"
REBUILD=$REBUILD NUCLEI_DSP=$NUCLEI_DSP make gen
echo "Generate NN Reference Library"
REBUILD=$REBUILD make gen_nnref_lib
