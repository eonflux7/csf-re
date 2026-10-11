#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, C_00840e68_UnregisterObject, 0x0051C090, (C_00840e68* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static unsigned int __fastcall C_00840f6c_ReleaseActiveState(
    C_00840f6c* self, void* /*edx*/)
{
    uint32_t recordOffset = 0;
    for (uint32_t index = 0; index < self->field_0018; ++index)
    {
        if (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010 + recordOffset), 0))
        {
            void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
            void* registry = reinterpret_cast<void*>(
                SceneLoader_GetObjectRegistry(sceneLoader, nullptr));
            uint32_t object = CSF_AT(
                uint32_t, reinterpret_cast<void*>(self->field_0010 + recordOffset), 0);
            reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, registry, 0)[0x18 / 4])(registry, nullptr, object);
        }
        recordOffset += 28;
    }

    uint32_t records = self->field_0010;
    self->field_0014 = 0;
    self->field_0018 = 0;
    if (records)
    {
        void* allocation = reinterpret_cast<void*>(records - 4);
        uint32_t count = CSF_AT(uint32_t, allocation, 0);
        if (static_cast<int32_t>(count - 1) >= 0)
        {
            uint32_t remaining = count;
            do
            {
                --remaining;
                void* recordBase = reinterpret_cast<void*>(records + 28 * remaining + 4);
                CSF_AT(void**, recordBase, 0) = CSF_VTBL(0x0082D410);
            }
            while (remaining);
        }
        Mem_Free(allocation);
        self->field_0010 = 0;
    }

    if (self->field_0020)
    {
        void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
        void* registry = reinterpret_cast<void*>(
            SceneLoader_GetObjectRegistry(sceneLoader, nullptr));
        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, registry, 0)[0x18 / 4])(
                registry, nullptr, self->field_0020);
        self->field_0020 = 0;
    }

    for (uint32_t index = 0; index < self->field_0044; ++index)
    {
        if (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_003c + 4 * index), 0))
        {
            void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
            void* registry = reinterpret_cast<void*>(
                SceneLoader_GetObjectRegistry(sceneLoader, nullptr));
            uint32_t object = CSF_AT(
                uint32_t, reinterpret_cast<void*>(self->field_003c + 4 * index), 0);
            reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, registry, 0)[0x18 / 4])(registry, nullptr, object);
        }
    }

    uint32_t objects = self->field_003c;
    self->field_0040 = 0;
    self->field_0044 = 0;
    if (objects)
    {
        Mem_Free(reinterpret_cast<void*>(objects));
        self->field_003c = 0;
    }

    return C_00840e68_UnregisterObject(
        reinterpret_cast<C_00840e68*>(self), nullptr);
}

HOOK(0x0051EDB0, C_00840f6c_ReleaseActiveState, "auto");
