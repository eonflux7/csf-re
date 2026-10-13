#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));

// Builds a 0x38-byte stack record (vtable 0x82D498 at +0, size 40 at +4, two
// 0x82D448 sub-vtables at +8/+0x14), fills it from the source object's virtual
// query result, then dispatches it to the global object.
static int __stdcall BuildAndDispatchObjectRecord(int* sourceObject)
{
    alignas(8) uint8_t recordBytes[0x38];
    uint8_t* record = recordBytes;

    CSF_AT(void*, record, 0x00) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0082D498));
    CSF_AT(int, record, 0x04) = 0x28;
    CSF_AT(void*, record, 0x08) = reinterpret_cast<void*>(CSF_VTBL(0x0082D448));
    CSF_AT(int, record, 0x0C) = -1;
    CSF_AT(int, record, 0x10) = -1;
    CSF_AT(void*, record, 0x14) = reinterpret_cast<void*>(CSF_VTBL(0x0082D448));
    CSF_AT(int, record, 0x18) = -1;
    CSF_AT(int, record, 0x1C) = -1;
    CSF_AT(int, record, 0x20) = 0;
    CSF_AT(int, record, 0x28) = 0;
    CSF_AT(int, record, 0x2C) = 0;
    CSF_AT(int, record, 0x24) = 0;
    CSF_AT(uint8_t, record, 0x30) = 0;
    CSF_AT(int*, record, 0x34) = nullptr;

    // vtable slot 108 / 4 = 27
    int hasInfo = ((int(__fastcall*)(void*, void*))(CSF_AT(void**, sourceObject, 0)[27]))(sourceObject, nullptr);
    if (hasInfo)
    {
        int infoValue = ((int(__fastcall*)(void*, void*))(CSF_AT(void**, sourceObject, 0)[27]))(sourceObject, nullptr);
        uint8_t* info = reinterpret_cast<uint8_t*>(infoValue);
        CSF_AT(int, record, 0x0C) = CSF_AT(int, info, 0x20);
        CSF_AT(int, record, 0x10) = CSF_AT(int, info, 0x24);
    }

    CSF_AT(int*, record, 0x34) = sourceObject;
    void* globalObject = reinterpret_cast<void*>(GetGlobalObjectField184());
    return static_cast<int>(DispatchRecordToGlobalObject(globalObject, nullptr, record, 0));
}

HOOK(0x401AC0, BuildAndDispatchObjectRecord, "auto");
