#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(unsigned int, __fastcall, SceneEntryList_RemoveValueAndNotify, 0x006B1840, (void* self, void* , int item));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(int, __cdecl, ReleaseSharedEmitterBuffer, 0x006E27A0, ());

static void __fastcall C_00861928_Destroy(C_00861928* self, void* /*edx*/)
{
    void* object = reinterpret_cast<void*>(self->field_0044);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861988);
    CSF_AT(uint32_t, self, 0x004) =
        reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00861980));

    if (object)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
        self->field_0044 = 0;
    }

    for (uint32_t index = 0; index < self->field_001c; ++index)
    {
        object = CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 4 * index);
        if (object)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), 4 * index) = 0;
        }
    }

    uint32_t remaining = self->field_003c;
    self->field_001c = 0;
    if (remaining)
    {
        uint32_t offset = 0;
        do
        {
            object = CSF_AT(void*, reinterpret_cast<void*>(self->field_0034), offset);
            if (object)
            {
                auto* allocation = reinterpret_cast<uint8_t*>(object) - 4;
                if (CSF_AT(uint32_t, allocation, 0))
                {
                    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, object, 0)[1])(object, nullptr, 3);
                }
                else
                {
                    CoreMemory_Free(allocation);
                }
                CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0034), offset) = 0;
            }
            offset += 12;
            --remaining;
        } while (remaining);
    }

    for (uint32_t index = 0; index < self->field_002c; ++index)
    {
        object = CSF_AT(void*, reinterpret_cast<void*>(self->field_0024), 4 * index);
        if (object)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0024), 4 * index) = 0;
        }
    }

    const uint32_t sceneEntry = self->field_005c;
    self->field_002c = 0;
    if (sceneEntry)
    {
        const int sceneList = GetGlobalInt();
        SceneEntryList_RemoveValueAndNotify(
            reinterpret_cast<void*>(sceneList), nullptr, static_cast<int>(sceneEntry));
        self->field_005c = 0;
    }

    object = reinterpret_cast<void*>(self->field_0048);
    if (object)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
        self->field_0048 = 0;
    }

    ReleaseSharedEmitterBuffer();

    if (self->field_0034)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0034));
        self->field_0034 = 0;
    }
    if (self->field_0024)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0024));
        self->field_0024 = 0;
    }
    if (self->field_0014)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0014));
        self->field_0014 = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861928);
    CSF_AT(uint32_t, self, 0x004) =
        reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00861924));
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006DF990, C_00861928_Destroy, "auto");
