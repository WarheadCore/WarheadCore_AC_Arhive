/*
 * Copyright (C) 2008-2019 TrinityCore <https://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WARHEAD_DEFINE_H
#define WARHEAD_DEFINE_H

#include "CompilerDefs.h"

#if WARHEAD_COMPILER == WARHEAD_COMPILER_GNU
#  if !defined(__STDC_FORMAT_MACROS)
#    define __STDC_FORMAT_MACROS
#  endif
#  if !defined(__STDC_CONSTANT_MACROS)
#    define __STDC_CONSTANT_MACROS
#  endif
#  if !defined(_GLIBCXX_USE_NANOSLEEP)
#    define _GLIBCXX_USE_NANOSLEEP
#  endif
#  if defined(HELGRIND)
#    include <valgrind/helgrind.h>
#    undef _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE
#    undef _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER
#    define _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(A) ANNOTATE_HAPPENS_BEFORE(A)
#    define _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(A)  ANNOTATE_HAPPENS_AFTER(A)
#  endif
#endif

#include <cstddef>
#include <cinttypes>
#include <climits>

#define WARHEAD_LITTLEENDIAN 0
#define WARHEAD_BIGENDIAN    1

#if !defined(WARHEAD_ENDIAN)
#  if defined (BOOST_BIG_ENDIAN)
#    define WARHEAD_ENDIAN WARHEAD_BIGENDIAN
#  else
#    define WARHEAD_ENDIAN WARHEAD_LITTLEENDIAN
#  endif
#endif

#if WARHEAD_PLATFORM == WARHEAD_PLATFORM_WINDOWS
#  define WARHEAD_PATH_MAX 260
#  define _USE_MATH_DEFINES
#  ifndef DECLSPEC_NORETURN
#    define DECLSPEC_NORETURN __declspec(noreturn)
#  endif //DECLSPEC_NORETURN
#  ifndef DECLSPEC_DEPRECATED
#    define DECLSPEC_DEPRECATED __declspec(deprecated)
#  endif //DECLSPEC_DEPRECATED
#else // WARHEAD_PLATFORM != WARHEAD_PLATFORM_WINDOWS
#  define WARHEAD_PATH_MAX PATH_MAX
#  define DECLSPEC_NORETURN
#  define DECLSPEC_DEPRECATED
#endif // WARHEAD_PLATFORM

#if !defined(COREDEBUG)
#  define WARHEAD_INLINE inline
#else //COREDEBUG
#  if !defined(WARHEAD_DEBUG)
#    define WARHEAD_DEBUG
#  endif //WARHEAD_DEBUG
#  define WARHEAD_INLINE
#endif //!COREDEBUG

#if WARHEAD_COMPILER == WARHEAD_COMPILER_GNU
#  define ATTR_NORETURN __attribute__((__noreturn__))
#  define ATTR_PRINTF(F, V) __attribute__ ((__format__ (__printf__, F, V)))
#  define ATTR_DEPRECATED __attribute__((__deprecated__))
#else //WARHEAD_COMPILER != WARHEAD_COMPILER_GNU
#  define ATTR_NORETURN
#  define ATTR_PRINTF(F, V)
#  define ATTR_DEPRECATED
#endif //WARHEAD_COMPILER == WARHEAD_COMPILER_GNU

#ifdef WARHEAD_API_USE_DYNAMIC_LINKING
#  if WARHEAD_COMPILER == WARHEAD_COMPILER_MICROSOFT
#    define WC_API_EXPORT __declspec(dllexport)
#    define WC_API_IMPORT __declspec(dllimport)
#  elif WARHEAD_COMPILER == WARHEAD_COMPILER_GNU
#    define WC_API_EXPORT __attribute__((visibility("default")))
#    define WC_API_IMPORT
#  else
#    error compiler not supported!
#  endif
#else
#  define WC_API_EXPORT
#  define WC_API_IMPORT
#endif

#ifdef WARHEAD_API_EXPORT_COMMON
#  define WC_COMMON_API WC_API_EXPORT
#else
#  define WC_COMMON_API WC_API_IMPORT
#endif

#ifdef WARHEAD_API_EXPORT_DATABASE
#  define WC_DATABASE_API WC_API_EXPORT
#else
#  define WC_DATABASE_API WC_API_IMPORT
#endif

#ifdef WARHEAD_API_EXPORT_SHARED
#  define WC_SHARED_API WC_API_EXPORT
#else
#  define WC_SHARED_API WC_API_IMPORT
#endif

#ifdef WARHEAD_API_EXPORT_GAME
#  define WC_GAME_API WC_API_EXPORT
#else
#  define WC_GAME_API WC_API_IMPORT
#endif

#define UI64FMTD "%" PRIu64
#define UI64LIT(N) UINT64_C(N)

#define SI64FMTD "%" PRId64
#define SI64LIT(N) INT64_C(N)

#define SZFMTD "%" PRIuPTR

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;

#endif //WARHEAD_DEFINE_H
