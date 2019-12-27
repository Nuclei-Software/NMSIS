.. _core_register_mapping:

Register Mapping
================

The table below associates some common register names used in NMSIS to the register names used in Technical Reference Manuals.

+-------------------------+--------------------------------+-------------------------------------------------+
| **NMSIS Register Name** | **N200, N300, N600, NX600**    | **Register Description**                        |
+-------------------------+--------------------------------+-------------------------------------------------+
|                         **Enhanced Core Local Interrupt Controller(ECLIC)**                                |
+-------------------------+--------------------------------+-------------------------------------------------+
| ECLIC->CFG              | cliccfg                        | ECLIC Global Configuration Register             |
+-------------------------+--------------------------------+-------------------------------------------------+
| ECLIC->INFO             | clicinfo                       | ECLIC Global Information Register               |
+-------------------------+--------------------------------+-------------------------------------------------+
| ECLIC->MTH              | mth                            | ECLIC Global Machine Mode Threshold Register    |
+-------------------------+--------------------------------+-------------------------------------------------+
| ECLIC->CTRL[i].INTIP    | clicintip[i]                   | ECLIC Interrupt Pending Register                |
+-------------------------+--------------------------------+-------------------------------------------------+
| ECLIC->CTRL[i].INTIE    | clicintie[i]                   | ECLIC Interrupt Enable Register                 |
+-------------------------+--------------------------------+-------------------------------------------------+
| ECLIC->CTRL[i].INTATTR  | clicintattr[i]                 | ECLIC Interrupt Attribute Register              |
+-------------------------+--------------------------------+-------------------------------------------------+
| ECLIC->CTRL[i].INTCTRL  | clicintctl[i]                  | ECLIC Interrupt Input Control Register          |
+-------------------------+--------------------------------+-------------------------------------------------+
|                                **System Timer Unit(SysTimer)**                                             |
+-------------------------+--------------------------------+-------------------------------------------------+
| SysTimer->MTIMER        | mtime_hi<<32 + mtime_lo        | System Timer current value 64bits Register      |
+-------------------------+--------------------------------+-------------------------------------------------+
| SysTimer->MTIMERCMP     | mtimecmp_hi<<32 + mtimecmp_lo  | System Timer compare value 64bits Register      |
+-------------------------+--------------------------------+-------------------------------------------------+
| SysTimer->MSTOP         | mstop                          | System Timer Stop Register                      |
+-------------------------+--------------------------------+-------------------------------------------------+
| SysTimer->MSIP          | msip                           | System Timer SW interrupt Register              |
+-------------------------+--------------------------------+-------------------------------------------------+
