#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Calls the owner's child (at owner+0x44) state getter (vtable slot 61).
// If it returns 3, builds a 0x24-byte event record on the stack and
// dispatches it through the owner's embedded sub-object (owner+4), vtable slot 22.
static int __fastcall AttackTasks_NotifyOnStateThree(S_0047ae10* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0014);
    void* stateSource = CSF_AT(void*, owner, 0x44);
    int result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, stateSource, 0)[244 / 4])(stateSource, nullptr);

    if (result == 3)
    {
        // 0x24-byte stack record, layout relative to its base (ebp-0x24):
        //   +0x00 = 2, +0x04 = 0, +0x08 = vtable C_0082d448,
        //   +0x0C = -1, +0x10 = -1, +0x14..+0x1F zeroed,
        //   +0x20 = 1, +0x21 = 0, +0x22 = 1
        alignas(8) uint8_t eventRecord[0x24];
        CSF_AT(uint32_t, eventRecord, 0x00) = 2;
        CSF_AT(uint32_t, eventRecord, 0x04) = 0;
        CSF_AT(void*, eventRecord, 0x08) = CSF_VTBL(0x0082D448);
        CSF_AT(int32_t, eventRecord, 0x0C) = -1;
        CSF_AT(int32_t, eventRecord, 0x10) = -1;
        CSF_AT(uint32_t, eventRecord, 0x14) = 0;
        CSF_AT(uint32_t, eventRecord, 0x18) = 0;
        CSF_AT(uint32_t, eventRecord, 0x1C) = 0;
        CSF_AT(uint8_t, eventRecord, 0x20) = 1;
        CSF_AT(uint8_t, eventRecord, 0x21) = 0;
        CSF_AT(uint8_t, eventRecord, 0x22) = 1;

        uint8_t* subObject = reinterpret_cast<uint8_t*>(owner) + 4;
        return reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, owner, 4)[88 / 4])(subObject, nullptr, eventRecord);
    }
    return result;
}

HOOK(0x0047CBE0, AttackTasks_NotifyOnStateThree, "auto");
