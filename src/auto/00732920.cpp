#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

// The original references this vtable through the GrowArrayCapacity call site.
[[maybe_unused]] static void* const kGrowArrayVtbl = reinterpret_cast<void*>(CSF_VTBL(0x0041E5F0));

static C_00867078* __fastcall C_00867078_ConstructButtonFx(C_00867078* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x00867078);

    self->field_0008 = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    self->field_0010 = 0;
    CSF_AT(uint32_t, self, 0x14) = 10;

    SetAllocationMode(3);
    int allocResult = CoreMemory_AllocUntagged(44);
    if (allocResult != 0)
    {
        uint8_t* obj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(allocResult));
        CSF_AT(uint32_t, obj, 0x04) = 1;
        *(void***)obj = CSF_VTBL(0x0086709C);
        CSF_AT(uint32_t, obj, 0x0C) = 0;
        CSF_AT(uint32_t, obj, 0x10) = 0;
        CSF_AT(uint32_t, obj, 0x14) = 0;
        CSF_AT(uint32_t, obj, 0x1C) = 0;
        CSF_AT(uint32_t, obj, 0x20) = 0;
        CSF_AT(uint32_t, obj, 0x24) = 0;
        CSF_AT(uint32_t, obj, 0x28) = 0;
        self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(obj));
    }
    else
    {
        self->field_0004 = 0;
    }

    SetAllocationMode(0);

    S_0041e5f0* array = reinterpret_cast<S_0041e5f0*>(&self->field_0008);
    uint32_t currentCount = CSF_AT(uint32_t, self, 0x0C);
    if (currentCount < 5u)
    {
        S_0041e5f0_GrowArrayCapacity(array, nullptr, static_cast<int>(5u - currentCount));
    }

    self->field_0018 = 1;
    self->field_001c = 0x3f800000;
    return self;
}

HOOK(0x00732920, C_00867078_ConstructButtonFx, "auto");
