.. _nn_overview:

Overview
========

Introduction
------------

This user manual describes the NMSIS NN software library,
a collection of efficient neural network kernels developed to maximize the
performance and minimize the memory footprint of neural networks on |nuclei_core| cores.

The library is divided into a number of functions each covering a specific category:

- Neural Network Convolution Functions
- Neural Network Activation Functions
- Fully-connected Layer Functions
- Neural Network Pooling Functions
- Softmax Functions
- Neural Network Support Functions

The library has separate functions for operating on different weight and activation data
types including 8-bit integers (q7_t) and 16-bit integers (q15_t). The descrition of the
kernels are included in the function description.

The implementation details are also
described in this paper `CMSIS-NN: Efficient Neural Network Kernels for Arm Cortex-M CPUs`_ .



Block Diagram
-------------

.. image:: /asserts/images/nn/NMSIS-NN-OVERVIEW.PNG
    :alt: NMSIS NN Block Diagram
    :scale: 80%


Examples
--------

The library ships with a number of examples which demonstrate how to use the library functions.

* :ref:`Convolutional Neural Network Example<nmsis_nn_api_convolutional_neural_network_example>`
* :ref:`Gated Recurrent Unit Example<nmsis_nn_api_gated_recurrent_unit_example>`


Pre-processor Macros
--------------------

Each library project have differant pre-processor macros.

This library is only built for little endian targets.

RISCV_MATH_DSP:
  Define macro RISCV_MATH_DSP, If the silicon supports DSP instructions.

RISCV_NN_TRUNCATE:
  Define macro RISCV_NN_TRUNCATE to use floor instead of round-to-the-nearest-int for the computation.


.. _CMSIS-NN\: Efficient Neural Network Kernels for Arm Cortex-M CPUs: https://arxiv.org/abs/1801.06601
