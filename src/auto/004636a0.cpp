#include <cstddef>
#include <cstdint>

#include "globals.h"
#include "hook.h"

// Original calls: GetGlobalObjectField184 (cdecl, no args), then the thiscall at 0x4758B0.
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0,
         (void* self, void* edx, void* record, char flag));

// Fills the 0x34-byte local record that is handed to the global object.
// Two secondary-base vtables (0x82D448) sit at +0x08 and +0x14.
static void InitSubmitRecord(uint8_t* record, int stateValue, uint32_t flagsValue)
{
    *CSF_AT(void***, record, 0x00) = CSF_VTBL(0x0082D478);
    *CSF_AT(int*, record, 0x04) = stateValue;
    *CSF_AT(void***, record, 0x08) = CSF_VTBL(0x0082D448);
    *CSF_AT(int*, record, 0x0C) = -1;
    *CSF_AT(int*, record, 0x10) = -1;
    *CSF_AT(void***, record, 0x14) = CSF_VTBL(0x0082D448);
    *CSF_AT(int*, record, 0x18) = -1;
    *CSF_AT(int*, record, 0x1C) = -1;
    *CSF_AT(int*, record, 0x20) = 0;
    *CSF_AT(uint32_t*, record, 0x24) = flagsValue;
    *CSF_AT(int*, record, 0x28) = 0;
    *CSF_AT(int*, record, 0x2C) = 0;
    *CSF_AT(uint8_t*, record, 0x30) = 0;
}

static int __stdcall SubmitObjectRecordToGlobal(int /*a1*/, int /*a2*/, int* objectRef)
{
    // object = *objectRef, loaded once; the first test reads subobject = object->+4 and its flags word.
    uint8_t* object = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(*objectRef));
    uint8_t* subobject =
        reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(*CSF_AT(uint32_t*, object, 0x04)));

    alignas(4) uint8_t record[0x34];

    if ((*CSF_AT(uint32_t*, subobject, 0x04) & 0x8000000) != 0)
    {
        // The original reloads object->+4 here (it does not reuse the subobject cached above),
        // then reads that pointer's flags word again.
        uint8_t* reloadedSubobject =
            reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(*CSF_AT(uint32_t*, object, 0x04)));
        // Flag bit 27 set: record state 1, flags with bit 27 cleared.
        const uint32_t flags = *CSF_AT(uint32_t*, reloadedSubobject, 0x04) & 0xF7FFFFFFu;
        InitSubmitRecord(record, 1, flags);
    }
    else
    {
        // The original reloads object->+4 here as well, then reads its flags word as the record state.
        uint8_t* reloadedSubobject =
            reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(*CSF_AT(uint32_t*, object, 0x04)));
        // Flag bit 27 clear: record state is the subobject's flags word, flags zero.
        const int recordState = *CSF_AT(int*, reloadedSubobject, 0x04);
        InitSubmitRecord(record, recordState, 0);
    }

    const int globalObject = GetGlobalObjectField184();
    return static_cast<int>(DispatchRecordToGlobalObject(
        reinterpret_cast<void*>(static_cast<uintptr_t>(globalObject)), nullptr, record, 0));
}

HOOK(0x004636A0, SubmitObjectRecordToGlobal, "auto");
