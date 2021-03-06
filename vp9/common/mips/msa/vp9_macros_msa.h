/*
 *  Copyright (c) 2015 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef VP9_COMMON_MIPS_MSA_VP9_MACROS_MSA_H_
#define VP9_COMMON_MIPS_MSA_VP9_MACROS_MSA_H_

#include <msa.h>

#include "./vpx_config.h"
#include "vpx/vpx_integer.h"

#define LD_B(RTYPE, psrc) *((const RTYPE *)(psrc))
#define LD_UB(...) LD_B(v16u8, __VA_ARGS__)
#define LD_SB(...) LD_B(v16i8, __VA_ARGS__)

#define LD_H(RTYPE, psrc) *((const RTYPE *)(psrc))
#define LD_UH(...) LD_H(v8u16, __VA_ARGS__)
#define LD_SH(...) LD_H(v8i16, __VA_ARGS__)

#define LD_W(RTYPE, psrc) *((const RTYPE *)(psrc))
#define LD_SW(...) LD_W(v4i32, __VA_ARGS__)

#define ST_B(RTYPE, in, pdst) *((RTYPE *)(pdst)) = (in)
#define ST_UB(...) ST_B(v16u8, __VA_ARGS__)
#define ST_SB(...) ST_B(v16i8, __VA_ARGS__)

#define ST_H(RTYPE, in, pdst) *((RTYPE *)(pdst)) = (in)
#define ST_SH(...) ST_H(v8i16, __VA_ARGS__)

#define ST_W(RTYPE, in, pdst) *((RTYPE *)(pdst)) = (in)
#define ST_SW(...) ST_W(v4i32, __VA_ARGS__)

#if (__mips_isa_rev >= 6)
#define LH(psrc) ({                                 \
  const uint8_t *psrc_m = (const uint8_t *)(psrc);  \
  uint16_t val_m;                                   \
                                                    \
  __asm__ __volatile__ (                            \
      "lh  %[val_m],  %[psrc_m]  \n\t"              \
                                                    \
      : [val_m] "=r" (val_m)                        \
      : [psrc_m] "m" (*psrc_m)                      \
  );                                                \
                                                    \
  val_m;                                            \
})

#define LW(psrc) ({                                 \
  const uint8_t *psrc_m = (const uint8_t *)(psrc);  \
  uint32_t val_m;                                   \
                                                    \
  __asm__ __volatile__ (                            \
      "lw  %[val_m],  %[psrc_m]  \n\t"              \
                                                    \
      : [val_m] "=r" (val_m)                        \
      : [psrc_m] "m" (*psrc_m)                      \
  );                                                \
                                                    \
  val_m;                                            \
})

#if (__mips == 64)
#define LD(psrc) ({                                 \
  const uint8_t *psrc_m = (const uint8_t *)(psrc);  \
  uint64_t val_m = 0;                               \
                                                    \
  __asm__ __volatile__ (                            \
      "ld  %[val_m],  %[psrc_m]  \n\t"              \
                                                    \
      : [val_m] "=r" (val_m)                        \
      : [psrc_m] "m" (*psrc_m)                      \
  );                                                \
                                                    \
  val_m;                                            \
})
#else  // !(__mips == 64)
#define LD(psrc) ({                                        \
  const uint8_t *psrc_m = (const uint8_t *)(psrc);         \
  uint32_t val0_m, val1_m;                                 \
  uint64_t val_m = 0;                                      \
                                                           \
  val0_m = LW(psrc_m);                                     \
  val1_m = LW(psrc_m + 4);                                 \
                                                           \
  val_m = (uint64_t)(val1_m);                              \
  val_m = (uint64_t)((val_m << 32) & 0xFFFFFFFF00000000);  \
  val_m = (uint64_t)(val_m | (uint64_t)val0_m);            \
                                                           \
  val_m;                                                   \
})
#endif  // (__mips == 64)

#define SH(val, pdst) {                 \
  uint8_t *pdst_m = (uint8_t *)(pdst);  \
  const uint16_t val_m = (val);         \
                                        \
  __asm__ __volatile__ (                \
      "sh  %[val_m],  %[pdst_m]  \n\t"  \
                                        \
      : [pdst_m] "=m" (*pdst_m)         \
      : [val_m] "r" (val_m)             \
  );                                    \
}

#define SW(val, pdst) {                 \
  uint8_t *pdst_m = (uint8_t *)(pdst);  \
  const uint32_t val_m = (val);         \
                                        \
  __asm__ __volatile__ (                \
      "sw  %[val_m],  %[pdst_m]  \n\t"  \
                                        \
      : [pdst_m] "=m" (*pdst_m)         \
      : [val_m] "r" (val_m)             \
  );                                    \
}

#define SD(val, pdst) {                 \
  uint8_t *pdst_m = (uint8_t *)(pdst);  \
  const uint64_t val_m = (val);         \
                                        \
  __asm__ __volatile__ (                \
      "sd  %[val_m],  %[pdst_m]  \n\t"  \
                                        \
      : [pdst_m] "=m" (*pdst_m)         \
      : [val_m] "r" (val_m)             \
  );                                    \
}
#else  // !(__mips_isa_rev >= 6)
#define LH(psrc) ({                                 \
  const uint8_t *psrc_m = (const uint8_t *)(psrc);  \
  uint16_t val_m;                                   \
                                                    \
  __asm__ __volatile__ (                            \
      "ulh  %[val_m],  %[psrc_m]  \n\t"             \
                                                    \
      : [val_m] "=r" (val_m)                        \
      : [psrc_m] "m" (*psrc_m)                      \
  );                                                \
                                                    \
  val_m;                                            \
})

#define LW(psrc) ({                                 \
  const uint8_t *psrc_m = (const uint8_t *)(psrc);  \
  uint32_t val_m;                                   \
                                                    \
  __asm__ __volatile__ (                            \
      "ulw  %[val_m],  %[psrc_m]  \n\t"             \
                                                    \
      : [val_m] "=r" (val_m)                        \
      : [psrc_m] "m" (*psrc_m)                      \
  );                                                \
                                                    \
  val_m;                                            \
})

#if (__mips == 64)
#define LD(psrc) ({                                 \
  const uint8_t *psrc_m = (const uint8_t *)(psrc);  \
  uint64_t val_m = 0;                               \
                                                    \
  __asm__ __volatile__ (                            \
      "uld  %[val_m],  %[psrc_m]  \n\t"             \
                                                    \
      : [val_m] "=r" (val_m)                        \
      : [psrc_m] "m" (*psrc_m)                      \
  );                                                \
                                                    \
  val_m;                                            \
})
#else  // !(__mips == 64)
#define LD(psrc) ({                                        \
  const uint8_t *psrc_m1 = (const uint8_t *)(psrc);        \
  uint32_t val0_m, val1_m;                                 \
  uint64_t val_m = 0;                                      \
                                                           \
  val0_m = LW(psrc_m1);                                    \
  val1_m = LW(psrc_m1 + 4);                                \
                                                           \
  val_m = (uint64_t)(val1_m);                              \
  val_m = (uint64_t)((val_m << 32) & 0xFFFFFFFF00000000);  \
  val_m = (uint64_t)(val_m | (uint64_t)val0_m);            \
                                                           \
  val_m;                                                   \
})
#endif  // (__mips == 64)

#define SH(val, pdst) {                  \
  uint8_t *pdst_m = (uint8_t *)(pdst);   \
  const uint16_t val_m = (val);          \
                                         \
  __asm__ __volatile__ (                 \
      "ush  %[val_m],  %[pdst_m]  \n\t"  \
                                         \
      : [pdst_m] "=m" (*pdst_m)          \
      : [val_m] "r" (val_m)              \
  );                                     \
}

#define SW(val, pdst) {                  \
  uint8_t *pdst_m = (uint8_t *)(pdst);   \
  const uint32_t val_m = (val);          \
                                         \
  __asm__ __volatile__ (                 \
      "usw  %[val_m],  %[pdst_m]  \n\t"  \
                                         \
      : [pdst_m] "=m" (*pdst_m)          \
      : [val_m] "r" (val_m)              \
  );                                     \
}

#define SD(val, pdst) {                                     \
  uint8_t *pdst_m1 = (uint8_t *)(pdst);                     \
  uint32_t val0_m, val1_m;                                  \
                                                            \
  val0_m = (uint32_t)((val) & 0x00000000FFFFFFFF);          \
  val1_m = (uint32_t)(((val) >> 32) & 0x00000000FFFFFFFF);  \
                                                            \
  SW(val0_m, pdst_m1);                                      \
  SW(val1_m, pdst_m1 + 4);                                  \
}
#endif  // (__mips_isa_rev >= 6)

/* Description : Store 4 words with stride
   Arguments   : Inputs  - in0, in1, in2, in3, pdst, stride
   Details     : Stores word from 'in0' to (pdst)
                 Stores word from 'in1' to (pdst + stride)
                 Stores word from 'in2' to (pdst + 2 * stride)
                 Stores word from 'in3' to (pdst + 3 * stride)
*/
#define SW4(in0, in1, in2, in3, pdst, stride) {  \
  SW(in0, (pdst))                                \
  SW(in1, (pdst) + stride);                      \
  SW(in2, (pdst) + 2 * stride);                  \
  SW(in3, (pdst) + 3 * stride);                  \
}

