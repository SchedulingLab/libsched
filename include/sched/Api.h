// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_API_H
#define SCHED_API_H

#if defined _WIN32 || defined __CYGWIN__
#  ifdef __GNUC__
#    define SCHED_EXPORT __attribute__((dllexport))
#    define SCHED_IMPORT __attribute__((dllimport))
#  else
#    define SCHED_EXPORT __declspec(dllexport)
#    define SCHED_IMPORT __declspec(dllimport)
#  endif
#else
#  if __GNUC__ >= 4
#    define SCHED_EXPORT __attribute__((visibility("default")))
#    define SCHED_IMPORT __attribute__((visibility("default")))
#  else
#    define SCHED_EXPORT
#    define SCHED_IMPORT
#  endif
#endif

#ifndef SCHED_STATIC
#  ifdef SCHED_BUILD
#    define SCHED_API SCHED_EXPORT
#  else
#    define SCHED_API SCHED_IMPORT
#  endif
#else
#  define SCHED_API
#endif

#endif // SCHED_API_H
