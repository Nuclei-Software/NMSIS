#!/bin/env bash
REBUILD=${REBUILD:-1}

echo "Generate DSP/NN Library"
REBUILD=$REBUILD make gen
echo "Generate NN Reference Library"
REBUILD=$REBUILD make gen_nnref_lib
