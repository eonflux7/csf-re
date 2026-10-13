#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __fastcall, CoreMemory_GrowReferenceArray, 0x00691730, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_0085e5d8_CreateReferenceArchive(
    C_0085e5d8* self, void* /*edx*/, int archiveArgument)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0085E5D8);
    auto* archives = reinterpret_cast<S_0041e5f0*>(
        reinterpret_cast<uint8_t*>(self) + 0x004);

    self->field_0004 = 0;
    CSF_AT(uint32_t, self, 0x008) = 0;
    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x010) = 10;
    self->field_0014 = static_cast<uint32_t>(archiveArgument);

    SetAllocationMode(1);
    auto* archive = static_cast<uint32_t*>(Mem_Alloc(0x10u));
    if (archive)
    {
        archive[0] = 0;
        archive[1] = 0;
        archive[2] = 0;
        archive[3] = 10;
    }
    SetAllocationMode(0);

    const uint32_t capacity = archive[1];
    if (capacity < 0x3E8)
        CoreMemory_GrowReferenceArray(
            archive, nullptr, static_cast<int>(1000 - capacity));

    (void)CSF_VTBL(0x0041E5F0);
    if (archives->field_0008 >= archives->field_0004)
        S_0041e5f0_GrowArrayCapacity(
            archives, nullptr, static_cast<int>(archives->field_000c));

    const uint32_t index = archives->field_0008;
    auto* destination =
        reinterpret_cast<uint32_t*>(archives->field_0000) + index;
    archives->field_0008 = index + 1;
    *destination = reinterpret_cast<uint32_t>(archive);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00691C80, C_0085e5d8_CreateReferenceArchive, "auto");