/* Description : Store 4 double words with stride
   Arguments   : Inputs  - in0, in1, in2, in3, pdst, stride
   Details     : Stores double word from 'in0' to (pdst)
                 Stores double word from 'in1' to (pdst + stride)
                 Stores double word from 'in2' to (pdst + 2 * stride)
                 Stores double word from 'in3' to (pdst + 3 * stride)
*/
#define SD4(in0, in1, in2, in3, pdst, stride) {  \
  SD(in0, (pdst))                                \
  SD(in1, (pdst) + stride);                      \
  SD(in2, (pdst) + 2 * stride);                  \
  SD(in3, (pdst) + 3 * stride);                  \
}

/* Description : Load vectors with 16 byte elements with stride
   Arguments   : Inputs  - psrc    (source pointer to load from)
                         - stride
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Loads 16 byte elements in 'out0' from (psrc)
                 Loads 16 byte elements in 'out1' from (psrc + stride)
*/
#define LD_B2(RTYPE, psrc, stride, out0, out1) {  \
  out0 = LD_B(RTYPE, (psrc));                     \
  out1 = LD_B(RTYPE, (psrc) + stride);            \
}
#define LD_UB2(...) LD_B2(v16u8, __VA_ARGS__)
#define LD_SB2(...) LD_B2(v16i8, __VA_ARGS__)

#define LD_B4(RTYPE, psrc, stride, out0, out1, out2, out3) {  \
  LD_B2(RTYPE, (psrc), stride, out0, out1);                   \
  LD_B2(RTYPE, (psrc) + 2 * stride , stride, out2, out3);     \
}
#define LD_UB4(...) LD_B4(v16u8, __VA_ARGS__)
#define LD_SB4(...) LD_B4(v16i8, __VA_ARGS__)

#define LD_B5(RTYPE, psrc, stride, out0, out1, out2, out3, out4) {  \
  LD_B4(RTYPE, (psrc), stride, out0, out1, out2, out3);             \
  out4 = LD_B(RTYPE, (psrc) + 4 * stride);                          \
}
#define LD_UB5(...) LD_B5(v16u8, __VA_ARGS__)
#define LD_SB5(...) LD_B5(v16i8, __VA_ARGS__)

#define LD_B7(RTYPE, psrc, stride,                             \
              out0, out1, out2, out3, out4, out5, out6) {      \
  LD_B5(RTYPE, (psrc), stride, out0, out1, out2, out3, out4);  \
  LD_B2(RTYPE, (psrc) + 5 * stride, stride, out5, out6);       \
}
#define LD_SB7(...) LD_B7(v16i8, __VA_ARGS__)

#define LD_B8(RTYPE, psrc, stride,                                    \
              out0, out1, out2, out3, out4, out5, out6, out7) {       \
  LD_B4(RTYPE, (psrc), stride, out0, out1, out2, out3);               \
  LD_B4(RTYPE, (psrc) + 4 * stride, stride, out4, out5, out6, out7);  \
}
#define LD_UB8(...) LD_B8(v16u8, __VA_ARGS__)
#define LD_SB8(...) LD_B8(v16i8, __VA_ARGS__)

/* Description : Load vectors with 8 halfword elements with stride
   Arguments   : Inputs  - psrc    (source pointer to load from)
                         - stride
                 Outputs - out0, out1
   Details     : Loads 8 halfword elements in 'out0' from (psrc)
                 Loads 8 halfword elements in 'out1' from (psrc + stride)
*/
#define LD_H2(RTYPE, psrc, stride, out0, out1) {  \
  out0 = LD_H(RTYPE, (psrc));                     \
  out1 = LD_H(RTYPE, (psrc) + (stride));          \
}
#define LD_SH2(...) LD_H2(v8i16, __VA_ARGS__)

#define LD_H4(RTYPE, psrc, stride, out0, out1, out2, out3) {  \
  LD_H2(RTYPE, (psrc), stride, out0, out1);                   \
  LD_H2(RTYPE, (psrc) + 2 * stride, stride, out2, out3);      \
}
#define LD_SH4(...) LD_H4(v8i16, __VA_ARGS__)

#define LD_H8(RTYPE, psrc, stride,                                    \
              out0, out1, out2, out3, out4, out5, out6, out7) {       \
  LD_H4(RTYPE, (psrc), stride, out0, out1, out2, out3);               \
  LD_H4(RTYPE, (psrc) + 4 * stride, stride, out4, out5, out6, out7);  \
}
#define LD_SH8(...) LD_H8(v8i16, __VA_ARGS__)

#define LD_H16(RTYPE, psrc, stride,                                     \
               out0, out1, out2, out3, out4, out5, out6, out7,          \
               out8, out9, out10, out11, out12, out13, out14, out15) {  \
  LD_H8(RTYPE, (psrc), stride,                                          \
        out0, out1, out2, out3, out4, out5, out6, out7);                \
  LD_H8(RTYPE, (psrc) + 8 * stride, stride,                             \
        out8, out9, out10, out11, out12, out13, out14, out15);          \
}
#define LD_SH16(...) LD_H16(v8i16, __VA_ARGS__)

/* Description : Store vectors of 16 byte elements with stride
   Arguments   : Inputs  - in0, in1, stride
                 Outputs - pdst    (destination pointer to store to)
   Details     : Stores 16 byte elements from 'in0' to (pdst)
                 Stores 16 byte elements from 'in1' to (pdst + stride)
*/
#define ST_B2(RTYPE, in0, in1, pdst, stride) {  \
  ST_B(RTYPE, in0, (pdst));                     \
  ST_B(RTYPE, in1, (pdst) + stride);            \
}
#define ST_UB2(...) ST_B2(v16u8, __VA_ARGS__)

#define ST_B4(RTYPE, in0, in1, in2, in3, pdst, stride) {  \
  ST_B2(RTYPE, in0, in1, (pdst), stride);                 \
  ST_B2(RTYPE, in2, in3, (pdst) + 2 * stride, stride);    \
}
#define ST_UB4(...) ST_B4(v16u8, __VA_ARGS__)

#define ST_B8(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,      \
              pdst, stride) {                                     \
  ST_B4(RTYPE, in0, in1, in2, in3, pdst, stride);                 \
  ST_B4(RTYPE, in4, in5, in6, in7, (pdst) + 4 * stride, stride);  \
}
#define ST_UB8(...) ST_B8(v16u8, __VA_ARGS__)

/* Description : Store vectors of 8 halfword elements with stride
   Arguments   : Inputs  - in0, in1, stride
                 Outputs - pdst    (destination pointer to store to)
   Details     : Stores 8 halfword elements from 'in0' to (pdst)
                 Stores 8 halfword elements from 'in1' to (pdst + stride)
*/
#define ST_H2(RTYPE, in0, in1, pdst, stride) {  \
  ST_H(RTYPE, in0, (pdst));                     \
  ST_H(RTYPE, in1, (pdst) + stride);            \
}
#define ST_SH2(...) ST_H2(v8i16, __VA_ARGS__)

