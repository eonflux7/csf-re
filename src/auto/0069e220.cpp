#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Import slot for kernel32!GlobalMemoryStatus (the original calls through [0x82D240]).
CSF_GLOBAL(void*, g_GlobalMemoryStatusImport, 0x82D240);

// The import is stdcall (one stack argument, callee pops 4). The original never writes ecx,
// so ecx reaches the call with the value it had on entry. To reproduce that, the hooked
// function receives ecx as a register parameter and the import is called through a
// fastcall-shaped pointer: ecx carries the entry value, edx is zero (unused by the import),
// and the buffer is the single stack argument that the stdcall target pops.
using GlobalMemoryStatusShimFn = void(__fastcall*)(uint32_t entryEcx, void* edx, void* buffer);

// Returns dwAvailPhys from a fresh MEMORYSTATUS snapshot.
static uint32_t __fastcall GetAvailablePhysicalMemory(uint32_t entryEcx, void* /*edx*/) {
    // MEMORYSTATUS is 0x20 bytes, living in this frame at [ebp-0x20].
    alignas(8) uint8_t memoryStatusBytes[0x20];

    auto globalMemoryStatus = reinterpret_cast<GlobalMemoryStatusShimFn>(g_GlobalMemoryStatusImport);
    globalMemoryStatus(entryEcx, nullptr, memoryStatusBytes);

    // dwAvailPhys sits at offset 0x0C of MEMORYSTATUS ([ebp-0x14] in the original).
    return CSF_AT(uint32_t, memoryStatusBytes, 0x0C);
}

HOOK(0x69E220, GetAvailablePhysicalMemory, "auto");
