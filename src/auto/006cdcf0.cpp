#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(void*, __fastcall, C_00860bb8_DetachFromScene, 0x006CC1A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00860bb8_ReleaseSceneResources, 0x006CD4B0, (void* self, void* edx));

static void __fastcall C_00860af0_Destroy(C_00860af0* self, void* /*edx*/)
{
    const uint8_t active = self->field_0068;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860BB8);
    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x00860BB0);

    if (active)
    {
        if (self->field_0069)
            C_00860bb8_DetachFromScene(self, nullptr);
        C_00860bb8_ReleaseSceneResources(self, nullptr);
    }

    void* resource = reinterpret_cast<void*>(self->field_0010);
    if (resource)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, resource, 0)[0])(resource, nullptr, 1);
        self->field_0010 = 0;
    }

    resource = reinterpret_cast<void*>(self->field_0070);
    if (resource)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, resource, 0)[1])(resource, nullptr, 1);
        self->field_0070 = 0;
    }

    resource = reinterpret_cast<void*>(self->field_0008);
    if (resource)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, resource, 0)[1])(resource, nullptr, 1);
        self->field_0008 = 0;
    }

    resource = reinterpret_cast<void*>(self->field_000c);
    if (resource)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, resource, 0)[1])(resource, nullptr, 1);
        self->field_000c = 0;
    }

    resource = reinterpret_cast<void*>(self->field_0098);
    if (resource)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, resource, 0)[1])(resource, nullptr, 1);
        self->field_0098 = 0;
    }

    resource = reinterpret_cast<void*>(self->field_009c);
    if (resource)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, resource, 0)[1])(resource, nullptr, 1);
        self->field_009c = 0;
    }

    if (self->field_00c8)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_00c8));
        self->field_00c8 = 0;
    }

    if (self->field_00a0)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_00a0));
        self->field_00a0 = 0;
    }

    if (self->field_0084)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0084));
        self->field_0084 = 0;
    }

    if (self->field_0074)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0074));
        self->field_0074 = 0;
    }

    if (self->field_0054)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0054));
        self->field_0054 = 0;
    }

    uint8_t* bufferSlot = reinterpret_cast<uint8_t*>(self) + 0x54;
    int remaining = 3;
    do
    {
        bufferSlot -= 0x10;
        void* buffer = reinterpret_cast<void*>(CSF_AT(uint32_t, bufferSlot, 0));
        if (buffer)
        {
            Mem_Free(buffer);
            CSF_AT(uint32_t, bufferSlot, 0) = 0;
        }
        --remaining;
    } while (remaining);

    if (self->field_0014)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0014));
        self->field_0014 = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860AF0);
    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x00860AEC);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006CDCF0, C_00860af0_Destroy, "auto");
