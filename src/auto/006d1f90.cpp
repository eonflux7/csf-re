#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, LightObject_Init, 0x006FD540, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* edx, int additionalCapacity));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Allocates a light object (0x84 bytes), initializes it, appends it to this object's
// light list (growing it when full), and clears a flag byte in its cached provider.
static void* __fastcall C_00860e68_AddLightToWorld(C_00860e68* self, void* /*edx*/, int a2)
{
    // The original references S_0041e5f0's vtable (at the call site of GrowArrayCapacity).
    (void)CSF_VTBL(0x0041E5F0);

    C_0085e7b8* light = nullptr;
    void* memory = Mem_Alloc(0x84);
    if (memory) {
        light = reinterpret_cast<C_0085e7b8*>(
            LightObject_Init(memory, nullptr, a2,
                             static_cast<int>(reinterpret_cast<std::uintptr_t>(self))));
    }

    // Light list lives embedded at this+0x64.
    S_0041e5f0* list = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x64);

    uint32_t count = self->field_006c;
    uint32_t capacity = self->field_0068;
    if (count >= capacity) {
        S_0041e5f0_GrowArrayCapacity(list, nullptr, list->field_000c);
    }

    uint32_t index = list->field_0008;
    C_0085e7b8** slot = reinterpret_cast<C_0085e7b8**>(
        static_cast<std::uintptr_t>(list->field_0000) + 4u * index);
    list->field_0008 = index + 1;
    *slot = light;

    void* provider = GetCachedObjectProvider(light, nullptr);
    CSF_AT(uint8_t, provider, 2) = 0;
    GetCachedObjectProvider(light, nullptr);

    return light;
}

HOOK(0x006D1F90, C_00860e68_AddLightToWorld, "auto");
