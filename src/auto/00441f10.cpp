#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original game globals and callees, bound by address.
CSF_GLOBAL(int32_t, g_NextUniqueId, 0x008A669C);

CSF_FUNC(int, __fastcall, DynamicObj_TryAllocate, 0x00441D80, (void* self, void* edx, int a2, int a3, char a4));
CSF_FUNC(int, __fastcall, DynamicObj_AllocateFallback, 0x00441D10, (void* self, void* edx, int a2, int a3, int a4));

// Creates a dynamic object: allocates through the primary path, assigns it the next
// unique id, marks its handle as unset (-1), and runs its virtual init slot 1.
// If the primary path yields nothing, the fallback allocation is used and only
// the id/handle fields are assigned (no init call).
static int __fastcall DynamicObj_CreateWithUniqueId(S_00441ca0* self, void* /*edx*/, int a2, int a3, int a4)
{
    int primaryResult = DynamicObj_TryAllocate(self, nullptr, a2, a3, static_cast<char>(a4));
    if (primaryResult != 0)
    {
        uint8_t* object = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(primaryResult));

        int32_t uniqueId = g_NextUniqueId;
        g_NextUniqueId = uniqueId + 1;
        CSF_AT(int32_t, object, 0x20) = uniqueId;
        CSF_AT(int32_t, object, 0x24) = -1;

        // Embedded sub-object at +0x0C: call vtable slot 1 (byte offset 4) with this = object+0x0C.
        uint8_t* subObject = object + 0x0C;
        void** subVtable = CSF_AT(void**, subObject, 0);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(subVtable[1])(subObject, nullptr);

        return primaryResult;
    }

    int result = DynamicObj_AllocateFallback(self, nullptr, a2, a3, a4);
    uint8_t* object = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));

    int32_t uniqueId = g_NextUniqueId;
    g_NextUniqueId = uniqueId + 1;
    CSF_AT(int32_t, object, 0x20) = uniqueId;
    CSF_AT(int32_t, object, 0x24) = -1;

    return result;
}

HOOK(0x00441F10, DynamicObj_CreateWithUniqueId, "auto");
