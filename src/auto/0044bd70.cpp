#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));

// Accumulates elapsed time into self+0x20. Once it reaches the threshold stored at
// GlobalValue+0xA68, resets the accumulator and dispatches a 0x12 record to the global object.
static void __fastcall SceneLoader_UpdateAttachedObject(uint8_t* self, void* /*edx*/, float a2)
{
    CSF_AT(float, self, 0x20) = CSF_AT(float, self, 0x20) + a2;

    uint8_t* globalValue = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
    float threshold = CSF_AT(float, globalValue, 0xA68);
    if (CSF_AT(float, self, 0x20) < threshold)
        return;

    CSF_AT(float, self, 0x20) = 0.0f;

    alignas(8) uint8_t recordBytes[0x34];
    std::memset(recordBytes + 0x20, 0, 16);
    recordBytes[0x30] = 0;
    CSF_AT(void*, recordBytes, 0x00) = CSF_VTBL(0x0082D478);
    CSF_AT(uint32_t, recordBytes, 0x04) = 0x12;
    CSF_AT(void*, recordBytes, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, recordBytes, 0x0C) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, recordBytes, 0x10) = 0xFFFFFFFFu;
    CSF_AT(void*, recordBytes, 0x14) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, recordBytes, 0x18) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, recordBytes, 0x1C) = 0xFFFFFFFFu;

    uint8_t* globalObject = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalObjectField184()));
    DispatchRecordToGlobalObject(globalObject, nullptr, recordBytes, 1);
}

HOOK(0x0044BD70, SceneLoader_UpdateAttachedObject, "auto");