#define ST_H4(RTYPE, in0, in1, in2, in3, pdst, stride) {  \
  ST_H2(RTYPE, in0, in1, (pdst), stride);                 \
  ST_H2(RTYPE, in2, in3, (pdst) + 2 * stride, stride);    \
}
#define ST_SH4(...) ST_H4(v8i16, __VA_ARGS__)

#define ST_H8(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7, pdst, stride) {  \
  ST_H4(RTYPE, in0, in1, in2, in3, (pdst), stride);                           \
  ST_H4(RTYPE, in4, in5, in6, in7, (pdst) + 4 * stride, stride);              \
}
#define ST_SH8(...) ST_H8(v8i16, __VA_ARGS__)

/* Description : Store as 4x4 byte block to destination memory from input vector
   Arguments   : Inputs  - in0, in1, pdst, stride
                 Return Type - unsigned byte
   Details     : Idx0 word element from input vector 'in0' is copied and stored
                 on first line
                 Idx1 word element from input vector 'in0' is copied and stored
                 on second line
                 Idx2 word element from input vector 'in1' is copied and stored
                 on third line
                 Idx3 word element from input vector 'in1' is copied and stored
                 on fourth line
*/
#define ST4x4_UB(in0, in1, idx0, idx1, idx2, idx3, pdst, stride) {  \
  uint32_t out0_m, out1_m, out2_m, out3_m;                          \
  uint8_t *pblk_4x4_m = (uint8_t *)(pdst);                          \
                                                                    \
  out0_m = __msa_copy_u_w((v4i32)in0, idx0);                        \
  out1_m = __msa_copy_u_w((v4i32)in0, idx1);                        \
  out2_m = __msa_copy_u_w((v4i32)in1, idx2);                        \
  out3_m = __msa_copy_u_w((v4i32)in1, idx3);                        \
                                                                    \
  SW4(out0_m, out1_m, out2_m, out3_m, pblk_4x4_m, stride);          \
}
#define ST4x8_UB(in0, in1, pdst, stride) {                        \
  uint8_t *pblk_4x8 = (uint8_t *)(pdst);                          \
                                                                  \
  ST4x4_UB(in0, in0, 0, 1, 2, 3, pblk_4x8, stride);               \
  ST4x4_UB(in1, in1, 0, 1, 2, 3, pblk_4x8 + 4 * stride, stride);  \
}

/* Description : Store as 8x1 byte block to destination memory from input vector
   Arguments   : Inputs  - in, pdst
   Details     : Index 0 double word element from input vector 'in' is copied
                 and stored to destination memory at (pdst)
*/
#define ST8x1_UB(in, pdst) {              \
  uint64_t out0_m;                        \
                                          \
  out0_m = __msa_copy_u_d((v2i64)in, 0);  \
  SD(out0_m, pdst);                       \
}

/* Description : Store as 8x4 byte block to destination memory from input
                 vectors
   Arguments   : Inputs  - in0, in1, pdst, stride
   Details     : Index 0 double word element from input vector 'in0' is copied
                 and stored to destination memory at (pblk_8x4_m)
                 Index 1 double word element from input vector 'in0' is copied
                 and stored to destination memory at (pblk_8x4_m + stride)
                 Index 0 double word element from input vector 'in1' is copied
                 and stored to destination memory at (pblk_8x4_m + 2 * stride)
                 Index 1 double word element from input vector 'in1' is copied
                 and stored to destination memory at (pblk_8x4_m + 3 * stride)
*/
#define ST8x4_UB(in0, in1, pdst, stride) {                  \
  uint64_t out0_m, out1_m, out2_m, out3_m;                  \
  uint8_t *pblk_8x4_m = (uint8_t *)(pdst);                  \
                                                            \
  out0_m = __msa_copy_u_d((v2i64)in0, 0);                   \
  out1_m = __msa_copy_u_d((v2i64)in0, 1);                   \
  out2_m = __msa_copy_u_d((v2i64)in1, 0);                   \
  out3_m = __msa_copy_u_d((v2i64)in1, 1);                   \
                                                            \
  SD4(out0_m, out1_m, out2_m, out3_m, pblk_8x4_m, stride);  \
}

/* Description : average with rounding (in0 + in1 + 1) / 2.
   Arguments   : Inputs  - in0, in1, in2, in3,
                 Outputs - out0, out1
                 Return Type - signed byte
   Details     : Each byte element from 'in0' vector is added with each byte
                 element from 'in1' vector. The addition of the elements plus 1
                (for rounding) is done unsigned with full precision,
                i.e. the result has one extra bit. Unsigned division by 2
                (or logical shift right by one bit) is performed before writing
                the result to vector 'out0'
                Similar for the pair of 'in2' and 'in3'
*/
#define AVER_UB2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE)__msa_aver_u_b((v16u8)in0, (v16u8)in1);    \
  out1 = (RTYPE)__msa_aver_u_b((v16u8)in2, (v16u8)in3);    \
}
#define AVER_UB2_UB(...) AVER_UB2(v16u8, __VA_ARGS__)

#define AVER_UB4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3) {                       \
  AVER_UB2(RTYPE, in0, in1, in2, in3, out0, out1)                \
  AVER_UB2(RTYPE, in4, in5, in6, in7, out2, out3)                \
}
#define AVER_UB4_UB(...) AVER_UB4(v16u8, __VA_ARGS__)

/* Description : Immediate number of columns to slide with zero
   Arguments   : Inputs  - in0, in1, slide_val
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Byte elements from 'zero_m' vector are slide into 'in0' by
                 number of elements specified by 'slide_val'
*/
#define SLDI_B2_0(RTYPE, in0, in1, out0, out1, slide_val) {          \
  v16i8 zero_m = { 0 };                                              \
  out0 = (RTYPE)__msa_sldi_b((v16i8)zero_m, (v16i8)in0, slide_val);  \
  out1 = (RTYPE)__msa_sldi_b((v16i8)zero_m, (v16i8)in1, slide_val);  \
}
#define SLDI_B2_0_SW(...) SLDI_B2_0(v4i32, __VA_ARGS__)

/* Description : Immediate number of columns to slide
   Arguments   : Inputs  - in0_0, in0_1, in1_0, in1_1, slide_val
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Byte elements from 'in0_0' vector are slide into 'in1_0' by
                 number of elements specified by 'slide_val'
*/
#define SLDI_B2(RTYPE, in0_0, in0_1, in1_0, in1_1, out0, out1, slide_val) {  \
  out0 = (RTYPE)__msa_sldi_b((v16i8)in0_0, (v16i8)in1_0, slide_val);         \
  out1 = (RTYPE)__msa_sldi_b((v16i8)in0_1, (v16i8)in1_1, slide_val);         \
}
#define SLDI_B2_SH(...) SLDI_B2(v8i16, __VA_ARGS__)

#define SLDI_B3(RTYPE, in0_0, in0_1, in0_2, in1_0, in1_1, in1_2,      \
                out0, out1, out2, slide_val) {                        \
  SLDI_B2(RTYPE, in0_0, in0_1, in1_0, in1_1, out0, out1, slide_val)   \
  out2 = (RTYPE)__msa_sldi_b((v16i8)in0_2, (v16i8)in1_2, slide_val);  \
}
#define SLDI_B3_SB(...) SLDI_B3(v16i8, __VA_ARGS__)
#define SLDI_B3_UH(...) SLDI_B3(v8u16, __VA_ARGS__)

/* Description : Shuffle byte vector elements as per mask vector
   Arguments   : Inputs  - in0, in1, in2, in3, mask0, mask1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Selective byte elements from in0 & in1 are copied to out0 as
                 per control vector mask0
                 Selective byte elements from in2 & in3 are copied to out1 as
                 per control vector mask1
*/
#define VSHF_B2(RTYPE, in0, in1, in2, in3, mask0, mask1, out0, out1) {  \
  out0 = (RTYPE)__msa_vshf_b((v16i8)mask0, (v16i8)in1, (v16i8)in0);     \
  out1 = (RTYPE)__msa_vshf_b((v16i8)mask1, (v16i8)in3, (v16i8)in2);     \
}
#define VSHF_B2_UB(...) VSHF_B2(v16u8, __VA_ARGS__)
#define VSHF_B2_SB(...) VSHF_B2(v16i8, __VA_ARGS__)
#define VSHF_B2_UH(...) VSHF_B2(v8u16, __VA_ARGS__)

