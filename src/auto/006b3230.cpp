#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, GfxObject_ConstructAtomic, 0x006E6980, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxObject_InitAtomicParams, 0x006E6A20, (C_00861c08* self, void* edx, uint32_t a2, uint32_t a3, uint32_t a4));

// a2 is a list header: a2[0] = slot array, a2[1] = count, a2[2]/a2[3] = init params.
static int __cdecl GfxObject_CreateAndAppendToList(int a1, uint32_t* a2)
{
    C_00861ce0* allocated = static_cast<C_00861ce0*>(Mem_Alloc(0x4c));

    C_00861c08* object = nullptr;
    if (allocated)
    {
        object = reinterpret_cast<C_00861c08*>(
            static_cast<std::uintptr_t>(GfxObject_ConstructAtomic(allocated, nullptr)));
    }

    GfxObject_InitAtomicParams(object, nullptr, static_cast<uint32_t>(a1), a2[2], a2[3]);

    // Append to list: slots[count] = object; count++
    reinterpret_cast<uint32_t*>(a2[0])[a2[1]] =
        static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(object));
    a2[1]++;

    return a1;
}

HOOK(0x006B3230, GfxObject_CreateAndAppendToList, "auto");
