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

// Callee: releases the object held in a script node's payload slot (its vtable slot 0 is the destructor).
CSF_FUNC(int, __cdecl, MissionScript_ReleaseNode, 0x004714E0, (void* Block));

// Game heap free.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Virtual destructor signature: __thiscall (ecx=this, edx unused), one int argument.
typedef void(__fastcall* ScriptObjectDtorFn)(void* self, void* edx, int flags);

// Walks the 8-byte entries in [a1, a2). For each entry whose +4 pointer is set, destroys the
// object at that pointer's +4, frees the pointer's block, and clears both links.
static int __cdecl MissionScript_FreeEntryList(int a1, int a2, int a3, int a4)
{
    int result = a2;
    uint32_t entry = static_cast<uint32_t>(a1);
    const uint32_t end = static_cast<uint32_t>(a2);

    for (; entry != end; entry += 8)
    {
        uint32_t holder = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(entry)), 4);
        if (holder)
        {
            void* holderPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(holder));
            void* object = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, holderPtr, 4)));
            if (object)
            {
                ScriptObjectDtorFn destroy = reinterpret_cast<ScriptObjectDtorFn>(CSF_AT(void**, object, 0)[0]);
                destroy(object, nullptr, 0);
                MissionScript_ReleaseNode(object);
            }
            CSF_AT(uint32_t, holderPtr, 4) = 0;
            Mem_Free(holderPtr);
            CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(entry)), 4) = 0;
        }
        result = a2;
    }
    return result;
}

HOOK(0x0046C620, MissionScript_FreeEntryList, "auto");
