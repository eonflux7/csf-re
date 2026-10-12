#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, ResizeIntVectorToCount, 0x0046D800, (S_0046c410* self, void* , unsigned int requestedCount, int fillValue));
CSF_FUNC(uint8_t*, __fastcall, BddTables_ResizeWeatherResourceVector, 0x0051B330, (void* self, void* edx, unsigned int a2, int a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));

static int __fastcall BddTables_RefreshWeatherResources(void* self, void* /*edx*/)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    void* block = CSF_AT(void*, self, 0x10);
    if (block)
        Mem_FreeDebug(block);

    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;

    block = CSF_AT(void*, self, 0x20);
    if (block)
        Mem_FreeDebug(block);

    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;

    SetAllocationMode(2);

    void* table = CSF_AT(void*, self, 0x08);
    uint32_t begin = CSF_AT(uint32_t, table, 0x04);
    int32_t count = 0;
    if (begin)
        count = static_cast<int32_t>(
            CSF_AT(uint32_t, table, 0x08) - begin) / 552;

    BddTables_ResizeWeatherResourceVector(
        bytes + 0x0C, nullptr, static_cast<unsigned int>(count), 0);

    SetAllocationMode(0);
    SetAllocationMode(2);

    table = CSF_AT(void*, self, 0x08);
    begin = CSF_AT(uint32_t, table, 0x04);
    count = 0;
    if (begin)
        count = static_cast<int32_t>(
            CSF_AT(uint32_t, table, 0x08) - begin) / 552;

    ResizeIntVectorToCount(
        reinterpret_cast<S_0046c410*>(bytes + 0x1C),
        nullptr, static_cast<unsigned int>(count), -1);

    return SetAllocationMode(0);
}

HOOK(0x0051B3B0, BddTables_RefreshWeatherResources, "auto");
