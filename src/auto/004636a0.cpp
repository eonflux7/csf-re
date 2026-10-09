#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));

static int __stdcall ScriptCommands_SubmitObjectRecordToGlobal(int /*a1*/, int /*a2*/, int* a3)
{
    struct ObjectRecord {
        void* vtbl0;
        int32_t field_04;
        void* vtbl1;
        int32_t field_0C;
        int32_t field_10;
        void* vtbl2;
        int32_t field_18;
        int32_t field_1C;
        int32_t field_20;
        uint32_t flags_24;
        int32_t field_28;
        int32_t field_2C;
        uint8_t field_30;
    };
    static_assert(sizeof(ObjectRecord) == 0x34);

    void* object = reinterpret_cast<void*>(static_cast<uint32_t>(*a3));
    void* initialSubobject = reinterpret_cast<void*>(CSF_AT(uint32_t, object, 0x04));
    const bool hasSpecialFlag =
        (CSF_AT(uint32_t, initialSubobject, 0x04) & 0x8000000) != 0;

    ObjectRecord record;
    if (hasSpecialFlag) {
        record.field_0C = -1;
        record.field_10 = -1;
        record.field_18 = -1;
        record.field_1C = -1;
        record.flags_24 = 0;
        record.vtbl1 = CSF_VTBL(0x0082D448);
        record.vtbl2 = CSF_VTBL(0x0082D448);
        record.field_20 = 0;
        record.field_2C = 0;
        record.field_28 = 0;
        record.field_30 = 0;
        record.vtbl0 = CSF_VTBL(0x0082D478);
        record.field_04 = 1;

        void* subobject = reinterpret_cast<void*>(CSF_AT(uint32_t, object, 0x04));
        record.flags_24 = CSF_AT(uint32_t, subobject, 0x04) & 0xF7FFFFFF;
    } else {
        void* subobject = reinterpret_cast<void*>(CSF_AT(uint32_t, object, 0x04));
        const int32_t state = CSF_AT(int32_t, subobject, 0x04);

        record.field_0C = -1;
        record.field_10 = -1;
        record.field_18 = -1;
        record.field_1C = -1;
        record.vtbl1 = CSF_VTBL(0x0082D448);
        record.vtbl2 = CSF_VTBL(0x0082D448);
        record.vtbl0 = CSF_VTBL(0x0082D478);
        record.field_04 = state;
        record.field_20 = 0;
        record.flags_24 = 0;
        record.field_2C = 0;
        record.field_28 = 0;
        record.field_30 = 0;
    }

    int globalObject = GetGlobalObjectField184();
    return static_cast<int>(DispatchRecordToGlobalObject(
        reinterpret_cast<void*>(static_cast<uint32_t>(globalObject)),
        nullptr, &record, 0));
}

HOOK(0x004636A0, ScriptCommands_SubmitObjectRecordToGlobal, "auto");
