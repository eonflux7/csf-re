#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));

static int __fastcall C_00833908_Serialize(
    C_00833908* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, stream, 0)[0x80 / 4])(
                stream, nullptr, 0, archive);
    }

    uint32_t savedState = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;

    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(&self->field_001c), archive);
    }

    bool ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;

        int result = reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
            CSF_AT(void**, stream, 0)[0x58 / 4])(
                stream, nullptr,
                reinterpret_cast<uint8_t*>(self) + 0x28, archive);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedState;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        int result = reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, stream, 0)[0x7C / 4])(
                stream, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x004918B0, C_00833908_Serialize, "auto");
