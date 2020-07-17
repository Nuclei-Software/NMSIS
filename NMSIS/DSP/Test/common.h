//
// Created by lujun on 7/13/19.
//

#ifndef DSP_COMMON_H
#define DSP_COMMON_H

#include <stdint.h>
#include <stdio.h>

#ifdef __riscv

#define read_csr(reg)                                                          \
    ({                                                                         \
        unsigned int __temp;                                                   \
        asm volatile("csrr %0, " #reg : "=r"(__temp));                         \
        __temp;                                                                \
    })

#if __riscv_xlen == 32
static inline uint64_t read_cpu_cycle(void)
{
    uint32_t hi = 0, hi1 = 0, lo = 0;
    uint64_t val = 0;
    hi = read_csr(cycleh);
    lo = read_csr(cycle);
    hi1 = read_csr(cycleh);
    if (hi != hi1) {
        hi = read_csr(cycleh);
        lo = read_csr(cycle);
    }
    val = (((uint64_t)hi) << 32) | lo;
    return val;
}
static inline uint64_t read_cpu_instret(void)
{
    uint32_t hi = 0, hi1 = 0, lo = 0;
    uint64_t val = 0;
    hi = read_csr(instreth);
    lo = read_csr(instret);
    hi1 = read_csr(instreth);
    if (hi != hi1) {
        hi = read_csr(instreth);
        lo = read_csr(instret);
    }
    val = (((uint64_t)hi) << 32) | lo;
    return val;
}
#else
static inline uint64_t read_cpu_cycle(void)
{
    uint64_t val = 0;
    val = read_csr(cycle);
    return val;
}
static inline uint64_t read_cpu_instret(void)
{
    uint64_t val = 0;
    val = read_csr(instret);
    return val;
}
#endif
static inline void reset_cpu_cycle(void) { ; }
#endif /*__riscv*/

#ifdef __arm__

#define DWT_CR *(uint32_t *)0xE0001000
#define DWT_CYCCNT *(uint32_t *)0xE0001004
#define DEM_CR *(uint32_t *)0xE000EDFC

#define DEM_CR_TRCENA (1 << 24)
#define DWT_CR_CYCCNTENA (1 << 0)

static inline uint32_t read_cpu_cycle(void) { return ((uint32_t)DWT_CYCCNT); }
static inline void reset_cpu_cycle(void)
{
    DEM_CR |= (uint32_t)DEM_CR_TRCENA;
    DWT_CYCCNT = (uint32_t)0u;
    DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
}
#endif

#ifndef READ_CYCLE
#define READ_CYCLE read_cpu_cycle
#endif

#ifndef RESET_CYCLE
#define RESET_CYCLE reset_cpu_cycle
#endif

uint64_t enter_cycle;
uint64_t exit_cycle;
uint64_t start_cycle;
uint64_t end_cycle;
uint64_t cycle;
uint64_t extra_cost = 0;
uint32_t bench_ercd;

#define BENCH_TRST RESET_CYCLE();

#define BENCH_INIT                                                             \
    printf("Benchmark Initialized\n");                                         \
    BENCH_TRST                                                                 \
    start_cycle = READ_CYCLE();                                                \
    end_cycle = READ_CYCLE();                                                  \
    extra_cost = end_cycle - start_cycle;                                      \
    enter_cycle = READ_CYCLE();

#define BENCH_START(func)                                                      \
    bench_ercd = 0;                                                            \
    BENCH_TRST                                                                 \
    start_cycle = READ_CYCLE();

#define BENCH_END(func)                                                        \
    end_cycle = READ_CYCLE();                                                  \
    cycle = end_cycle - start_cycle - extra_cost;                              \
    printf("CSV, %s, %lu\n", #func, cycle);

#define BENCH_ERROR(func) bench_ercd = 1;
#define BENCH_STATUS(func)                                                     \
    if (bench_ercd) {                                                          \
        printf("ERROR, %s\n", #func);                                          \
    } else {                                                                   \
        printf("SUCCESS, %s\n", #func);                                        \
    }

#define BENCH_FINISH                                                           \
    exit_cycle = READ_CYCLE();                                                 \
    cycle = exit_cycle - enter_cycle - extra_cost;                             \
    printf("CSV, BENCH END, %llu\n", cycle);

#endif // DSP_COMMON_H
