# On-device benchmark results

## Method

The first benchmark measures a 32-element signed `int8` dot product. It runs the
same reference kernel 1,000 times and reads the STM32 Cortex-M4 DWT cycle
counter immediately before and after the loop. UART output is outside the timed
region.

The synthetic input window and weights are deterministic, so the expected
reference score is stable across runs.

## Baseline 001: readable reference kernel

| Field | Value |
| --- | ---: |
| Board | NUCLEO-F446RE |
| Core clock | 84 MHz |
| Build preset | Debug |
| Kernel | `int8_dot_product_reference` |
| Vector length | 32 |
| Iterations | 1,000 |
| Score | 48 |
| Total cycles | 1,274,903 |
| Cycles per run | 1,274 |
| Approximate time per run | 15.2 µs |

This is an educational correctness baseline, not an optimized number. It
includes Debug-build overhead and deliberately prevents inlining. Future
measurements will compare an optimized build and a Cortex-M4 DSP implementation
against this exact input, weight set, and measurement method.
