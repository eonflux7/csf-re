#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original global referenced by the wrapper: its address is passed as the value in ecx.
CSF_GLOBAL(uint8_t, g_unk_8BAD98, 0x008BAD98);

// Callee bound as in the packet.
CSF_FUNC(uint32_t, __fastcall, GetAvailablePhysicalMemory, 0x0069E220, (uint32_t entryEcx, void*));

// Original: mov ecx, 0x8bad98; jmp GetAvailablePhysicalMemory
// Tail call with no stack arguments; the callee's result is returned unchanged.
static int __cdecl MemDebug_QueryAvailablePhysicalMemory() {
    const uint32_t entryAddress = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&g_unk_8BAD98));
    return static_cast<int>(GetAvailablePhysicalMemory(entryAddress, nullptr));
}

HOOK(0x00691080, MemDebug_QueryAvailablePhysicalMemory, "auto");
