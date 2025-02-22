/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -fno-vect-cost-model -ffast-math" } */

#include <stdint-gcc.h>

#define DEF_LOOP(TYPE, NAME, OP)			\
  void __attribute__ ((noipa))				\
  test_##TYPE##_##NAME (TYPE *__restrict r,		\
			TYPE *__restrict a,		\
			TYPE *__restrict b, TYPE c,	\
			TYPE *__restrict pred, int n)	\
  {							\
    for (int i = 0; i < n; ++i)				\
      r[i] = pred[i] ? -a[i] OP b[i] * c : 5;		\
  }

#define TEST_TYPE(T, TYPE) \
  T (TYPE, add, +) \
  T (TYPE, sub, -)

#define TEST_ALL(T) \
  TEST_TYPE (T, _Float16) \
  TEST_TYPE (T, float) \
  TEST_TYPE (T, double)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {vfnmacc\.vv\s+v[0-9]+,v[0-9]+,v[0-9]+} 3 } } */
/* { dg-final { scan-assembler-times {vfmsac\.vv\s+v[0-9]+,v[0-9]+,v[0-9]+,v0.t} 3 } } */
