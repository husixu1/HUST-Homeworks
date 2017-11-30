#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TMin INT_MIN
#define TMax INT_MAX

#include "btest.h"
#include "bits.h"

test_rec test_set[] = {
/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
 {"lsbZero", (funct_t) lsbZero, (funct_t) test_lsbZero, 1,
    "! ~ & ^ | + << >>", 5, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"byteNot", (funct_t) byteNot, (funct_t) test_byteNot, 2,
    "! ~ & ^ | + << >>", 6, 2,
  {{TMin, TMax},{0,3},{TMin,TMax}}},
 {"byteXor", (funct_t) byteXor, (funct_t) test_byteXor,
   3, "! ~ & ^ | + << >>", 20, 2,
  {{TMin, TMax},{TMin,TMax},{0,3}}},
 {"logicalAnd", (funct_t) logicalAnd, (funct_t) test_logicalAnd,
   2, "! ~ & ^ | + << >>", 20, 3,
  {{TMin, TMax},{TMin, TMax},{TMin,TMax}}},
 {"logicalOr", (funct_t) logicalOr, (funct_t) test_logicalOr,
   2, "! ~ & ^ | + << >>", 20, 3,
  {{TMin, TMax},{TMin, TMax},{TMin,TMax}}},
 {"rotateLeft", (funct_t) rotateLeft, (funct_t) test_rotateLeft,
   2, "! ~ & ^ | + << >>", 25, 3,
  {{TMin, TMax},{0,31},{TMin,TMax}}},
 {"parityCheck", (funct_t) parityCheck, (funct_t) test_parityCheck, 1, "! ~ & ^ | + << >>", 20, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"mul2OK", (funct_t) mul2OK, (funct_t) test_mul2OK, 1,
    "~ & ^ | + << >>", 20, 2,
  {{TMin,TMax},{TMin,TMax},{TMin,TMax}}},
 {"mult3div2", (funct_t) mult3div2, (funct_t) test_mult3div2, 1,
    "! ~ & ^ | + << >>", 12, 2,
  {{TMin,TMax},{TMin,TMax},{TMin,TMax}}},
 {"subOK", (funct_t) subOK, (funct_t) test_subOK, 2,
    "! ~ & ^ | + << >>", 20, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"absVal", (funct_t) absVal, (funct_t) test_absVal, 1, "! ~ & ^ | + << >>", 10, 4,
  {{-TMax, TMax},{TMin,TMax},{TMin,TMax}}},
 {"float_abs", (funct_t) float_abs, (funct_t) test_float_abs, 1,
    "$", 10, 2,
     {{1, 1},{1,1},{1,1}}},
 {"float_f2i", (funct_t) float_f2i, (funct_t) test_float_f2i, 1,
    "$", 30, 4,
     {{1, 1},{1,1},{1,1}}},
  {"", NULL, NULL, 0, "", 0, 0,
   {{0, 0},{0,0},{0,0}}}
};
