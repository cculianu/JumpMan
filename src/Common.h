#pragma once

// IS_IOS detection
#if __has_include(<TargetConditionals.h>) // macOS, iOS, etc
#  include <TargetConditionals.h>
#  if (defined(TARGET_OS_SIMULATOR) && TARGET_OS_SIMULATOR)  || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE)
#    define IS_IOS true
#  endif
#endif
#ifndef IS_IOS
#  define IS_IOS false
#endif

inline constexpr double PHYSICS_RATE = 1000.0 / 24.0; /* internal physics originally assumed this framerate */
