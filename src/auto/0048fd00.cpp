#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));

static int __fastcall C_00833908_DispatchToBaseHandler(
    C_00833908* self, void* /*edx*/, int source)
{
    void* subobject = &self->field_0004;
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, subobject, 0)[0x24 / 4])(subobject, nullptr);

    alignas(4) uint8_t record[0x34];
    const uint32_t owner = self->field_0020;
    CSF_AT(void**, record, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(void**, record, 0x14) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, record, 0x10) = self->field_0024;
    CSF_AT(uint32_t, record, 0x20) = 0;
    CSF_AT(uint32_t, record, 0x24) = 0;
    CSF_AT(uint32_t, record, 0x2C) = 0;
    CSF_AT(uint32_t, record, 0x28) = 0;
    CSF_AT(char, record, 0x30) = 0;
    CSF_AT(uint32_t, record, 0x0C) = owner;

    void* sourceRecord = reinterpret_cast<void*>(source);
    const uint32_t first = CSF_AT(uint32_t, sourceRecord, 0x04);
    const uint32_t second = CSF_AT(uint32_t, sourceRecord, 0x08);
    CSF_AT(void**, record, 0x00) = CSF_VTBL(0x0082D478);
    CSF_AT(uint32_t, record, 0x04) = 6;
    CSF_AT(uint32_t, record, 0x18) = first;
    CSF_AT(uint32_t, record, 0x1C) = second;

    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    return static_cast<int>(
        DispatchRecordToGlobalObject(dispatcher, nullptr, record, 0));
}

HOOK(0x0048FD00, C_00833908_DispatchToBaseHandler, "auto");
