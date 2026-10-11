#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(void**, __cdecl, CreateCategoryObject, 0x004D1180, (int a1, uint32_t* a2));

static int __fastcall C_00833908_SetCategory(
    C_00833908* self, void* /*edx*/, int category)
{
    auto* previous = reinterpret_cast<C_008672e4*>(self->field_0034);
    if (previous)
    {
        previous->vSlot0(1);
        self->field_0034 = 0;
    }

    void** created = CreateCategoryObject(
        category, reinterpret_cast<uint32_t*>(self));
    self->field_0034 = reinterpret_cast<uint32_t>(created);
    if (!created)
        return 0;

    auto* objects = reinterpret_cast<S_0041e5f0*>(
        reinterpret_cast<uint8_t*>(self) + 0x9C);

    if (self->field_00a4 >= self->field_00a0)
    {
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            objects, nullptr, objects->field_000c);
    }

    uint32_t index = objects->field_0008;
    auto* entry = reinterpret_cast<uint32_t*>(objects->field_0000) + index;
    uint32_t count = index + 1;
    objects->field_0008 = count;
    *entry = reinterpret_cast<uint32_t>(created);
    return static_cast<int>(count);
}

HOOK(0x00490E00, C_00833908_SetCategory, "auto");
