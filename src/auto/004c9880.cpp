#include <cstddef>
#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bindings (by address).
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));
CSF_FUNC(int, __fastcall, S_0041e5f0_DispatchRecord, 0x004C51C0, (void* self, void* edx, int a2, int a3));

// Removes the front 12-byte entry of the list at +0xEC (count at +0xF4), shifting the
// remaining entries down, then dispatches {vtable 0x82D448, entry.+4, entry.+8} to the
// sub-object at +0x6C.
static bool __fastcall S_004bc930_RemoveFrontEntry(S_004bc930* self, void* /*edx*/, int /*unusedArg*/) {
    uint8_t* entries = reinterpret_cast<uint8_t*>(self->field_00ec);
    int firstValue = CSF_AT(int, entries, 4);
    int secondValue = CSF_AT(int, entries, 8);
    uint32_t remainingCount = self->field_00f4 - 1;

    int a3[3];
    a3[0] = reinterpret_cast<int>(CSF_VTBL(0x0082D448));
    a3[1] = firstValue;
    a3[2] = secondValue;

    if (remainingCount != 0) {
        uint32_t entriesAddress = self->field_00ec;
        self->field_00f4 = remainingCount;
        _memcpy(reinterpret_cast<void*>(entriesAddress),
                reinterpret_cast<const void*>(entriesAddress + 12),
                static_cast<std::size_t>(12u * remainingCount));
    } else {
        self->field_00f4 = 0;
    }

    S_0041e5f0_DispatchRecord(reinterpret_cast<uint8_t*>(self) + 0x6C, nullptr, 3,
                              reinterpret_cast<int>(a3));
    return false;
}

HOOK(0x004C9880, S_004bc930_RemoveFrontEntry, "auto");
