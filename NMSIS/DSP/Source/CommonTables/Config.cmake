cmake_minimum_required (VERSION 3.14)



target_sources(NMSISDSP PRIVATE CommonTables/riscv_common_tables.c
    CommonTables/riscv_common_tables_f16.c)


target_sources(NMSISDSP PRIVATE CommonTables/riscv_const_structs.c)
target_sources(NMSISDSP PRIVATE CommonTables/riscv_const_structs_f16.c)
