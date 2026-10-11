#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
static CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
static CSF_FUNC(char, __fastcall, SceneEntity_Activate, 0x004E51B0, (void* self, void* edx, char a2));
static CSF_FUNC(char, __fastcall, SetSecondaryEntityActive, 0x004F0C30, (void* self, void* edx, char a2));

static int __fastcall C_00833908_DeactivateAndDispatchRecord(
    C_00833908* self, void* /*edx*/, int source)
{
    if (self->field_003c)
        SceneEntity_Activate(reinterpret_cast<void*>(self->field_003c), nullptr, 0);
    if (self->field_0040)
        SetSecondaryEntityActive(reinterpret_cast<void*>(self->field_0040), nullptr, 0);

    void* embeddedObject = reinterpret_cast<uint8_t*>(self) + 4;
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, embeddedObject, 0)[0x20 / 4])(embeddedObject, nullptr);

    alignas(4) uint8_t record[0x34];
    CSF_AT(void**, record, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(void**, record, 0x14) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, record, 0x0C) = self->field_0020;
    CSF_AT(uint32_t, record, 0x10) = self->field_0024;
    CSF_AT(uint32_t, record, 0x18) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(source), 0x04);
    CSF_AT(uint32_t, record, 0x1C) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(source), 0x08);
    CSF_AT(void**, record, 0x00) = CSF_VTBL(0x0082D478);
    CSF_AT(uint32_t, record, 0x04) = 8;
    CSF_AT(uint32_t, record, 0x20) = 0;
    CSF_AT(uint32_t, record, 0x24) = 0;
    CSF_AT(uint32_t, record, 0x28) = 0;
    CSF_AT(uint32_t, record, 0x2C) = 0;
    CSF_AT(uint8_t, record, 0x30) = 0;

    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    return static_cast<int>(
        DispatchRecordToGlobalObject(dispatcher, nullptr, record, 0));
}

HOOK(0x0048FC70, C_00833908_DeactivateAndDispatchRecord, "auto");
