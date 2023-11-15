/**
 * \file no_opt.h
 * \brief Includes a define to specify that a function or variable should not be optimized
 */

#if defined(__ARMCC_VERSION)
#define NO_OPT __attribute__((__used__))
#elif defined(__ICCARM__)
#define NO_OPT __root
#elif defined(__GNUC__)
#define NO_OPT __attribute__((__used__))
#else
#define NO_OPT
#error "Unsupported compiler"
#endif
