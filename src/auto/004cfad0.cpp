#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callees (bound by address)
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));

// If bit 0 of self+0x40 is set, builds a 0x34-byte dispatch record on the stack
// from the argument's +0x20/+0x24 fields and the self+0x14 sub-object's +4/+8
// fields, then forwards it to the global object. Returns the dispatch result,
// or 0 when the flag is clear (the original's result is uninitialized there).
static int __fastcall SubmitFlaggedRecordToGlobalObject(void* self, void* /*edx*/, uint32_t a2)
{
    if ((CSF_AT(uint8_t, self, 0x40) & 1) != 0)
    {
        alignas(8) uint8_t record[0x34];

        uint32_t esi = CSF_AT(uint32_t, a2, 0x20);
        uint32_t a2Field24 = CSF_AT(uint32_t, a2, 0x24);

        uint8_t* inner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x14)) + 0x1C;

        CSF_AT(uint32_t, record, 0x00) = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D478));
        CSF_AT(uint32_t, record, 0x04) = 30;
        CSF_AT(uint32_t, record, 0x08) = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        CSF_AT(uint32_t, record, 0x0C) = esi;
        CSF_AT(uint32_t, record, 0x10) = a2Field24;
        CSF_AT(uint32_t, record, 0x14) = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        CSF_AT(uint32_t, record, 0x18) = CSF_AT(uint32_t, inner, 0x04);
        CSF_AT(uint32_t, record, 0x1C) = CSF_AT(uint32_t, inner, 0x08);
        CSF_AT(uint32_t, record, 0x20) = 0;
        CSF_AT(uint32_t, record, 0x24) = 0;
        CSF_AT(uint32_t, record, 0x28) = 0;
        CSF_AT(uint32_t, record, 0x2C) = 0;
        CSF_AT(uint8_t, record, 0x30) = 0;

        void* globalObject = reinterpret_cast<void*>(GetGlobalObjectField184());
        return static_cast<int>(DispatchRecordToGlobalObject(globalObject, nullptr, record, 0));
    }
    return 0;
}

HOOK(0x004CFAD0, SubmitFlaggedRecordToGlobalObject, "auto");
