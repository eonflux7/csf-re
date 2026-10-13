#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// The import pops only the buffer argument; preserve the receiver in ECX.
using GlobalMemoryStatusFunction =
    void (__fastcall*)(C_0085eac0*, void*, void*);
CSF_GLOBAL(GlobalMemoryStatusFunction, g_GlobalMemoryStatusImport, 0x0082D240);

CSF_FUNC(unsigned int, __fastcall, S_00471340_StoreModeAndRun, 0x0069B250, (S_00471340* self, void* , int a2, unsigned int a3, char a4));
CSF_FUNC(unsigned int, __fastcall, SetFlag29PropagateToChildren, 0x0069B380, (void* self, void* , char value));

static unsigned int __fastcall C_0085eac0_ResetHeapPools(
    C_0085eac0* self, void* /*edx*/)
{
    alignas(8) uint8_t memoryStatusBytes[0x20];
    g_GlobalMemoryStatusImport(self, nullptr, memoryStatusBytes);
    self->field_0004 = CSF_AT(uint32_t, memoryStatusBytes, 0x0C);

    auto* objectBytes = reinterpret_cast<uint8_t*>(self);
    auto* smallPool = reinterpret_cast<S_00471340*>(objectBytes + 0x08);
    auto* mediumPool = reinterpret_cast<S_00471340*>(objectBytes + 0x4C);
    auto* largePool = reinterpret_cast<S_00471340*>(objectBytes + 0x90);
    auto* largestPool = reinterpret_cast<S_00471340*>(objectBytes + 0xD4);

    S_00471340_StoreModeAndRun(smallPool, nullptr, 4, 0x1B58u, 1);
    S_00471340_StoreModeAndRun(mediumPool, nullptr, 8, 0x2710u, 1);
    S_00471340_StoreModeAndRun(largePool, nullptr, 16, 0x3A98u, 1);
    S_00471340_StoreModeAndRun(largestPool, nullptr, 32, 0x2904u, 1);

    SetFlag29PropagateToChildren(smallPool, nullptr, 1);
    SetFlag29PropagateToChildren(mediumPool, nullptr, 1);
    SetFlag29PropagateToChildren(largePool, nullptr, 1);
    unsigned int result = SetFlag29PropagateToChildren(largestPool, nullptr, 1);

    self->field_0118 = 1;
    return result;
}

HOOK(0x0069DDE0, C_0085eac0_ResetHeapPools, "auto");
