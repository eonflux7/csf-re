#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, C_00840e68_UnregisterObject, 0x0051C090, (C_00840e68* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static unsigned int __fastcall C_00840f48_StopAndReleaseObjects(
    C_00840f48* self, void* /*edx*/)
{
    for (uint32_t index = 0; index < self->field_0014; ++index)
    {
        if (CSF_AT(uint32_t,
                   reinterpret_cast<void*>(self->field_000c),
                   index * 16))
        {
            void* global = reinterpret_cast<void*>(GetGlobalInt());
            void* registry = reinterpret_cast<void*>(
                SceneLoader_GetObjectRegistry(global, nullptr));

            uint32_t object = CSF_AT(
                uint32_t, reinterpret_cast<void*>(self->field_000c),
                index * 16);

            reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, registry, 0)[0x18 / 4])(
                    registry, nullptr, object);
        }
    }

    uint32_t entries = self->field_000c;
    self->field_0010 = 0;
    self->field_0014 = 0;

    if (entries)
    {
        uint32_t allocation = entries - 4;
        uint32_t count = CSF_AT(
            uint32_t, reinterpret_cast<void*>(allocation), 0);

        if (static_cast<int32_t>(count - 1) >= 0)
        {
            uint32_t remaining = count;
            do
            {
                --remaining;
                void* entry = reinterpret_cast<void*>(
                    entries + remaining * 16);
                CSF_AT(void**, entry, 0x4) = CSF_VTBL(0x0082D410);
            }
            while (remaining);
        }

        Mem_Free(reinterpret_cast<void*>(allocation));
        self->field_000c = 0;
    }

    for (uint32_t index = 0; index < self->field_0024; ++index)
    {
        if (CSF_AT(uint32_t,
                   reinterpret_cast<void*>(self->field_001c),
                   index * 4))
        {
            void* global = reinterpret_cast<void*>(GetGlobalInt());
            void* registry = reinterpret_cast<void*>(
                SceneLoader_GetObjectRegistry(global, nullptr));

            uint32_t object = CSF_AT(
                uint32_t, reinterpret_cast<void*>(self->field_001c),
                index * 4);

            reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, registry, 0)[0x18 / 4])(
                    registry, nullptr, object);
        }
    }

    uint32_t objects = self->field_001c;
    self->field_0020 = 0;
    self->field_0024 = 0;

    if (objects)
    {
        Mem_Free(reinterpret_cast<void*>(objects));
        self->field_001c = 0;
    }

    return C_00840e68_UnregisterObject(
        reinterpret_cast<C_00840e68*>(self), nullptr);
}

HOOK(0x0051E640, C_00840f48_StopAndReleaseObjects, "auto");
