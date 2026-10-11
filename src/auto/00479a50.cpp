#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008316d8_Serialize, 0x00477850, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall C_008317c8_Serialize(C_008317c8* self, void* /*edx*/, int a2)
{
    void* archive = reinterpret_cast<void*>(a2);
    bool failed = false;

    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* service = CSF_AT(void*, archive, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, service, 0)[0x80 / 4])(service, nullptr, 0, a2);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = static_cast<uint8_t>(failed);
    C_008316d8_Serialize(self, nullptr, a2);

    bool ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x64),
            archive);
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(&self->field_0070, reinterpret_cast<bool*>(archive));
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            reinterpret_cast<uint8_t*>(self) + 0x74,
            reinterpret_cast<bool*>(archive));
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (ready)
    {
        void* service = CSF_AT(void*, archive, 0x04);
        const int result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, service, 0)[0x7C / 4])(service, nullptr, a2);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x00479A50, C_008317c8_Serialize, "auto");
