Copy IAR built library from NMSIS\ideprojects\iar\library\Debug\Exe to this folder.

eg. 

nmsis_dsp.a -> libnmsis_dsp_rv32imafdc.a
nmsis_nn.a -> libnmsis_nn_rv32imafdc.a


rv32imafdc is the library built risc-v arch in IAR embedded workbench settings,
if you are using a different library of risc-v arch, please modify the example and library project settings
using correct arch settings as you wish, and modify project settings to select correct prebuilt IAR compiled
dsp and nn library