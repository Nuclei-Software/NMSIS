# Nuclei Microcontroller Software Interface Standard

[![Build Status](https://travis-ci.com/Nuclei-Software/NMSIS.svg?branch=master)](https://travis-ci.com/Nuclei-Software/NMSIS)

The **Nuclei Microcontroller Software Interface Standard(NMSIS)** is a vendor-independent hardware abstraction layer for micro-controllers that are based on Nuclei processors.

The **NMSIS** defines generic tool interfaces and enables consistent device support.
It provides simple software interfaces to the processor and the peripherals, simplifying software re-use, reducing the learning
curve for micro-controller developers, and reducing the time to market for new devices.

## NMSIS Componments

* **NMSIS CORE**

    All Nuclei N/NX Class Processors Standardized API for the Nuclei processor core and peripherals.

* **NMSIS DSP**

    All Nuclei N/NX Class Processors DSP library collection with a lot of functions for various data types:
    fixed-point (fractional q7, q15, q31) and single precision floating-point (32-bit).
    Implementations optimized for the Nuclei processor core which has RISC-V SIMD instruction set.

* **NMSIS NN**

    All Nuclei N/NX Class Processors Collection of efficient neural network kernels developed to maximize
    the performance and minimize the memory footprint Nuclei processor core.
 
## Directory Structure

| Directory                  | Content                                                   |
|:-------------------------- |:--------------------------------------------------------- |
| NMSIS/Core                 | NMSIS-Core related files                                  |
| NMSIS/DSP                  | NMSIS-DSP related files                                   |
| NMSIS/NN                   | NMSIS-NN related files                                    |
| NMSIS/doc                  | Source of the documentation                               |
| Device/_Template_Vendor    | Vendor device template files                              |
| Device/Nuclei              | Nuclei N and Nuclei NX Class based device examples        |

## Documentation

If you want to learn more about this project, please visit this [NMSIS documentation](https://doc.nucleisys.com/nmsis/).

## License

This NMSIS is modified based on open-source project CMSIS to match Nuclei requirements.
NMSIS is licensed under Apache-2.0.

## Contributions and Pull Requests

Contributions are accepted under Apache-2.0.
Only submit contributions where you have authored all of the code.

### Issues and Labels

Please feel free to raise an [issue on GitHub](https://github.com/Nuclei-Software/NMSIS/issues)
to report misbehavior (i.e. bugs) or start discussions about enhancements. This
is your best way to interact directly with the maintenance team and the community.
We encourage you to append implementation suggestions as this helps to decrease the
workload of the very limited maintenance team. 

We will be monitoring and responding to issues as best we can.
Please attempt to avoid filing duplicates of open or closed items when possible.
In the spirit of openness we will be tagging issues with the following:

- **CORE** – We consider this issue to be related with NMSIS-Core topic.

- **Template** – We consider this issue to be related with NMSIS-Core Device Templates topic.

- **DSP** – We consider this issue to be related with NMSIS-DSP topic.

- **NN** – We consider this issue to be related with NMSIS-NN topic.

- **bug** – We consider this issue to be a bug that will be investigated.

- **wontfix** - We appreciate this issue but decided not to change the current behavior.
	
- **enhancement** – Denotes something that will be implemented soon. 

- **future** - Denotes something not yet schedule for implementation.

- **out-of-scope** - We consider this issue loosely related to NMSIS. It might by implemented outside of NMSIS. Let us know about your work.
	
- **question** – We have further questions to this issue. Please review and provide feedback.

- **documentation** - This issue is a documentation flaw that will be improved in future.

- **review** - This issue is under review. Please be patient.
	
- **DONE** - We consider this issue as resolved - please review and close it. In case of no further activity this issues will be closed after a week.

- **duplicate** - This issue is already addressed elsewhere, see comment with provided references.

- **Important Information** - We provide essential informations regarding planned or resolved major enhancements.