#define VSHF_B4(RTYPE, in0, in1, mask0, mask1, mask2, mask3,     \
                out0, out1, out2, out3) {                        \
  VSHF_B2(RTYPE, in0, in1, in0, in1, mask0, mask1, out0, out1);  \
  VSHF_B2(RTYPE, in0, in1, in0, in1, mask2, mask3, out2, out3);  \
}
#define VSHF_B4_SB(...) VSHF_B4(v16i8, __VA_ARGS__)
#define VSHF_B4_SH(...) VSHF_B4(v8i16, __VA_ARGS__)

/* Description : Dot product of byte vector elements
   Arguments   : Inputs  - mult0, mult1
                           cnst0, cnst1
                 Outputs - out0, out1
                 Return Type - unsigned halfword
   Details     : Unsigned byte elements from mult0 are multiplied with
                 unsigned byte elements from cnst0 producing a result
                 twice the size of input i.e. unsigned halfword.
                 Then this multiplication results of adjacent odd-even elements
                 are added together and stored to the out vector
                 (2 unsigned halfword results)
*/
#define DOTP_UB2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1) {  \
  out0 = (RTYPE)__msa_dotp_u_h((v16u8)mult0, (v16u8)cnst0);        \
  out1 = (RTYPE)__msa_dotp_u_h((v16u8)mult1, (v16u8)cnst1);        \
}
#define DOTP_UB2_UH(...) DOTP_UB2(v8u16, __VA_ARGS__)

#define DOTP_UB4(RTYPE, mult0, mult1, mult2, mult3,         \
                 cnst0, cnst1, cnst2, cnst3,                \
                 out0, out1, out2, out3) {                  \
  DOTP_UB2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1);  \
  DOTP_UB2(RTYPE, mult2, mult3, cnst2, cnst3, out2, out3);  \
}
#define DOTP_UB4_UH(...) DOTP_UB4(v8u16, __VA_ARGS__)

/* Description : Dot product of byte vector elements
   Arguments   : Inputs  - mult0, mult1
                           cnst0, cnst1
                 Outputs - out0, out1
                 Return Type - signed halfword
   Details     : Signed byte elements from mult0 are multiplied with
                 signed byte elements from cnst0 producing a result
                 twice the size of input i.e. signed halfword.
                 Then this multiplication results of adjacent odd-even elements
                 are added together and stored to the out vector
                 (2 signed halfword results)
*/
#define DOTP_SB2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1) {  \
  out0 = (RTYPE)__msa_dotp_s_h((v16i8)mult0, (v16i8)cnst0);        \
  out1 = (RTYPE)__msa_dotp_s_h((v16i8)mult1, (v16i8)cnst1);        \
}
#define DOTP_SB2_SH(...) DOTP_SB2(v8i16, __VA_ARGS__)

#define DOTP_SB4(RTYPE, mult0, mult1, mult2, mult3,                     \
                 cnst0, cnst1, cnst2, cnst3, out0, out1, out2, out3) {  \
  DOTP_SB2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1);              \
  DOTP_SB2(RTYPE, mult2, mult3, cnst2, cnst3, out2, out3);              \
}
#define DOTP_SB4_SH(...) DOTP_SB4(v8i16, __VA_ARGS__)

/* Description : Dot product of halfword vector elements
   Arguments   : Inputs  - mult0, mult1
                           cnst0, cnst1
                 Outputs - out0, out1
                 Return Type - signed word
   Details     : Signed halfword elements from mult0 are multiplied with
                 signed halfword elements from cnst0 producing a result
                 twice the size of input i.e. signed word.
                 Then this multiplication results of adjacent odd-even elements
                 are added together and stored to the out vector
                 (2 signed word results)
*/
#define DOTP_SH2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1) {  \
  out0 = (RTYPE)__msa_dotp_s_w((v8i16)mult0, (v8i16)cnst0);        \
  out1 = (RTYPE)__msa_dotp_s_w((v8i16)mult1, (v8i16)cnst1);        \
}
#define DOTP_SH2_SW(...) DOTP_SH2(v4i32, __VA_ARGS__)

#define DOTP_SH4(RTYPE, mult0, mult1, mult2, mult3,         \
                 cnst0, cnst1, cnst2, cnst3,                \
                 out0, out1, out2, out3) {                  \
  DOTP_SH2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1);  \
  DOTP_SH2(RTYPE, mult2, mult3, cnst2, cnst3, out2, out3);  \
}
#define DOTP_SH4_SW(...) DOTP_SH4(v4i32, __VA_ARGS__)

/* Description : Dot product & addition of byte vector elements
   Arguments   : Inputs  - mult0, mult1
                           cnst0, cnst1
                 Outputs - out0, out1
                 Return Type - signed halfword
   Details     : Signed byte elements from mult0 are multiplied with
                 signed byte elements from cnst0 producing a result
                 twice the size of input i.e. signed halfword.
                 Then this multiplication results of adjacent odd-even elements
                 are added to the out vector
                 (2 signed halfword results)
*/
#define DPADD_SB2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1) {         \
  out0 = (RTYPE)__msa_dpadd_s_h((v8i16)out0, (v16i8)mult0, (v16i8)cnst0);  \
  out1 = (RTYPE)__msa_dpadd_s_h((v8i16)out1, (v16i8)mult1, (v16i8)cnst1);  \
}
#define DPADD_SB2_SH(...) DPADD_SB2(v8i16, __VA_ARGS__)

#define DPADD_SB4(RTYPE, mult0, mult1, mult2, mult3,                     \
                  cnst0, cnst1, cnst2, cnst3, out0, out1, out2, out3) {  \
  DPADD_SB2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1);              \
  DPADD_SB2(RTYPE, mult2, mult3, cnst2, cnst3, out2, out3);              \
}
#define DPADD_SB4_SH(...) DPADD_SB4(v8i16, __VA_ARGS__)

/* Description : Minimum values between unsigned elements of
                 either vector are copied to the output vector
   Arguments   : Inputs  - in0, in1, min_vec
                 Outputs - in0, in1, (in place)
                 Return Type - unsigned halfword
   Details     : Minimum of unsigned halfword element values from 'in0' and
                 'min_value' are written to output vector 'in0'
*/
#define MIN_UH2(RTYPE, in0, in1, min_vec) {         \
  in0 = (RTYPE)__msa_min_u_h((v8u16)in0, min_vec);  \
  in1 = (RTYPE)__msa_min_u_h((v8u16)in1, min_vec);  \
}
#define MIN_UH2_UH(...) MIN_UH2(v8u16, __VA_ARGS__)

#define MIN_UH4(RTYPE, in0, in1, in2, in3, min_vec) {  \
  MIN_UH2(RTYPE, in0, in1, min_vec);                   \
  MIN_UH2(RTYPE, in2, in3, min_vec);                   \
}
#define MIN_UH4_UH(...) MIN_UH4(v8u16, __VA_ARGS__)

/* Description : Clips all signed halfword elements of input vector
                 between 0 & 255
   Arguments   : Inputs  - in       (input vector)
                 Outputs - out_m    (output vector with clipped elements)
                 Return Type - signed halfword
*/
#define CLIP_SH_0_255(in) ({                          \
  v8i16 max_m = __msa_ldi_h(255);                     \
  v8i16 out_m;                                        \
                                                      \
  out_m = __msa_maxi_s_h((v8i16)in, 0);               \
  out_m = __msa_min_s_h((v8i16)max_m, (v8i16)out_m);  \
  out_m;                                              \
})
#define CLIP_SH2_0_255(in0, in1) {  \
  in0 = CLIP_SH_0_255(in0);         \
  in1 = CLIP_SH_0_255(in1);         \
}
#define CLIP_SH4_0_255(in0, in1, in2, in3) {  \
  CLIP_SH2_0_255(in0, in1);                   \
  CLIP_SH2_0_255(in2, in3);                   \
}

