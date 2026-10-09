#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall AudioSoundManager_SerializeSoundEntry(
    void* self, void* /*edx*/, bool* archive)
{
    const int32_t savedContext = CSF_AT(int32_t, archive, 0x1C);

    bool failed = false;
    if (!CSF_AT(bool, archive, 0x00))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, bool*))
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
            ((int(__fastcall*)(void*, void*, void*, bool*))
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
            ((int(__fastcall*)(void*, void*, void*, bool*))
                CSF_AT(void**, backend, 0x00)[0x54 / 4])(
                    backend, nullptr,
                    static_cast<uint8_t*>(self) + 0x04, archive) == -1;
    }

    failed = CSF_AT(bool, archive, 0x00);
    CSF_AT(int32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            static_cast<uint8_t*>(self) + 0x08, archive);
    }

    failed = CSF_AT(bool, archive, 0x00);
    CSF_AT(int32_t, archive, 0x1C) = savedContext;
    if (!failed)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        const bool result =
            ((bool(__fastcall*)(void*, void*, bool*))
                CSF_AT(void**, backend, 0x00)[0x7C / 4])(
                    backend, nullptr, archive);
        if (result)
        {
            CSF_AT(bool, archive, 0x00) = false;
            return result;
        }
    }

    CSF_AT(bool, archive, 0x00) = true;
    return 1;
}

HOOK(0x00523EE0, AudioSoundManager_SerializeSoundEntry, "auto");
