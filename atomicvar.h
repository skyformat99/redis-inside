/* This file implements atomic counters using __atomic or __sync macros if
 * available, otherwise synchronizing different threads using a mutex.
 *
 * The exported interaface is composed of three macros:
 *
 * atomicIncr(var,count) -- Increment the atomic counter
 * atomicGetIncr(var,oldvalue_var,count) -- Get and increment the atomic counter
 * atomicDecr(var,count) -- Decrement the atomic counter
 * atomicGet(var,dstvar) -- Fetch the atomic counter value
 * atomicSet(var,value)  -- Set the atomic counter value
 *
 * The variable 'var' should also have a declared mutex with the same
 * name and the "_mutex" postfix, for instance:
 *
 *  long myvar;
 *  pthread_mutex_t myvar_mutex;
 *  atomicSet(myvar,12345);
 *
 * If atomic primitives are availble (tested in config.h) the mutex
 * is not used.
 *
 * Never use return value from the macros, instead use the AtomicGetIncr()
 * if you need to get the current value and increment it atomically, like
 * in the followign example:
 *
 *  long oldvalue;
 *  atomicGetIncr(myvar,oldvalue,1);
 *  doSomethingWith(oldvalue);
 */

#include <pthread.h>

#ifndef __ATOMIC_VAR_H
#define __ATOMIC_VAR_H

/* To test Redis with Helgrind (a Valgrind tool) it is useful to define
 * the following macro, so that __sync macros are used: those can be detected
 * by Helgrind (even if they are less efficient) so that no false positive
 * is reported. */
// #define __ATOMIC_VAR_FORCE_SYNC_MACROS

#if !defined(__ATOMIC_VAR_FORCE_SYNC_MACROS) && defined(__ATOMIC_RELAXED) && !defined(__sun) && (!defined(__clang__) || !defined(__APPLE__) || __apple_build_version__ > 4210057)
/* Implementation using __atomic macros. */

#define atomicIncr(var,count) __atomic_add_fetch(&var,(count),__ATOMIC_RELAXED)
#define atomicGetIncr(var,oldvalue_var,count) do { \
    oldvalue_var = __atomic_fetch_add(&var,(count),__ATOMIC_RELAXED); \
} while(0)
#define atomicDecr(var,count) __atomic_sub_fetch(&var,(count),__ATOMIC_RELAXED)
#define atomicGet(var,dstvar) do { \
    dstvar = __atomic_load_n(&var,__ATOMIC_RELAXED); \
} while(0)
#define atomicSet(var,value) __atomic_store_n(&var,value,__ATOMIC_RELAXED)
#define REDIS_ATOMIC_API "atomic-builtin"

#elif defined(HAVE_ATOMIC)
/* Implementation using __sync macros. */

#define atomicIncr(var,count) __sync_add_and_fetch(&var,(count))
#define atomicGetIncr(var,oldvalue_var,count) do { \
    oldvalue_var = __sync_fetch_and_add(&var,(count)); \
} while(0)
#define atomicDecr(var,count) __sync_sub_and_fetch(&var,(count))
#define atomicGet(var,dstvar) do { \
    dstvar = __sync_sub_and_fetch(&var,0); \
} while(0)
#define atomicSet(var,value) do { \
    while(!__sync_bool_compare_and_swap(&var,var,value)); \
} while(0)
#define REDIS_ATOMIC_API "sync-builtin"

#else
/* Implementation using pthread mutex. */

#define atomicIncr(var,count) do { \
    pthread_mutex_lock(&var ## _mutex); \
    var += (count); \
    pthread_mutex_unlock(&var ## _mutex); \
} while(0)
#define atomicGetIncr(var,oldvalue_var,count) do { \
    pthread_mutex_lock(&var ## _mutex); \
    oldvalue_var = var; \
    var += (count); \
    pthread_mutex_unlock(&var ## _mutex); \
} while(0)
#define atomicDecr(var,count) do { \
    pthread_mutex_lock(&var ## _mutex); \
    var -= (count); \
    pthread_mutex_unlock(&var ## _mutex); \
} while(0)
#define atomicGet(var,dstvar) do { \
    pthread_mutex_lock(&var ## _mutex); \
    dstvar = var; \
    pthread_mutex_unlock(&var ## _mutex); \
} while(0)
#define atomicSet(var,value) do { \
    pthread_mutex_lock(&var ## _mutex); \
    var = value; \
    pthread_mutex_unlock(&var ## _mutex); \
} while(0)
#define REDIS_ATOMIC_API "pthread-mutex"

#endif
#endif /* __ATOMIC_VAR_H */
