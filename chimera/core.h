// Copyright 2017 Dan Ristic

#ifndef CHIMERA_CORE_H_
#define CHIMERA_CORE_H_

#define CHIMERA_UNUSED(expr) (void)(expr)

#ifdef NDEBUG
    #define CHIMERA_DEBUG(x)
#else
    #define CHIMERA_DEBUG(x) x
#endif

#ifdef NDEBUG
    #define CHIMERA_TIME(x)
    #define CHIMERA_TIME_END(x)
#else
    #include <iostream>
    #include <chrono>
    #define CHIMERA_TIME(x) \
        std::chrono::steady_clock::time_point begin##x = \
            std::chrono::steady_clock::now();
    #define CHIMERA_TIME_END(x) \
        std::cout << "Time " #x " " << \
            std::chrono::duration_cast<std::chrono::milliseconds>( \
                std::chrono::steady_clock::now() - begin##x \
            ).count() << "ms" << std::endl;
#endif

#endif  // CHIMERA_CORE_H_