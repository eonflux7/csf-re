#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SerializeBaseRecord, 0x00445290,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, SceneLoader_SerializeRecordMember, 0x00445620,
         (void* a1, int a2));

static int __fastcall C_008310d8_SerializeRecord(
    C_008310d8* self, void* /*edx*/, int recordAddress)
{
    void* record = reinterpret_cast<void*>(recordAddress);
    bool failed = false;
    if (CSF_AT(uint8_t, record, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, record, 0x04);
        failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, recordAddress);
    }

    int savedContext = CSF_AT(int, record, 0x1C);
    CSF_AT(uint8_t, record, 0x00) = failed;
    CSF_AT(int, record, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, record, 0x15) = 0;
        CSF_AT(uint8_t, record, 0x16) = 0;
        SerializeBaseRecord(&self->field_0008, nullptr, recordAddress);
    }

    bool active = CSF_AT(uint8_t, record, 0x00) == 0;
    CSF_AT(int, record, 0x1C) = 0;
    if (active)
    {
        CSF_AT(uint8_t, record, 0x15) = 0;
        CSF_AT(uint8_t, record, 0x16) = 0;
        SceneLoader_SerializeRecordMember(
            reinterpret_cast<uint8_t*>(self) + 0x54, recordAddress);
    }

    active = CSF_AT(uint8_t, record, 0x00) == 0;
    CSF_AT(int, record, 0x1C) = savedContext;
    if (active)
    {
        void* serializer = CSF_AT(void*, record, 0x04);
        int result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, recordAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, record, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, record, 0x00) = 1;
    return 1;
}

HOOK(0x00445800, C_008310d8_SerializeRecord, "auto");
