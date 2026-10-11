#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall NamedEntry_Serialize(void* self, void* /*edx*/, S_00691860* archive)
{
    const int32_t savedContext = CSF_AT(int32_t, archive, 0x1C);

    bool failed = false;
    if (!CSF_AT(bool, archive, 0x00))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, S_00691860*))
            CSF_AT(void**, backend, 0x00)[0x80 / 4])(
                backend, nullptr, 0, archive);
    }

    CSF_AT(bool, archive, 0x00) = failed;
    CSF_AT(int32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(bool, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0x00)[0x44 / 4])(
                    backend, nullptr, self, archive) == -1;
    }

    failed = CSF_AT(bool, archive, 0x00);
    CSF_AT(int32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(bool, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0x00)[0x38 / 4])(
                    backend, nullptr,
                    static_cast<uint8_t*>(self) + 0x04, archive) == -1;
    }

    failed = CSF_AT(bool, archive, 0x00);
    CSF_AT(int32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x10), archive);
    }

    failed = CSF_AT(bool, archive, 0x00);
    CSF_AT(int32_t, archive, 0x1C) = savedContext;
    if (!failed)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        const int result =
            ((int(__fastcall*)(void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0x00)[0x7C / 4])(
                    backend, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(bool, archive, 0x00) = false;
            return result;
        }
    }

    CSF_AT(bool, archive, 0x00) = true;
    return 1;
}

HOOK(0x004D1830, NamedEntry_Serialize, "auto");
