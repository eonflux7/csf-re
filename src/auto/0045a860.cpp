#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, HudStatus_SerializeIconMember, 0x0045A150, (void* a1, int a2));
CSF_FUNC(int, __stdcall, HudStatus_SerializeIconBase, 0x0045A360, (void* a1, int a2));

static int __fastcall HudStatusIcons_SerializeRecords(
    void* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, archiveAddress);
    }

    const int savedContext = CSF_AT(int, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = failed;
    CSF_AT(int, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        HudStatus_SerializeIconMember(
            reinterpret_cast<uint8_t*>(self) + 0x04,
            archiveAddress);
    }

    const bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        HudStatus_SerializeIconBase(
            reinterpret_cast<uint8_t*>(self) + 0x14,
            archiveAddress);
    }

    CSF_AT(int, archive, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, serializer, 0)[0x7C / 4])(
                    serializer, nullptr, archiveAddress);
        if ((result & 0xFF) != 0)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x0045A860, HudStatusIcons_SerializeRecords, "auto");
