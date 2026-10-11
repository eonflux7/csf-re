#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static int __fastcall C_00840fdc_ClearSceneResources(
    C_00840fdc* self, void* /*edx*/, char clearOptionalResources)
{
    uint32_t offset = 0;
    for (uint32_t index = 0; index < self->field_0034; ++index)
    {
        void* object = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_002c), offset + 8);
        if (object)
        {
            void* global = reinterpret_cast<void*>(GetGlobalInt());
            void* registry = reinterpret_cast<void*>(
                SceneLoader_GetObjectRegistry(global, nullptr));
            reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, registry, 0)[24 / 4])(
                    registry, nullptr, object);
        }
        offset += 20;
    }

    void* allocation = reinterpret_cast<void*>(self->field_002c);
    self->field_0030 = 0;
    self->field_0034 = 0;
    if (allocation)
    {
        Mem_Free(allocation);
        self->field_002c = 0;
    }

    offset = 0;
    for (uint32_t index = 0; index < self->field_0044; ++index)
    {
        void* object = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_003c), offset + 8);
        if (object)
        {
            void* global = reinterpret_cast<void*>(GetGlobalInt());
            void* registry = reinterpret_cast<void*>(
                SceneLoader_GetObjectRegistry(global, nullptr));
            void** registryVtable = CSF_AT(void**, registry, 0);
            int objectId = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[4 / 4])(object, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                registryVtable[24 / 4])(registry, nullptr, objectId);
        }
        offset += 12;
    }

    allocation = reinterpret_cast<void*>(self->field_003c);
    self->field_0040 = 0;
    self->field_0044 = 0;
    if (allocation)
    {
        Mem_Free(allocation);
        self->field_003c = 0;
    }

    if (clearOptionalResources)
    {
        offset = 0;
        for (uint32_t index = 0; index < self->field_0054; ++index)
        {
            void* entry = reinterpret_cast<void*>(self->field_004c + offset);
            if (CSF_AT(uint32_t, entry, 0))
            {
                int serviceId = CSF_AT(int, entry, 12);
                if (serviceId == -1)
                {
                    void* global = reinterpret_cast<void*>(GetGlobalInt());
                    void* registry = reinterpret_cast<void*>(
                        SceneLoader_GetObjectRegistry(global, nullptr));
                    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                        CSF_AT(void**, registry, 0)[24 / 4])(
                            registry, nullptr,
                            CSF_AT(uint32_t,
                                   reinterpret_cast<void*>(self->field_004c),
                                   offset));
                }
                else
                {
                    void* service = reinterpret_cast<void*>(self->field_0090);
                    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, service, 0)[12 / 4])(
                            service, nullptr, serviceId);
                }
            }
            offset += 16;
        }

        allocation = reinterpret_cast<void*>(self->field_004c);
        self->field_0050 = 0;
        self->field_0054 = 0;
        if (allocation)
        {
            Mem_Free(allocation);
            self->field_004c = 0;
        }
    }

    offset = 0;
    for (uint32_t index = 0; index < self->field_0064; ++index)
    {
        void* entry = reinterpret_cast<void*>(self->field_005c + offset);
        if (CSF_AT(uint32_t, entry, 20))
        {
            int serviceId = CSF_AT(int, entry, 32);
            if (serviceId == -1)
            {
                void* global = reinterpret_cast<void*>(GetGlobalInt());
                void* registry = reinterpret_cast<void*>(
                    SceneLoader_GetObjectRegistry(global, nullptr));
                reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                    CSF_AT(void**, registry, 0)[24 / 4])(
                        registry, nullptr,
                        CSF_AT(uint32_t,
                               reinterpret_cast<void*>(self->field_005c),
                               offset + 20));
            }
            else
            {
                void* service = reinterpret_cast<void*>(self->field_0090);
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, service, 0)[12 / 4])(
                        service, nullptr, serviceId);
            }
        }
        offset += 36;
    }

    uint32_t arrayAddress = self->field_005c;
    self->field_0060 = 0;
    self->field_0064 = 0;
    if (arrayAddress)
    {
        uint32_t count = CSF_AT(
            uint32_t, reinterpret_cast<void*>(arrayAddress - 4), 0);
        for (int index = static_cast<int>(count - 1); index >= 0; --index)
        {
            void* entry = reinterpret_cast<void*>(
                arrayAddress + 36 * static_cast<uint32_t>(index));
            CSF_AT(void**, entry, 4) = CSF_VTBL(0x0082D410);
        }
        Mem_Free(reinterpret_cast<void*>(arrayAddress - 4));
        self->field_005c = 0;
    }

    for (uint32_t index = 0; index < self->field_0024; ++index)
    {
        void* object = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_001c), 4 * index);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[28 / 4])(object, nullptr);
    }

    for (int index = static_cast<int>(self->field_0074 - 1);
         index >= 0; --index)
    {
        void* object = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_006c),
            4 * static_cast<uint32_t>(index));
        if (object)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_006c),
                   4 * static_cast<uint32_t>(index)) = 0;
        }

        uint32_t remaining = self->field_0074 - 1;
        self->field_0074 = remaining;
        if (static_cast<uint32_t>(index) != remaining)
        {
            _memcpy(
                reinterpret_cast<void*>(
                    self->field_006c + 4 * static_cast<uint32_t>(index)),
                reinterpret_cast<const void*>(
                    self->field_006c + 4 * static_cast<uint32_t>(index) + 4),
                4 * (remaining - static_cast<uint32_t>(index)));
        }
    }

    int result = static_cast<int>(self->field_006c);
    self->field_0070 = 0;
    self->field_0074 = 0;
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(result));
        self->field_006c = 0;
    }

    arrayAddress = self->field_007c;
    self->field_0080 = 0;
    self->field_0084 = 0;
    if (arrayAddress)
    {
        uint32_t count = CSF_AT(
            uint32_t, reinterpret_cast<void*>(arrayAddress - 4), 0);
        for (int index = static_cast<int>(count - 1); index >= 0; --index)
        {
            void* entry = reinterpret_cast<void*>(
                arrayAddress + 24 * static_cast<uint32_t>(index));
            CSF_AT(void**, entry, 4) = CSF_VTBL(0x0082D410);
        }
        result = Mem_Free(reinterpret_cast<void*>(arrayAddress - 4));
        self->field_007c = 0;
    }

    return result;
}

HOOK(0x00520420, C_00840fdc_ClearSceneResources, "auto");
