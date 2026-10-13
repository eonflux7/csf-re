#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwTexDictionaryGetCurrent, 0x006319B0, ());
CSF_FUNC(int, __cdecl, RwTexDictionaryForAllTextures, 0x006316E0, (int dictionary, int (__cdecl* callback)(uint32_t*, int), int context));
CSF_FUNC(int, __cdecl, RwTexDictionaryDestroy, 0x00632340, (int dictionary));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

static uint32_t* __fastcall C_008607f8_DestroyTextureDictionary(C_008607f8* self, void* /*edx*/, char flags)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008607F8);

    int dictionary = RwTexDictionaryGetCurrent();
    if (dictionary != 0)
    {
        RwTexDictionaryForAllTextures(
            dictionary,
            reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(CSF_ADDR_FN(0x006C1120)),
            0);
        RwTexDictionaryDestroy(dictionary);
    }

    self->field_0004 = 0;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008607B0);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((flags & 1) != 0)
    {
        CoreMemory_Free(self);
    }
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006C1850, C_008607f8_DestroyTextureDictionary, "auto");
