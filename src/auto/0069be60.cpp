#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, ReleaseEntryStorage, 0x0069B860,
         (int a1, int a2, int a3, int a4));

static int __fastcall C_0085e7d8_Destroy(C_0085e7d8* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0085E7D8);

    for (uint32_t offset = 0; offset < 0x140; offset += 32)
    {
        void* entry = CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), offset);
        if (entry)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, entry, 0)[0])(entry, nullptr, 1);
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), offset) = 0;
        }
    }

    const int storageObject = reinterpret_cast<int>(
        reinterpret_cast<uint8_t*>(self) + 4);

    if (self->field_0008)
    {
        ReleaseEntryStorage(static_cast<int>(self->field_0008),
                            static_cast<int>(self->field_000c),
                            storageObject, 0);
        if (self->field_0008)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008));
    }

    self->field_0008 = 0;
    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;

    if (self->field_002c >= 8 && self->field_0018)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0018));

    self->field_002c = 7;
    self->field_0028 = 0;
    std::memset(&self->field_0018, 0, sizeof(uint16_t));

    int result = static_cast<int>(self->field_0008);
    if (result)
    {
        ReleaseEntryStorage(result, static_cast<int>(self->field_000c),
                            storageObject, 0);
        result = static_cast<int>(self->field_0008);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008));
    }

    self->field_0008 = 0;
    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    return result;
}

HOOK(0x0069BE60, C_0085e7d8_Destroy, "auto");
