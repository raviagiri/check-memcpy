
/**
 * @author Ravi A Giri <ravi.a.giri@intel.com>
 * @link https://github.com/raviagiri/check-memcpy
 *
 * Benchmark memcpy for Intel Innovation Demo
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>

#include "libs/memcpy_multithread.h"

#define MEM_ALLOC (1024 * 1024 * 128)
#define TEST_COUNT 10

#define START_BENCHMARK(LABEL) do {                         \
    uint32_t i = 0;                                         \
    total = min = max = 0;                                  \
    chk_##LABEL:                                            \
    src = (char *)malloc(MEM_ALLOC);                        \
    dst = (char *)malloc(MEM_ALLOC);                        \
    printf("Initializing data...\n");                \
    fflush(stdout);                                         \
    memset(src, 'a', MEM_ALLOC);                            \
    start_time = get_timestamp();


#define END_BENCHMARK(LABEL)                                \
    end_time = get_timestamp();                             \
    double secs = (end_time - start_time) / 1000000.0L;     \
    printf("%0.10lf seconds\n", secs);                      \
    printf("Validating copied data...\n");                \
    assert(!memcmp(dst, src, MEM_ALLOC));                   \
    fflush(stdout);                                         \
    total += secs;                                          \
    min = i ? (min > secs ? secs : min) : secs;             \
    max = i ? (max < secs ? secs : max) : secs;             \
    free(dst); free(src);                                   \
    if (i++ < TEST_COUNT) goto chk_##LABEL;                 \
    printf("Max\t: %0.10lf\n", max);                        \
    printf("Min\t: %0.10lf\n", min);                        \
    printf("Avg\t: %0.10lf\n", total / TEST_COUNT);         \
    printf("Total\t: %0.10lf\n", total);                    \
  } while (0)

void *memcpy_avx256(void *dst, const void *src, size_t len);
void *memcpy_avx512(void *dst, const void *src, size_t len);
void *memcpy_movsb(void *dst, const void *src, size_t len);

typedef unsigned long long timestamp_t;

static void
usage(char *app) {
  printf("Usage: %s 1|2|3|4 [1 == libc, 2 == avx2, 3 == avx-512 and 4 == movsb]\n", app);
}

static timestamp_t
get_timestamp()
{
  struct timeval now;
  gettimeofday(&now, NULL);
  return now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}


int
main(int argc, char *argv[])
{

  if (argc != 2) {
    usage(argv[0]);
    return 1;
  }

  double total, min, max;
  char *dst = NULL, *src = NULL;
  timestamp_t start_time, end_time;

  switch (argv[1][0]) {
    case '1':
      printf("Benchmarking memcpy libc...\n");
      START_BENCHMARK(memcpy);
      memcpy(dst, src, MEM_ALLOC);
      END_BENCHMARK(memcpy);
      break;

    case '2':
      printf("Benchmarking memcpy_avx256...\n");
      START_BENCHMARK(memcpy_avx256);
      memcpy_avx256(dst, src, MEM_ALLOC);
      END_BENCHMARK(memcpy_avx256);
      break;

    case '3':
      printf("Benchmarking memcpy_avx512...\n");
      START_BENCHMARK(memcpy_avx512);
      memcpy_avx512(dst, src, MEM_ALLOC);
      END_BENCHMARK(memcpy_avx512);
      break;

    case '4':
      printf("Benchmarking memcpy_movsb...\n");
      START_BENCHMARK(memcpy_movsb);
      memcpy_movsb(dst, src, MEM_ALLOC);
      END_BENCHMARK(memcpy_movsb);
      break;

    case '5':
      memcpy_mt_init(4);
      memcpy_mt_set_callback(memcpy_avx256);
      printf("Benchmarking memcpy_mt_exec...\n");
      START_BENCHMARK(memcpy_mt);
      memcpy_mt_exec(dst, src, MEM_ALLOC);
      END_BENCHMARK(memcpy_mt);
      memcpy_mt_destroy();
      break;

    default:
      usage(argv[0]);
      free(dst);
      free(src);
      return 1;
  }
  printf("OK!\n");
  return 0;
}
