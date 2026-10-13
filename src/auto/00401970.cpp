#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));

static int __fastcall SceneLoader_ResetGlobalService(void* self, void* /*edx*/)
{
    uint32_t entryOffset = 0;
    for (uint32_t index = 0; index < CSF_AT(uint32_t, self, 0x10); ++index)
    {
        void* service = reinterpret_cast<void*>(GetGlobalInt());
        void* renderHandle = reinterpret_cast<void*>(
            RenderHandle_FromObject(service, nullptr));
        uint32_t object = CSF_AT(
            uint32_t, CSF_AT(void*, self, 0x08), entryOffset);

        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, renderHandle, 0)[0x30 / 4])(
                renderHandle, nullptr, object);
        entryOffset += 20;
    }

    auto* entries = CSF_AT(uint8_t*, self, 0x08);
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    if (entries)
    {
        uint8_t* allocation = entries - 4;
        uint32_t remaining = CSF_AT(uint32_t, allocation, 0);
        if (static_cast<int32_t>(remaining - 1) >= 0)
        {
            do
            {
                --remaining;
                CSF_AT(void**, entries, remaining * 20 + 4) =
                    CSF_VTBL(0x0082D410);
            }
            while (remaining);
        }
        CoreMemory_Free(allocation);
        CSF_AT(void*, self, 0x08) = nullptr;
    }

    for (uint32_t index = 0; index < CSF_AT(uint32_t, self, 0x20); ++index)
    {
        void* service = reinterpret_cast<void*>(GetGlobalInt());
        void* renderHandle = reinterpret_cast<void*>(
            RenderHandle_FromObject(service, nullptr));
        uint32_t object = CSF_AT(
            uint32_t, CSF_AT(void*, self, 0x18), index * 4);

        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, renderHandle, 0)[0x30 / 4])(
                renderHandle, nullptr, object);
    }

    void* objects = CSF_AT(void*, self, 0x18);
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    if (objects)
    {
        CoreMemory_Free(objects);
        CSF_AT(void*, self, 0x18) = nullptr;
    }

    entries = CSF_AT(uint8_t*, self, 0x28);
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x30) = 0;
    int result = 0;
    if (entries)
    {
        uint8_t* allocation = entries - 4;
        uint32_t remaining = CSF_AT(uint32_t, allocation, 0);
        if (static_cast<int32_t>(remaining - 1) >= 0)
        {
            do
            {
                --remaining;
                CSF_AT(void**, entries, remaining * 20 + 4) =
                    CSF_VTBL(0x0082D410);
            }
            while (remaining);
        }
        result = CoreMemory_Free(allocation);
        CSF_AT(void*, self, 0x28) = nullptr;
    }
    return result;
}

HOOK(0x00401970, SceneLoader_ResetGlobalService, "auto");
