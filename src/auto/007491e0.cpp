#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Debug-heap allocator used by the original (cdecl, caller pops).
CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0, (std::size_t size, int flags, int tag));

// Debug-heap free used on the unwind path (cdecl, caller pops).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Second-stage initializer: __fastcall with ecx = new object, 3 stack args, callee pops 12.
CSF_FUNC(void*, __fastcall, InitPairAndForwardSubobject, 0x007491B0, (void* self, void* /*edx*/, int a2, int a3, int a4));

// C++ rethrow (__CxxThrowException with null object = rethrow current exception).
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

// Allocates a 0x48-byte block from the debug heap and, when the allocation
// succeeds, forwards the three stack arguments to the subobject initializer.
// If the initializer throws, the block is released and the exception is rethrown.
// Returns the new block (or 0 when the allocation failed).
static int __stdcall AllocateAndInitPairObject(int a1, int a2, int a3)
{
    int allocatedBlock = Mem_AllocDebug(0x48u, 0, 0);
    try
    {
        if (allocatedBlock)
        {
            InitPairAndForwardSubobject(reinterpret_cast<void*>(allocatedBlock), nullptr, a1, a2, a3);
        }
    }
    catch (...)
    {
        if (allocatedBlock)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(allocatedBlock));
        }
        RethrowCppException(nullptr, nullptr);
    }
    return allocatedBlock;
}

HOOK(0x7491E0, AllocateAndInitPairObject, "auto");
