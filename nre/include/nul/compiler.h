/*
 * Copyright (C) 2013, Nils Asmussen <nils@os.inf.tu-dresden.de>
 * Copyright (C) 2010-2011, Julian Stecklina <jsteckli@os.inf.tu-dresden.de>
 * Economic rights: Technische Universitaet Dresden (Germany)
 *
 * This file is part of NRE (NOVA runtime environment).
 *
 * NRE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * NRE is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details.
 */

#pragma once

#include <Compiler.h>

#if !defined(__GNUC__)
#error Your platform is not supported.
#endif

#define REGPARM(x) __attribute__((regparm(x)))
#define PURE       __attribute__((pure))
#define COLD       __attribute__((cold))
#define MEMORY_BARRIER __asm__ __volatile__ ("" ::: "memory")
#define RESTRICT   __restrict__

/* XXX Enable this and knock yourself out... */
//#define DEPRECATED __attribute__((deprecated))
#define DEPRECATED

#ifdef __cplusplus
# define BEGIN_EXTERN_C extern "C" {
# define END_EXTERN_C   }
#else
# define BEGIN_EXTERN_C
# define END_EXTERN_C
#endif

#define MAX(a, b) ({ decltype (a) _a = (a); \
      decltype (b) _b = (b);		  \
      _a > _b ? _a : _b; })

#define MIN(a, b) ({ decltype (a) _a = (a); \
      decltype (b) _b = (b);		  \
      _a > _b ? _b : _a; })