/* Description : Interleave even byte elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even byte elements of 'in0' and even byte
                 elements of 'in1' are interleaved and copied to 'out0'
                 Even byte elements of 'in2' and even byte
                 elements of 'in3' are interleaved and copied to 'out1'
*/
#define ILVEV_B2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE)__msa_ilvev_b((v16i8)in1, (v16i8)in0);     \
  out1 = (RTYPE)__msa_ilvev_b((v16i8)in3, (v16i8)in2);     \
}
#define ILVEV_B2_UB(...) ILVEV_B2(v16u8, __VA_ARGS__)
#define ILVEV_B2_SH(...) ILVEV_B2(v8i16, __VA_ARGS__)

/* Description : Interleave even halfword elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even halfword elements of 'in0' and even halfword
                 elements of 'in1' are interleaved and copied to 'out0'
                 Even halfword elements of 'in2' and even halfword
                 elements of 'in3' are interleaved and copied to 'out1'
*/
#define ILVEV_H2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE)__msa_ilvev_h((v8i16)in1, (v8i16)in0);     \
  out1 = (RTYPE)__msa_ilvev_h((v8i16)in3, (v8i16)in2);     \
}
#define ILVEV_H2_UB(...) ILVEV_H2(v16u8, __VA_ARGS__)
#define ILVEV_H2_SH(...) ILVEV_H2(v8i16, __VA_ARGS__)
#define ILVEV_H2_SW(...) ILVEV_H2(v4i32, __VA_ARGS__)

/* Description : Interleave left half of byte elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Left half of byte elements of in0 and left half of byte
                 elements of in1 are interleaved and copied to out0.
                 Left half of byte elements of in2 and left half of byte
                 elements of in3 are interleaved and copied to out1.
*/
#define ILVL_B2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE)__msa_ilvl_b((v16i8)in0, (v16i8)in1);     \
  out1 = (RTYPE)__msa_ilvl_b((v16i8)in2, (v16i8)in3);     \
}
#define ILVL_B2_UB(...) ILVL_B2(v16u8, __VA_ARGS__)
#define ILVL_B2_SB(...) ILVL_B2(v16i8, __VA_ARGS__)
#define ILVL_B2_UH(...) ILVL_B2(v8u16, __VA_ARGS__)
#define ILVL_B2_SH(...) ILVL_B2(v8i16, __VA_ARGS__)

#define ILVL_B4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                out0, out1, out2, out3) {                       \
  ILVL_B2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  ILVL_B2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define ILVL_B4_SB(...) ILVL_B4(v16i8, __VA_ARGS__)
#define ILVL_B4_UH(...) ILVL_B4(v8u16, __VA_ARGS__)

/* Description : Interleave left half of halfword elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Left half of halfword elements of in0 and left half of halfword
                 elements of in1 are interleaved and copied to out0.
                 Left half of halfword elements of in2 and left half of halfword
                 elements of in3 are interleaved and copied to out1.
*/
#define ILVL_H2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE)__msa_ilvl_h((v8i16)in0, (v8i16)in1);     \
  out1 = (RTYPE)__msa_ilvl_h((v8i16)in2, (v8i16)in3);     \
}
#define ILVL_H2_SH(...) ILVL_H2(v8i16, __VA_ARGS__)

/* Description : Interleave left half of word elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Left half of word elements of in0 and left half of word
                 elements of in1 are interleaved and copied to out0.
                 Left half of word elements of in2 and left half of word
                 elements of in3 are interleaved and copied to out1.
*/
#define ILVL_W2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE)__msa_ilvl_w((v4i32)in0, (v4i32)in1);     \
  out1 = (RTYPE)__msa_ilvl_w((v4i32)in2, (v4i32)in3);     \
}
#define ILVL_W2_UB(...) ILVL_W2(v16u8, __VA_ARGS__)
#define ILVL_W2_SH(...) ILVL_W2(v8i16, __VA_ARGS__)

/* Description : Interleave right half of byte elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3, in4, in5, in6, in7
                 Outputs - out0, out1, out2, out3
                 Return Type - as per RTYPE
   Details     : Right half of byte elements of in0 and right half of byte
                 elements of in1 are interleaved and copied to out0.
                 Right half of byte elements of in2 and right half of byte
                 elements of in3 are interleaved and copied to out1.
                 Similar for other pairs
*/
#define ILVR_B2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE)__msa_ilvr_b((v16i8)in0, (v16i8)in1);     \
  out1 = (RTYPE)__msa_ilvr_b((v16i8)in2, (v16i8)in3);     \
}
#define ILVR_B2_UB(...) ILVR_B2(v16u8, __VA_ARGS__)
#define ILVR_B2_SB(...) ILVR_B2(v16i8, __VA_ARGS__)
#define ILVR_B2_UH(...) ILVR_B2(v8u16, __VA_ARGS__)
#define ILVR_B2_SH(...) ILVR_B2(v8i16, __VA_ARGS__)

#define ILVR_B4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                out0, out1, out2, out3) {                       \
  ILVR_B2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  ILVR_B2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define ILVR_B4_UB(...) ILVR_B4(v16u8, __VA_ARGS__)
#define ILVR_B4_SB(...) ILVR_B4(v16i8, __VA_ARGS__)
#define ILVR_B4_UH(...) ILVR_B4(v8u16, __VA_ARGS__)
#define ILVR_B4_SH(...) ILVR_B4(v8i16, __VA_ARGS__)

/* Description : Interleave right half of halfword elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3, in4, in5, in6, in7
                 Outputs - out0, out1, out2, out3
                 Return Type - signed halfword
   Details     : Right half of halfword elements of in0 and right half of
                 halfword elements of in1 are interleaved and copied to out0.
                 Right half of halfword elements of in2 and right half of
                 halfword elements of in3 are interleaved and copied to out1.
                 Similar for other pairs
*/
#define ILVR_H2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE)__msa_ilvr_h((v8i16)in0, (v8i16)in1);     \
  out1 = (RTYPE)__msa_ilvr_h((v8i16)in2, (v8i16)in3);     \
}
#define ILVR_H2_SH(...) ILVR_H2(v8i16, __VA_ARGS__)

#define ILVR_H4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                out0, out1, out2, out3) {                       \
  ILVR_H2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  ILVR_H2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define ILVR_H4_SH(...) ILVR_H4(v8i16, __VA_ARGS__)

#define ILVR_W2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE)__msa_ilvr_w((v4i32)in0, (v4i32)in1);     \
  out1 = (RTYPE)__msa_ilvr_w((v4i32)in2, (v4i32)in3);     \
}
#define ILVR_W2_UB(...) ILVR_W2(v16u8, __VA_ARGS__)
#define ILVR_W2_SH(...) ILVR_W2(v8i16, __VA_ARGS__)

/* Description : Interleave right half of double word elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3, in4, in5, in6, in7
                 Outputs - out0, out1, out2, out3
                 Return Type - unsigned double word
   Details     : Right half of double word elements of in0 and right half of
                 double word elements of in1 are interleaved and copied to out0.
                 Right half of double word elements of in2 and right half of
                 double word elements of in3 are interleaved and copied to out1.
*/
#define ILVR_D2(RTYPE, in0, in1, in2, in3, out0, out1) {   \
  out0 = (RTYPE)__msa_ilvr_d((v2i64)(in0), (v2i64)(in1));  \
  out1 = (RTYPE)__msa_ilvr_d((v2i64)(in2), (v2i64)(in3));  \
}
#define ILVR_D2_UB(...) ILVR_D2(v16u8, __VA_ARGS__)
#define ILVR_D2_SB(...) ILVR_D2(v16i8, __VA_ARGS__)
#define ILVR_D2_SH(...) ILVR_D2(v8i16, __VA_ARGS__)

#define ILVR_D3(RTYPE, in0, in1, in2, in3, in4, in5, out0, out1, out2) {  \
  ILVR_D2(RTYPE, in0, in1, in2, in3, out0, out1);                         \
  out2 = (RTYPE)__msa_ilvr_d((v2i64)(in4), (v2i64)(in5));                 \
}
#define ILVR_D3_SB(...) ILVR_D3(v16i8, __VA_ARGS__)

