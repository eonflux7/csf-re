#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(uint32_t*, __cdecl, CreateSetupObject, 0x004F0DC0, (int a1, int a2));

static int __fastcall C_00833908_FinalizeSetupParameters(
    C_00833908* self, void* /*edx*/, int setup)
{
    auto* previous = reinterpret_cast<void*>(self->field_0040);
    if (previous)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, previous, 0)[0])(previous, nullptr, 1);
        self->field_0040 = 0;
    }

    uint32_t* created = CreateSetupObject(
        setup, reinterpret_cast<int>(self));
    self->field_0040 = reinterpret_cast<uint32_t>(created);
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

HOOK(0x00490FE0, C_00833908_FinalizeSetupParameters, "auto");
