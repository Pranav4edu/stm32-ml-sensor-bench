#ifndef ML_BENCH_H
#define ML_BENCH_H

#include <stdint.h>

typedef struct
{
  int32_t score;
  uint32_t iterations;
  uint32_t total_cycles;
  uint32_t cycles_per_run;
} ml_bench_result_t;

ml_bench_result_t ml_bench_run_reference(void);

#endif /* ML_BENCH_H */
