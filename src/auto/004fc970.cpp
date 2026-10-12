#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(uint32_t*, __fastcall, C_0083f364_InitializeRenderObject, 0x004FBC10, (C_0083f364* self, void* , uint32_t* parameters));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static int __fastcall C_0083f448_AddSceneRegion(
    C_0083f448* self, void* /*edx*/,
    int a2, int a3, int a4, int a5, int a6)
{
    // Retain the packet's vtable annotation; no vtable store occurs here.
    (void)CSF_VTBL(0x0041E5F0);

    alignas(8) uint8_t parameterBytes[0x24];
    auto* parameters = reinterpret_cast<uint32_t*>(parameterBytes);

    parameters[0] = static_cast<uint32_t>(a2);
    parameters[1] = static_cast<uint32_t>(a3);
    parameters[5] = 0;
    parameters[6] = 0;
    parameters[7] = 0;
    CSF_AT(float, parameterBytes, 0x20) = 1.0f;
    parameters[2] = static_cast<uint32_t>(a4);
    parameters[3] = static_cast<uint32_t>(a5);
    parameters[4] = static_cast<uint32_t>(a6);

    auto* object = static_cast<C_0083f364*>(Mem_Alloc(20));
    uint32_t* region = nullptr;
    if (object)
        region = C_0083f364_InitializeRenderObject(object, nullptr, parameters);

    if (self->field_001c >= self->field_0018)
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<uint8_t*>(self) + 0x14),
            nullptr,
            CSF_AT(int, self, 0x20));
    }

    const uint32_t index = self->field_001c;
    auto* destination =
        reinterpret_cast<uint32_t*>(self->field_0014) + index;
    self->field_001c = index + 1;
    *destination = reinterpret_cast<uint32_t>(region);

    return static_cast<int>(self->field_001c - 1);
}

HOOK(0x004FC970, C_0083f448_AddSceneRegion, "auto");
