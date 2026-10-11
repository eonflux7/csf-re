#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(uint32_t*, __cdecl, CreateSetupObject, 0x0048C570, (int a1, uint32_t* a2));

static uint32_t* __fastcall C_00833908_EnableSetup(
    C_00833908* self, void* /*edx*/, int setup)
{
    auto* previous = reinterpret_cast<void*>(self->field_0044);
    if (previous)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, previous, 0)[0])(previous, nullptr, 1);
        self->field_0044 = 0;
    }

    uint32_t* created = CreateSetupObject(
        setup, reinterpret_cast<uint32_t*>(self));
    self->field_0044 = reinterpret_cast<uint32_t>(created);
    if (!created)
        return nullptr;

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
    return reinterpret_cast<uint32_t*>(count);
}

HOOK(0x00491050, C_00833908_EnableSetup, "auto");
