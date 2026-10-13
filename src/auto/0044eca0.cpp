#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall Serialization_ProcessRecord(
    void* self, void* /*edx*/, bool* record)
{
    bool failed = false;
    if (CSF_AT(uint8_t, record, 0x00) == 0)
    {
        void* object = CSF_AT(void*, record, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, bool*))
            CSF_AT(void**, object, 0)[0x80 / 4])(
                object, nullptr, 0, record);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, record, 0x1C);
    CSF_AT(uint8_t, record, 0x00) = failed;
    CSF_AT(uint32_t, record, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, record, 0x15) = 0;
        CSF_AT(uint8_t, record, 0x16) = 0;
        Archive_SerializeObjectRef(self, record);
    }

    bool active = CSF_AT(uint8_t, record, 0x00) == 0;
    CSF_AT(uint32_t, record, 0x1C) = 0;
    if (active)
    {
        CSF_AT(uint8_t, record, 0x15) = 0;
        CSF_AT(uint8_t, record, 0x16) = 0;
        Archive_SerializeObjectRef(
            reinterpret_cast<uint8_t*>(self) + 4, record);
    }

    active = CSF_AT(uint8_t, record, 0x00) == 0;
    CSF_AT(uint32_t, record, 0x1C) = 0;
    if (active)
    {
        void* object = CSF_AT(void*, record, 0x04);
        CSF_AT(uint8_t, record, 0x15) = 0;
        CSF_AT(uint8_t, record, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, void*, bool*))
            CSF_AT(void**, object, 0)[0x48 / 4])(
                object, nullptr, reinterpret_cast<uint8_t*>(self) + 8, record);
        CSF_AT(uint8_t, record, 0x00) = result == -1;
    }

    active = CSF_AT(uint8_t, record, 0x00) == 0;
    CSF_AT(uint32_t, record, 0x1C) = savedContext;
    if (active)
    {
        void* object = CSF_AT(void*, record, 0x04);
        const bool result = ((bool(__fastcall*)(void*, void*, bool*))
            CSF_AT(void**, object, 0)[0x7C / 4])(
                object, nullptr, record);
        if (result)
        {
            CSF_AT(uint8_t, record, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, record, 0x00) = 1;
    return 1;
}

HOOK(0x0044ECA0, Serialization_ProcessRecord, "auto");
