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

CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, BddLoader_ReleasePendingHandle, 0x0050E250, (void* self, void*));

// Clears the pending dispatch record and releases the associated BDD loader handle.
static int __fastcall DispatchPendingRecordAndReset(void* self, void* /*edx*/)
{
    // Snapshot the fields the original reads before any call.
    uint32_t fieldAt18 = CSF_AT(uint32_t, self, 0x18);
    uint32_t field04 = CSF_AT(uint32_t, self, 0x04);
    uint32_t field14 = CSF_AT(uint32_t, self, 0x14);

    // Build the 0x34-byte record on the stack.
    alignas(8) uint8_t recordBytes[0x34];
    CSF_AT(uint32_t, recordBytes, 0x00) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D478)));
    CSF_AT(uint32_t, recordBytes, 0x04) = 32;
    CSF_AT(uint32_t, recordBytes, 0x08) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    CSF_AT(uint32_t, recordBytes, 0x0C) = field14;
    CSF_AT(uint32_t, recordBytes, 0x10) = fieldAt18;
    CSF_AT(uint32_t, recordBytes, 0x14) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    CSF_AT(uint32_t, recordBytes, 0x18) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, recordBytes, 0x1C) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, recordBytes, 0x20) = 0;
    CSF_AT(uint32_t, recordBytes, 0x24) = field04;
    CSF_AT(uint32_t, recordBytes, 0x28) = 0;
    CSF_AT(uint32_t, recordBytes, 0x2C) = 0;
    CSF_AT(uint8_t, recordBytes, 0x30) = 0;

    void* globalObject = reinterpret_cast<void*>(GetGlobalObjectField184());
    DispatchRecordToGlobalObject(globalObject, nullptr, recordBytes, 0);

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    int result = BddLoader_ReleasePendingHandle(globalState, nullptr);

    CSF_AT(uint8_t, self, 0x30) = 0;
    CSF_AT(int, self, 0x14) = -1;
    CSF_AT(int, self, 0x18) = -1;
    return result;
}

HOOK(0x0048D7E0, DispatchPendingRecordAndReset, "auto");
