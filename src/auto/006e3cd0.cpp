#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (cdecl, caller pops).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Base-class initializer called first by the object constructor (__fastcall: ecx=this, edx unused).
CSF_FUNC(int, __fastcall, sub_6D2710, 0x006D2710, (void* self, void* edx));

// Allocates a 164-byte shared-resource object from the game heap, runs its base
// initializer, installs its vtables and default fields, and returns it (nullptr if allocation fails).
static void* __cdecl GetSharedResource()
{
    void* resourceObject = Mem_Alloc(0xA4);
    if (!resourceObject)
        return nullptr;

    // Base initializer runs before this class's vtables and fields are written.
    sub_6D2710(resourceObject, nullptr);

    CSF_AT(void*, resourceObject, 0x00) = CSF_VTBL(0x008631C8);
    CSF_AT(void*, resourceObject, 0x04) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x008631C4));
    CSF_AT(int, resourceObject, 0x10) = 8;
    CSF_AT(int, resourceObject, 0x14) = 2;
    return resourceObject;
}

HOOK(0x006E3CD0, GetSharedResource, "auto");