#define ILVR_D4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                out0, out1, out2, out3) {                       \
  ILVR_D2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  ILVR_D2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define ILVR_D4_SB(...) ILVR_D4(v16i8, __VA_ARGS__)
#define ILVR_D4_UB(...) ILVR_D4(v16u8, __VA_ARGS__)

/* Description : Interleave both left and right half of input vectors
   Arguments   : Inputs  - in0, in1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Right half of byte elements from 'in0' and 'in1' are
                 interleaved and stored to 'out0'
                 Left half of byte elements from 'in0' and 'in1' are
                 interleaved and stored to 'out1'
*/
#define ILVRL_B2(RTYPE, in0, in1, out0, out1) {        \
  out0 = (RTYPE)__msa_ilvr_b((v16i8)in0, (v16i8)in1);  \
  out1 = (RTYPE)__msa_ilvl_b((v16i8)in0, (v16i8)in1);  \
}
#define ILVRL_B2_UB(...) ILVRL_B2(v16u8, __VA_ARGS__)
#define ILVRL_B2_SB(...) ILVRL_B2(v16i8, __VA_ARGS__)
#define ILVRL_B2_UH(...) ILVRL_B2(v8u16, __VA_ARGS__)
#define ILVRL_B2_SH(...) ILVRL_B2(v8i16, __VA_ARGS__)

#define ILVRL_H2(RTYPE, in0, in1, out0, out1) {        \
  out0 = (RTYPE)__msa_ilvr_h((v8i16)in0, (v8i16)in1);  \
  out1 = (RTYPE)__msa_ilvl_h((v8i16)in0, (v8i16)in1);  \
}
#define ILVRL_H2_SH(...) ILVRL_H2(v8i16, __VA_ARGS__)
#define ILVRL_H2_SW(...) ILVRL_H2(v4i32, __VA_ARGS__)

#define ILVRL_W2(RTYPE, in0, in1, out0, out1) {        \
  out0 = (RTYPE)__msa_ilvr_w((v4i32)in0, (v4i32)in1);  \
  out1 = (RTYPE)__msa_ilvl_w((v4i32)in0, (v4i32)in1);  \
}
#define ILVRL_W2_SH(...) ILVRL_W2(v8i16, __VA_ARGS__)
#define ILVRL_W2_SW(...) ILVRL_W2(v4i32, __VA_ARGS__)

/* Description : Saturate the halfword element values to the max
                 unsigned value of (sat_val+1 bits)
                 The element data width remains unchanged
   Arguments   : Inputs  - in0, in1, in2, in3, sat_val
                 Outputs - in0, in1, in2, in3 (in place)
                 Return Type - unsigned halfword
   Details     : Each unsigned halfword element from 'in0' is saturated to the
                 value generated with (sat_val+1) bit range
                 Results are in placed to original vectors
*/
#define SAT_UH2(RTYPE, in0, in1, sat_val) {         \
  in0 = (RTYPE)__msa_sat_u_h((v8u16)in0, sat_val);  \
  in1 = (RTYPE)__msa_sat_u_h((v8u16)in1, sat_val);  \
}
#define SAT_UH2_UH(...) SAT_UH2(v8u16, __VA_ARGS__)

#define SAT_UH4(RTYPE, in0, in1, in2, in3, sat_val) {  \
  SAT_UH2(RTYPE, in0, in1, sat_val);                   \
  SAT_UH2(RTYPE, in2, in3, sat_val)                    \
}
#define SAT_UH4_UH(...) SAT_UH4(v8u16, __VA_ARGS__)

/* Description : Saturate the halfword element values to the max
                 unsigned value of (sat_val+1 bits)
                 The element data width remains unchanged
   Arguments   : Inputs  - in0, in1, in2, in3, sat_val
                 Outputs - in0, in1, in2, in3 (in place)
                 Return Type - unsigned halfword
   Details     : Each unsigned halfword element from 'in0' is saturated to the
                 value generated with (sat_val+1) bit range
                 Results are in placed to original vectors
*/
#define SAT_SH2(RTYPE, in0, in1, sat_val) {         \
  in0 = (RTYPE)__msa_sat_s_h((v8i16)in0, sat_val);  \
  in1 = (RTYPE)__msa_sat_s_h((v8i16)in1, sat_val);  \
}
#define SAT_SH2_SH(...) SAT_SH2(v8i16, __VA_ARGS__)

#define SAT_SH4(RTYPE, in0, in1, in2, in3, sat_val) {  \
  SAT_SH2(RTYPE, in0, in1, sat_val);                   \
  SAT_SH2(RTYPE, in2, in3, sat_val);                   \
}
#define SAT_SH4_SH(...) SAT_SH4(v8i16, __VA_ARGS__)

/* Description : Indexed halfword element values are replicated to all
                 elements in output vector
   Arguments   : Inputs  - in, idx0, idx1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : 'idx0' element value from 'in' vector is replicated to all
                  elements in 'out0' vector
                  Valid index range for halfword operation is 0-7
*/
#define SPLATI_H2(RTYPE, in, idx0, idx1, out0, out1) {  \
  out0 = (RTYPE)__msa_splati_h((v8i16)in, idx0);        \
  out1 = (RTYPE)__msa_splati_h((v8i16)in, idx1);        \
}
#define SPLATI_H2_SH(...) SPLATI_H2(v8i16, __VA_ARGS__)

#define SPLATI_H4(RTYPE, in, idx0, idx1, idx2, idx3,  \
                  out0, out1, out2, out3) {           \
  SPLATI_H2(RTYPE, in, idx0, idx1, out0, out1);       \
  SPLATI_H2(RTYPE, in, idx2, idx3, out2, out3);       \
}
#define SPLATI_H4_SB(...) SPLATI_H4(v16i8, __VA_ARGS__)
#define SPLATI_H4_SH(...) SPLATI_H4(v8i16, __VA_ARGS__)

/* Description : Pack even byte elements of vector pairs
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even byte elements of in0 are copied to the left half of
                 out0 & even byte elements of in1 are copied to the right
                 half of out0.
                 Even byte elements of in2 are copied to the left half of
                 out1 & even byte elements of in3 are copied to the right
                 half of out1.
*/
#define PCKEV_B2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE)__msa_pckev_b((v16i8)in0, (v16i8)in1);     \
  out1 = (RTYPE)__msa_pckev_b((v16i8)in2, (v16i8)in3);     \
}
#define PCKEV_B2_SB(...) PCKEV_B2(v16i8, __VA_ARGS__)
#define PCKEV_B2_UB(...) PCKEV_B2(v16u8, __VA_ARGS__)
#define PCKEV_B2_SH(...) PCKEV_B2(v8i16, __VA_ARGS__)

#define PCKEV_B4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3) {                       \
  PCKEV_B2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  PCKEV_B2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define PCKEV_B4_SB(...) PCKEV_B4(v16i8, __VA_ARGS__)
#define PCKEV_B4_UB(...) PCKEV_B4(v16u8, __VA_ARGS__)
#define PCKEV_B4_SH(...) PCKEV_B4(v8i16, __VA_ARGS__)

/* Description : Pack even halfword elements of vector pairs
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even halfword elements of in0 are copied to the left half of
                 out0 & even halfword elements of in1 are copied to the right
                 half of out0.
                 Even halfword elements of in2 are copied to the left half of
                 out1 & even halfword elements of in3 are copied to the right
                 half of out1.
*/
#define PCKEV_H2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE)__msa_pckev_h((v8i16)in0, (v8i16)in1);     \
  out1 = (RTYPE)__msa_pckev_h((v8i16)in2, (v8i16)in3);     \
}
#define PCKEV_H2_SH(...) PCKEV_H2(v8i16, __VA_ARGS__)
#define PCKEV_H2_SW(...) PCKEV_H2(v4i32, __VA_ARGS__)

#define PCKEV_H4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3) {                       \
  PCKEV_H2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  PCKEV_H2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define PCKEV_H4_SH(...) PCKEV_H4(v8i16, __VA_ARGS__)

