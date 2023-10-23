# IAR Embedded Workbench IDE Projects For Nuclei Processors

In this folder, we provided a prebuilt IAR workspace with NMSIS DSP/NMSIS NN libraries, and NN Reference library for testing NN.

> You can refer to this demo projects to build your own iar projects.

These projects demonstrated the following features:

- Provide support for Nuclei Evaluation SoC which is a prototype and evaluation SoC mainly
  used to evaluate Nuclei RISC-V processor such as 200/300/600/900 series.
- By default, RISC-V Extension IMAFDC are selected in this project, and the optimization level is "High Speed".
- If you want to **enable P extension**, choose "Xandesdsp" option following the path: the Options->General Options-> ISA Extensions->DSP;.
  > And don't choose "P", which generates no __riscv_dsp macro

  > NN Reference library must build with **None DSP**
- Currently both Nuclei RV32 and RV64 processors are supported in this project.
- When building done, the generated library with **.a** suffix is placed in "NMSIS\ideprojects\iar\Debug\Exe"


You can directly try with this iar workspace by click this nmsis.eww after you have installed [IAR Workbench](https://www.iar.com/riscv).

![IAR Projects for Nuclei](assests/nmsis_iar_projects.png)


For more details about how to use IAR workbench, please refer to its user guides.
