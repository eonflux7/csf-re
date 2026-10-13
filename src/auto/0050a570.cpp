#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Binds the original global-accessor (__cdecl, no args) returning the global block pointer.
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// COptionsFile method: stores a byte in self+7, then mirrors that byte
// into the global block at offset 0x21DC. Returns the global block pointer.
static int __fastcall COptionsFile_SetFlagByteAndSyncGlobal(uint8_t* self, void* /*edx*/, char flagValue) {
    CSF_AT(uint8_t, self, 0x07) = static_cast<uint8_t>(flagValue);

    int globalBlock = GetGlobalValue_8BC1E0();

    uint8_t* globalBytes = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(static_cast<uint32_t>(globalBlock)));
    CSF_AT(uint8_t, globalBytes, 0x21DC) = CSF_AT(uint8_t, self, 0x07);

    return globalBlock;
}

HOOK(0x50A570, COptionsFile_SetFlagByteAndSyncGlobal, "auto");