/* Description : Pack even double word elements of vector pairs
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - unsigned byte
   Details     : Even double elements of in0 are copied to the left half of
                 out0 & even double elements of in1 are copied to the right
                 half of out0.
                 Even double elements of in2 are copied to the left half of
                 out1 & even double elements of in3 are copied to the right
                 half of out1.
*/
#define PCKEV_D2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE)__msa_pckev_d((v2i64)in0, (v2i64)in1);     \
  out1 = (RTYPE)__msa_pckev_d((v2i64)in2, (v2i64)in3);     \
}
#define PCKEV_D2_UB(...) PCKEV_D2(v16u8, __VA_ARGS__)
#define PCKEV_D2_SH(...) PCKEV_D2(v8i16, __VA_ARGS__)

#define PCKEV_D4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3) {                       \
  PCKEV_D2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  PCKEV_D2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define PCKEV_D4_UB(...) PCKEV_D4(v16u8, __VA_ARGS__)

/* Description : Each byte element is logically xor'ed with immediate 128
   Arguments   : Inputs  - in0, in1
                 Outputs - in0, in1 (in-place)
                 Return Type - as per RTYPE
   Details     : Each unsigned byte element from input vector 'in0' is
                 logically xor'ed with 128 and result is in-place stored in
                 'in0' vector
                 Each unsigned byte element from input vector 'in1' is
                 logically xor'ed with 128 and result is in-place stored in
                 'in1' vector
                 Similar for other pairs
*/
#define XORI_B2_128(RTYPE, in0, in1) {         \
  in0 = (RTYPE)__msa_xori_b((v16u8)in0, 128);  \
  in1 = (RTYPE)__msa_xori_b((v16u8)in1, 128);  \
}
#define XORI_B2_128_UB(...) XORI_B2_128(v16u8, __VA_ARGS__)
#define XORI_B2_128_SB(...) XORI_B2_128(v16i8, __VA_ARGS__)

#define XORI_B3_128(RTYPE, in0, in1, in2) {    \
  XORI_B2_128(RTYPE, in0, in1);                \
  in2 = (RTYPE)__msa_xori_b((v16u8)in2, 128);  \
}
#define XORI_B3_128_SB(...) XORI_B3_128(v16i8, __VA_ARGS__)

#define XORI_B4_128(RTYPE, in0, in1, in2, in3) {  \
  XORI_B2_128(RTYPE, in0, in1);                   \
  XORI_B2_128(RTYPE, in2, in3);                   \
}
#define XORI_B4_128_UB(...) XORI_B4_128(v16u8, __VA_ARGS__)
#define XORI_B4_128_SB(...) XORI_B4_128(v16i8, __VA_ARGS__)

#define XORI_B7_128(RTYPE, in0, in1, in2, in3, in4, in5, in6) {  \
  XORI_B4_128(RTYPE, in0, in1, in2, in3);                        \
  XORI_B3_128(RTYPE, in4, in5, in6);                             \
}
#define XORI_B7_128_SB(...) XORI_B7_128(v16i8, __VA_ARGS__)

/* Description : Addition of signed halfword elements and signed saturation
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Signed halfword elements from 'in0' are added to signed
                 halfword elements of 'in1'. The result is then signed saturated
                 between -32768 to +32767 (as per halfword data type)
                 Similar for other pairs
*/
#define ADDS_SH2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE)__msa_adds_s_h((v8i16)in0, (v8i16)in1);    \
  out1 = (RTYPE)__msa_adds_s_h((v8i16)in2, (v8i16)in3);    \
}
#define ADDS_SH2_SH(...) ADDS_SH2(v8i16, __VA_ARGS__)

#define ADDS_SH4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3) {                       \
  ADDS_SH2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  ADDS_SH2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define ADDS_SH4_SH(...) ADDS_SH4(v8i16, __VA_ARGS__)

/* Description : Shift right arithmetic rounded (immediate)
   Arguments   : Inputs  - in0, in1, in2, in3, shift
                 Outputs - in0, in1, in2, in3 (in place)
                 Return Type - as per RTYPE
   Details     : Each element of vector 'in0' is shifted right arithmetic by
                 value in 'shift'.
                 The last discarded bit is added to shifted value for rounding
                 and the result is in place written to 'in0'
                 Similar for other pairs
*/
#define SRARI_H2(RTYPE, in0, in1, shift) {        \
  in0 = (RTYPE)__msa_srari_h((v8i16)in0, shift);  \
  in1 = (RTYPE)__msa_srari_h((v8i16)in1, shift);  \
}
#define SRARI_H2_UH(...) SRARI_H2(v8u16, __VA_ARGS__)
#define SRARI_H2_SH(...) SRARI_H2(v8i16, __VA_ARGS__)

#define SRARI_H4(RTYPE, in0, in1, in2, in3, shift) {  \
  SRARI_H2(RTYPE, in0, in1, shift);                   \
  SRARI_H2(RTYPE, in2, in3, shift);                   \
}
#define SRARI_H4_UH(...) SRARI_H4(v8u16, __VA_ARGS__)
#define SRARI_H4_SH(...) SRARI_H4(v8i16, __VA_ARGS__)

/* Description : Shift right arithmetic rounded (immediate)
   Arguments   : Inputs  - in0, in1, shift
                 Outputs - in0, in1     (in place)
                 Return Type - as per RTYPE
   Details     : Each element of vector 'in0' is shifted right arithmetic by
                 value in 'shift'.
                 The last discarded bit is added to shifted value for rounding
                 and the result is in place written to 'in0'
                 Similar for other pairs
*/
#define SRARI_W2(RTYPE, in0, in1, shift) {        \
  in0 = (RTYPE)__msa_srari_w((v4i32)in0, shift);  \
  in1 = (RTYPE)__msa_srari_w((v4i32)in1, shift);  \
}
#define SRARI_W2_SW(...) SRARI_W2(v4i32, __VA_ARGS__)

#define SRARI_W4(RTYPE, in0, in1, in2, in3, shift) {  \
  SRARI_W2(RTYPE, in0, in1, shift);                   \
  SRARI_W2(RTYPE, in2, in3, shift);                   \
}
#define SRARI_W4_SW(...) SRARI_W4(v4i32, __VA_ARGS__)

/* Description : Addition of 2 pairs of vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
   Details     : Each element from 2 pairs vectors is added and 2 results are
                 produced
*/
#define ADD2(in0, in1, in2, in3, out0, out1) {  \
  out0 = in0 + in1;                             \
  out1 = in2 + in3;                             \
}
#define ADD4(in0, in1, in2, in3, in4, in5, in6, in7,  \
             out0, out1, out2, out3) {                \
  ADD2(in0, in1, in2, in3, out0, out1);               \
  ADD2(in4, in5, in6, in7, out2, out3);               \
}

/* Description : Subtraction of 2 pairs of vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
   Details     : Each element from 2 pairs vectors is subtracted and 2 results
                 are produced
*/
#define SUB2(in0, in1, in2, in3, out0, out1) {  \
  out0 = in0 - in1;                             \
  out1 = in2 - in3;                             \
}
#define SUB4(in0, in1, in2, in3, in4, in5, in6, in7,  \
             out0, out1, out2, out3) {                \
  out0 = in0 - in1;                                   \
  out1 = in2 - in3;                                   \
  out2 = in4 - in5;                                   \
  out3 = in6 - in7;                                   \
}

/* Description : Zero extend unsigned byte elements to halfword elements
   Arguments   : Inputs  - in           (1 input unsigned byte vector)
                 Outputs - out0, out1   (unsigned 2 halfword vectors)
                 Return Type - signed halfword
   Details     : Zero extended right half of vector is returned in 'out0'
                 Zero extended left half of vector is returned in 'out1'
*/
#define UNPCK_UB_SH(in, out0, out1) {   \
  v16i8 zero_m = { 0 };                 \
                                        \
  ILVRL_B2_SH(zero_m, in, out0, out1);  \
}

