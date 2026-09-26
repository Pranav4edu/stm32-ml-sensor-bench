#include "ml_bench.h"

#include "main.h"

#define ML_BENCH_VECTOR_LENGTH 32U
#define ML_BENCH_ITERATIONS    1000U

/* A deterministic stand-in for one short, quantized sensor window. */
/*
 * Volatile is deliberate here.  The optimizer must treat this as sensor data
 * that can change outside the program, so it cannot pre-compute one dot
 * product and replace the 1,000 measured inferences with a multiplication.
 */
static volatile int8_t sensor_window[ML_BENCH_VECTOR_LENGTH];

/* Quantized weights for one output neuron. */
static const int8_t weights[ML_BENCH_VECTOR_LENGTH] = {
   12,  -8,  31, -15,   7,  22, -27,  10,
  -19,  14,   5, -30,  26,  -6,  18, -11,
    9, -24,  16,   3, -12,  29, -21,   8,
   -4,  25, -17,   6,  20, -28,  13,  -9
};

static volatile int32_t benchmark_sink;

static void make_synthetic_sensor_window(void)
{
  uint32_t state = 0x4D4C4245U; /* "MLBE" */

  for (uint32_t i = 0; i < ML_BENCH_VECTOR_LENGTH; ++i)
  {
    state = (state * 1664525U) + 1013904223U;
    sensor_window[i] = (int8_t)(((state >> 24) & 0x3FU) - 32);
  }
}

/* Our readable, unoptimized inference primitive. */
__attribute__((noinline))
static int32_t int8_dot_product_reference(const volatile int8_t *input,
                                          const int8_t *kernel,
                                          uint32_t length)
{
  int32_t sum = 0;

  for (uint32_t i = 0; i < length; ++i)
  {
    sum += (int32_t)input[i] * (int32_t)kernel[i];
  }

  return sum;
}

ml_bench_result_t ml_bench_run_reference(void)
{
  ml_bench_result_t result = {0};
  int32_t accumulator = 0;

  make_synthetic_sensor_window();

  DWT->CYCCNT = 0U;
  for (uint32_t i = 0; i < ML_BENCH_ITERATIONS; ++i)
  {
    accumulator += int8_dot_product_reference(sensor_window, weights,
                                               ML_BENCH_VECTOR_LENGTH);
  }
  result.total_cycles = DWT->CYCCNT;

  benchmark_sink = accumulator;
  result.score = int8_dot_product_reference(sensor_window, weights,
                                            ML_BENCH_VECTOR_LENGTH);
  result.iterations = ML_BENCH_ITERATIONS;
  result.cycles_per_run = result.total_cycles / result.iterations;

  return result;
}
