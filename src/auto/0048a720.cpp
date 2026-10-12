#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, WeaponFirearms_CreateActionEntry, 0x00475A40, (int type, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, CopyActionRecord, 0x0048A090, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, InitializeActionRecord, 0x0048A110, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, GrowActionRecords, 0x0048A2B0, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_00833708_AppendFirearmAction(
    C_00833708* self, void* /*edx*/, int actionArgument, int /*unused*/)
{
    alignas(8) uint8_t record[0x60];
    InitializeActionRecord(record, nullptr);

    uint32_t* action = WeaponFirearms_CreateActionEntry(
        3, actionArgument, static_cast<int>(self->field_0008));
    CSF_AT(uint32_t*, record, 0) = action;
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, action, 0)[0xDC / 4])(action, nullptr);

    const int ownerState = CSF_AT(
        int, reinterpret_cast<void*>(self->field_0008), 0x78);
    const uint32_t capacity = self->field_0010;
    CSF_AT(uint32_t, record, 0x04) = 1;
    CSF_AT(uint8_t, record, 0x58) = 1;
    const uint32_t count = CSF_AT(uint32_t, self, 0x14);
    CSF_AT(int, record, 0x54) = ownerState != 1;

    if (count >= capacity)
        GrowActionRecords(reinterpret_cast<uint8_t*>(self) + 0x0C,
                          nullptr, CSF_AT(int, self, 0x18));

    const uint32_t index = CSF_AT(uint32_t, self, 0x14);
    void* destination = reinterpret_cast<void*>(self->field_000c + 96 * index);
    CSF_AT(uint32_t, self, 0x14) = index + 1;
    return CopyActionRecord(destination, nullptr, reinterpret_cast<int>(record));
}

HOOK(0x0048A720, C_00833708_AppendFirearmAction, "auto");
