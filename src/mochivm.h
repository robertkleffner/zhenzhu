#ifndef mochivm_h
#define mochivm_h

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// The MochiVM semantic version number components.
#define MOCHIVM_VERSION_MAJOR 0
#define MOCHIVM_VERSION_MINOR 1
#define MOCHIVM_VERSION_PATCH 0

// A human-friendly string representation of the version.
#define MOCHIVM_VERSION_STRING "0.1.0"

// A monotonically increasing numeric representation of the version number. Use
// this if you want to do range checks over versions.
#define MOCHIVM_VERSION_NUMBER (MOCHIVM_VERSION_MAJOR * 1000000 + MOCHIVM_VERSION_MINOR * 1000 + MOCHIVM_VERSION_PATCH)

#ifndef MOCHIVM_API
#if defined(_MSC_VER) && defined(MOCHIVM_API_DLLEXPORT)
#define MOCHIVM_API __declspec(dllexport)
#else
#define MOCHIVM_API
#endif
#endif

// A single virtual machine for executing MochiVM byte code.
//
// MochiVM has no global state, so all state stored by a running interpreter lives
// here.
typedef struct MochiVM MochiVM;
typedef uint64_t TableKey;
typedef struct ObjFiber ObjFiber;

// A generic allocation function that handles all explicit memory management
// used by MochiVM. It's used like so:
//
// - To allocate new memory, [memory] is NULL and [newSize] is the desired
//   size. It should return the allocated memory or NULL on failure.
//
// - To attempt to grow an existing allocation, [memory] is the memory, and
//   [newSize] is the desired size. It should return [memory] if it was able to
//   grow it in place, or a new pointer if it had to move it.
//
// - To shrink memory, [memory] and [newSize] are the same as above but it will
//   always return [memory].
//
// - To free memory, [memory] will be the memory to free and [newSize] will be
//   zero. It should return NULL.
typedef void* (*MochiVMReallocateFn)(void* memory, size_t newSize, void* userData);

// A function callable from MochiVM code, but implemented in C. Passes in the fiber
// the function was called from.
typedef void (*MochiVMForeignMethodFn)(MochiVM* vm, ObjFiber* fiber);

// Reports an error to the user.
//
// A runtime error is reported by calling this once with no [module] or [line], and the runtime error's
// [message]. After that, a series of calls are made for each line in the stack trace. Each of those has the resolved
// [module] and [line] where the method or function is defined and [message] is
// the name of the method or function.
typedef void (*MochiVMErrorFn)(MochiVM* vm, const char* module, int line, const char* message);

typedef struct {
    // The callback MochiVM will use to allocate, reallocate, and deallocate memory.
    //
    // If `NULL`, defaults to a built-in function that uses `realloc` and `free`.
    MochiVMReallocateFn reallocateFn;

    // The callback MochiVM uses to report errors.
    //
    // When an error occurs, this will be called with the module name, line
    // number, and an error message. If this is `NULL`, MochiVM doesn't report any
    // errors.
    MochiVMErrorFn errorFn;

    // The maximum number of values the VM will allow in a fiber's value stack.
    // If zero, defaults to 128.
    int valueStackCapacity;

    // The maximum number of frames the VM will allow in a fiber's frame stack.
    // If zero, defaults to 512.
    int frameStackCapacity;

    // The maximum number of objects the VM will allow in a fiber's root stack.
    // If zero, defaults to 16.
    int rootStackCapacity;

    // The number of bytes MochiVM will allocate before triggering the first garbage
    // collection.
    //
    // If zero, defaults to 10MB.
    size_t initialHeapSize;

    // After a collection occurs, the threshold for the next collection is
    // determined based on the number of bytes remaining in use. This allows MochiVM
    // to shrink its memory usage automatically after reclaiming a large amount
    // of memory.
    //
    // This can be used to ensure that the heap does not get too small, which can
    // in turn lead to a large number of collections afterwards as the heap grows
    // back to a usable size.
    //
    // If zero, defaults to 1MB.
    size_t minHeapSize;

    // MochiVM will resize the heap automatically as the number of bytes
    // remaining in use after a collection changes. This number determines the
    // amount of additional memory MochiVM will use after a collection, as a
    // percentage of the current heap size.
    //
    // For example, say that this is 50. After a garbage collection, when there
    // are 400 bytes of memory still in use, the next collection will be triggered
    // after a total of 600 bytes are allocated (including the 400 already in
    // use.)
    //
    // Setting this to a smaller number wastes less memory, but triggers more
    // frequent garbage collections.
    //
    // If zero, defaults to 50.
    int heapGrowthPercent;

    // User-defined data associated with the VM.
    void* userData;

} MochiVMConfiguration;

typedef enum
{
    MOCHIVM_RESULT_SUCCESS,
    MOCHIVM_RESULT_RUNTIME_ERROR
} MochiVMInterpretResult;

// Get the current MochiVM version number.
//
// Can be used to range checks over versions.
MOCHIVM_API int mochiGetVersionNumber();

// Initializes [configuration] with all of its default values.
//
// Call this before setting the particular fields you care about.
MOCHIVM_API void mochiInitConfiguration(MochiVMConfiguration* configuration);

// Creates a new MochiVM virtual machine using the given [configuration]. MochiVM
// will copy the configuration data, so the argument passed to this can be
// freed after calling this. If [configuration] is `NULL`, uses a default
// configuration.
MOCHIVM_API MochiVM* mochiNewVM(MochiVMConfiguration* configuration);

// Disposes of all resources is use by [vm], which was previously created by a
// call to [mochiNewVM].
MOCHIVM_API void mochiFreeVM(MochiVM* vm);

// Immediately run the garbage collector to free unused memory.
MOCHIVM_API void mochiCollectGarbage(MochiVM* vm);

// Runs a fiber as the root in the context of the given VM.
MOCHIVM_API MochiVMInterpretResult mochiInterpret(MochiVM* vm, ObjFiber* fiber);

// Add a foreign C function to the list of callable foreign methods, returning
// the index assigned to the foreign method.
MOCHIVM_API int mochiAddForeign(MochiVM* vm, MochiVMForeignMethodFn fn);

#endif