/* Description : Butterfly of 4 input vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1, out2, out3
   Details     : Butterfly operation
*/
#define BUTTERFLY_4(in0, in1, in2, in3, out0, out1, out2, out3) {  \
  out0 = in0 + in3;                                                \
  out1 = in1 + in2;                                                \
                                                                   \
  out2 = in1 - in2;                                                \
  out3 = in0 - in3;                                                \
}

/* Description : Butterfly of 8 input vectors
   Arguments   : Inputs  - in0 ...  in7
                 Outputs - out0 .. out7
   Details     : Butterfly operation
*/
#define BUTTERFLY_8(in0, in1, in2, in3, in4, in5, in6, in7,            \
                    out0, out1, out2, out3, out4, out5, out6, out7) {  \
  out0 = in0 + in7;                                                    \
  out1 = in1 + in6;                                                    \
  out2 = in2 + in5;                                                    \
  out3 = in3 + in4;                                                    \
                                                                       \
  out4 = in3 - in4;                                                    \
  out5 = in2 - in5;                                                    \
  out6 = in1 - in6;                                                    \
  out7 = in0 - in7;                                                    \
}

/* Description : Transposes 4x8 block with half word elements in vectors
   Arguments   : Inputs  - in0, in1, in2, in3, in4, in5, in6, in7
                 Outputs - out0, out1, out2, out3, out4, out5, out6, out7
                 Return Type - signed halfword
   Details     :
*/
#define TRANSPOSE4X8_SH_SH(in0, in1, in2, in3, in4, in5, in6, in7,            \
                           out0, out1, out2, out3, out4, out5, out6, out7) {  \
  v8i16 tmp0_m, tmp1_m, tmp2_m, tmp3_m;                                       \
  v8i16 tmp0_n, tmp1_n, tmp2_n, tmp3_n;                                       \
  v8i16 zero_m = { 0 };                                                       \
                                                                              \
  ILVR_H4_SH(in1, in0, in3, in2, in5, in4, in7, in6,                          \
             tmp0_n, tmp1_n, tmp2_n, tmp3_n);                                 \
  ILVRL_W2_SH(tmp1_n, tmp0_n, tmp0_m, tmp2_m);                                \
  ILVRL_W2_SH(tmp3_n, tmp2_n, tmp1_m, tmp3_m);                                \
                                                                              \
  out0 = (v8i16)__msa_ilvr_d((v2i64)tmp1_m, (v2i64)tmp0_m);                   \
  out1 = (v8i16)__msa_ilvl_d((v2i64)tmp1_m, (v2i64)tmp0_m);                   \
  out2 = (v8i16)__msa_ilvr_d((v2i64)tmp3_m, (v2i64)tmp2_m);                   \
  out3 = (v8i16)__msa_ilvl_d((v2i64)tmp3_m, (v2i64)tmp2_m);                   \
                                                                              \
  out4 = zero_m;                                                              \
  out5 = zero_m;                                                              \
  out6 = zero_m;                                                              \
  out7 = zero_m;                                                              \
}

/* Description : Transposes 8x4 block with half word elements in vectors
   Arguments   : Inputs  - in0, in1, in2, in3, in4, in5, in6, in7
                 Outputs - out0, out1, out2, out3, out4, out5, out6, out7
                 Return Type - signed halfword
   Details     :
*/
#define TRANSPOSE8X4_SH_SH(in0, in1, in2, in3, out0, out1, out2, out3) {  \
  v8i16 tmp0_m, tmp1_m, tmp2_m, tmp3_m;                                   \
                                                                          \
  ILVR_H2_SH(in1, in0, in3, in2, tmp0_m, tmp1_m);                         \
  ILVL_H2_SH(in1, in0, in3, in2, tmp2_m, tmp3_m);                         \
  ILVR_W2_SH(tmp1_m, tmp0_m, tmp3_m, tmp2_m, out0, out2);                 \
  ILVL_W2_SH(tmp1_m, tmp0_m, tmp3_m, tmp2_m, out1, out3);                 \
}

/* Description : Transposes 8x8 block with half word elements in vectors
   Arguments   : Inputs  - in0, in1, in2, in3, in4, in5, in6, in7
                 Outputs - out0, out1, out2, out3, out4, out5, out6, out7
                 Return Type - signed halfword
   Details     :
*/
#define TRANSPOSE8x8_H(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,     \
                       out0, out1, out2, out3, out4, out5, out6, out7) {  \
  v8i16 s0_m, s1_m;                                                       \
  v8i16 tmp0_m, tmp1_m, tmp2_m, tmp3_m;                                   \
  v8i16 tmp4_m, tmp5_m, tmp6_m, tmp7_m;                                   \
                                                                          \
  ILVR_H2_SH(in6, in4, in7, in5, s0_m, s1_m);                             \
  ILVRL_H2_SH(s1_m, s0_m, tmp0_m, tmp1_m);                                \
  ILVL_H2_SH(in6, in4, in7, in5, s0_m, s1_m);                             \
  ILVRL_H2_SH(s1_m, s0_m, tmp2_m, tmp3_m);                                \
  ILVR_H2_SH(in2, in0, in3, in1, s0_m, s1_m);                             \
  ILVRL_H2_SH(s1_m, s0_m, tmp4_m, tmp5_m);                                \
  ILVL_H2_SH(in2, in0, in3, in1, s0_m, s1_m);                             \
  ILVRL_H2_SH(s1_m, s0_m, tmp6_m, tmp7_m);                                \
  PCKEV_D4(RTYPE, tmp0_m, tmp4_m, tmp1_m, tmp5_m, tmp2_m, tmp6_m,         \
           tmp3_m, tmp7_m, out0, out2, out4, out6);                       \
  out1 = (RTYPE)__msa_pckod_d((v2i64)tmp0_m, (v2i64)tmp4_m);              \
  out3 = (RTYPE)__msa_pckod_d((v2i64)tmp1_m, (v2i64)tmp5_m);              \
  out5 = (RTYPE)__msa_pckod_d((v2i64)tmp2_m, (v2i64)tmp6_m);              \
  out7 = (RTYPE)__msa_pckod_d((v2i64)tmp3_m, (v2i64)tmp7_m);              \
}
#define TRANSPOSE8x8_SH_SH(...) TRANSPOSE8x8_H(v8i16, __VA_ARGS__)

/* Description : Pack even elements of input vectors & xor with 128
   Arguments   : Inputs  - in0, in1
                 Outputs - out_m
                 Return Type - unsigned byte
   Details     : Signed byte even elements from 'in0' and 'in1' are packed
                 together in one vector and the resulted vector is xor'ed with
                 128 to shift the range from signed to unsigned byte
*/
#define PCKEV_XORI128_UB(in0, in1) ({                    \
  v16u8 out_m;                                           \
                                                         \
  out_m = (v16u8)__msa_pckev_b((v16i8)in1, (v16i8)in0);  \
  out_m = (v16u8)__msa_xori_b((v16u8)out_m, 128);        \
  out_m;                                                 \
})

/* Description : Pack even byte elements and store byte vector in destination
                 memory
   Arguments   : Inputs  - in0, in1, pdst
*/
#define PCKEV_ST_SB(in0, in1, pdst) {             \
  v16i8 tmp_m;                                    \
                                                  \
  tmp_m = __msa_pckev_b((v16i8)in1, (v16i8)in0);  \
  ST_SB(tmp_m, (pdst));                           \
}

/* Description : Horizontal 2 tap filter kernel code
   Arguments   : Inputs  - in0, in1, mask, coeff, shift
*/
#define HORIZ_2TAP_FILT_UH(in0, in1, mask, coeff, shift) ({    \
  v16i8 tmp0_m;                                                \
  v8u16 tmp1_m;                                                \
                                                               \
  tmp0_m = __msa_vshf_b((v16i8)mask, (v16i8)in1, (v16i8)in0);  \
  tmp1_m = __msa_dotp_u_h((v16u8)tmp0_m, (v16u8)coeff);        \
  tmp1_m = (v8u16)__msa_srari_h((v8i16)tmp1_m, shift);         \
  tmp1_m = __msa_sat_u_h(tmp1_m, shift);                       \
                                                               \
  tmp1_m;                                                      \
})
#endif  /* VP9_COMMON_MIPS_MSA_VP9_MACROS_MSA_H_ */
