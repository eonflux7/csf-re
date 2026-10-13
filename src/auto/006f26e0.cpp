#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores a2 at this+0x2C, calls a2's vtable slot 4 (offset 0x10), and copies the
// first three dwords of the returned record into the embedded sub-object at this+0x30.
// Returns the third dword of the record.

using SourceGetRecordFn = void* (__fastcall*)(void* self, void* edx);

static int __fastcall C_00862598_CopyFromSource(C_00862598* self, void* /*edx*/, int a2)
{
    self->field_002c = static_cast<uint32_t>(a2);

    // a2->vtbl[0x10 / 4]
    void* slot = CSF_AT(void**, reinterpret_cast<void*>(a2), 0)[4];
    void** record = static_cast<void**>(
        reinterpret_cast<SourceGetRecordFn>(slot)(reinterpret_cast<void*>(a2), nullptr));

    uint8_t* sub = reinterpret_cast<uint8_t*>(self) + 0x30;
    CSF_AT(void*, sub, 0) = record[0];
    CSF_AT(uint32_t, sub, 4) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(record[1]));
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(record[2]));
    CSF_AT(uint32_t, sub, 8) = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x6F26E0, C_00862598_CopyFromSource, "auto");
