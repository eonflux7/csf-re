#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets this object's per-frame tracking fields, builds a 0x34-byte dispatch
// record (vtable 0x82D478, record-type 26, two 0x82D448 vtable slots, owner
// indices copied from the owner object) and sends it to the global object.

CSF_FUNC(void, __fastcall, ResetPadSubObject, 0x00427010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0,
         (void* self, void* edx, void* record, char flag));

static int __fastcall C_0082faf8_ResetAndDispatchRecord(C_0082faf8* self, void* /*edx*/)
{
    // Reset the embedded sub-object at +0x24 first.
    ResetPadSubObject(reinterpret_cast<uint8_t*>(self) + 0x24, nullptr);

    // Owner object referenced by field_0014.
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0014);

    self->field_0050 = 0.0f;
    self->field_0058 = CSF_AT(float, owner, 0x194);
    self->field_005c = self->field_0054;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0060 = 0;
    self->field_0020 = 1;

    // 0x34-byte record on the stack; layout matches the original frame:
    //   +0x00 vtable 0x82D478, +0x04 type 26,
    //   +0x08 vtable 0x82D448, +0x0C owner+0x20, +0x10 owner+0x24,
    //   +0x14 vtable 0x82D448, +0x18..+0x1C -1, +0x20..+0x2C 0, +0x30 flag byte 0.
    alignas(8) uint8_t recordBytes[0x34];
    CSF_AT(void*, recordBytes, 0x00) = reinterpret_cast<void*>(CSF_VTBL(0x0082D478));
    CSF_AT(int, recordBytes, 0x04) = 26;
    CSF_AT(void*, recordBytes, 0x08) = reinterpret_cast<void*>(CSF_VTBL(0x0082D448));
    CSF_AT(int, recordBytes, 0x0C) = CSF_AT(int, owner, 0x20);
    CSF_AT(int, recordBytes, 0x10) = CSF_AT(int, owner, 0x24);
    CSF_AT(void*, recordBytes, 0x14) = reinterpret_cast<void*>(CSF_VTBL(0x0082D448));
    CSF_AT(int, recordBytes, 0x18) = -1;
    CSF_AT(int, recordBytes, 0x1C) = -1;
    CSF_AT(int, recordBytes, 0x20) = 0;
    CSF_AT(int, recordBytes, 0x24) = 0;
    CSF_AT(int, recordBytes, 0x28) = 0;
    CSF_AT(int, recordBytes, 0x2C) = 0;
    CSF_AT(uint8_t, recordBytes, 0x30) = 0;

    // The original calls GetGlobalObjectField184 between pushing the record
    // arguments and calling DispatchRecord; its result becomes DispatchRecord's this.
    void* globalObject = reinterpret_cast<void*>(GetGlobalObjectField184());
    return static_cast<int>(
        DispatchRecordToGlobalObject(globalObject, nullptr, recordBytes, 0));
}

HOOK(0x00424440, C_0082faf8_ResetAndDispatchRecord, "auto");
