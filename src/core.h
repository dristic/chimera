// Copyright 2017 Dan Ristic

#ifndef SRC_CORE_H_
#define SRC_CORE_H_

#define CHIMERA_UNUSED(expr) (void)(expr)

#ifdef NDEBUG
    #define CHIMERA_DEBUG(x)
#else
    #define CHIMERA_DEBUG(x) x
#endif

#endif  // SRC_CORE_H_