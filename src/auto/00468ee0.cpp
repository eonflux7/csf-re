#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Callee: per-record reset of a 0x34-byte state record (called 12 times on the local buffer).
CSF_FUNC(uint32_t*, __fastcall, StateRecord_Reset, 0x00401870, (void* self, void* edx, int a2));

// Callee: fills the local state buffer from the parser object (__stdcall, pops 8).
CSF_FUNC(int, __stdcall, MissionScript_FillStateFromObject, 0x00468F50, (int a1, void* a2));

static int* __stdcall MissionScript_ResetParserState(int a1)
{
    // 12 records of 0x34 bytes each fill the 0x270-byte stack buffer.
    uint8_t stateBuffer[0x270];
    uint8_t* cursor = stateBuffer;
    for (int remaining = 12; remaining != 0; --remaining)
    {
        StateRecord_Reset(cursor, nullptr, -1);
        cursor += 52;
    }

    void* object = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));
    uint8_t* objectBytes = reinterpret_cast<uint8_t*>(object);

    CSF_AT(int, objectBytes, 0x1c) = 0;

    if (CSF_AT(uint8_t, objectBytes, 0) == 0)
    {
        CSF_AT(uint8_t, objectBytes, 0x15) = 0;
        CSF_AT(uint8_t, objectBytes, 0x16) = 0;
        MissionScript_FillStateFromObject(
            static_cast<int>(reinterpret_cast<uintptr_t>(stateBuffer)), object);
    }

    // Teardown of the 12 records, last to first: each has three C_0082d410 subobjects (+0x14, +0x08, +0x00).
    void** const vtableValue = CSF_VTBL(0x0082D410);
    for (int index = 11; index >= 0; --index)
    {
        CSF_AT(void**, stateBuffer, index * 52 + 0x14) = vtableValue;
        CSF_AT(void**, stateBuffer, index * 52 + 0x08) = vtableValue;
        CSF_AT(void**, stateBuffer, index * 52 + 0x00) = vtableValue;
    }
    // The original leaves the address of the first record's second subobject in eax.
    return reinterpret_cast<int*>(stateBuffer + 8);
}

HOOK(0x00468EE0, MissionScript_ResetParserState, "auto");
