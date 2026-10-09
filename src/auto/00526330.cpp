#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AudioSoundManager_SerializeObjectReferences,
         0x00526020, (void* self, void* edx, int a2));

static int __fastcall C_00841090_Serialize(
    C_00841090* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* backend = CSF_AT(void*, archive, 4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, archiveAddress);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AudioSoundManager_SerializeObjectReferences(
            reinterpret_cast<uint8_t*>(self) + 0x28,
            nullptr, archiveAddress);
    }

    bool canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, backend, 0)[0x44 / 4])(
                    backend, nullptr, &self->field_0014, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x48 / 4])(
                    backend, nullptr, &self->field_001c, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x48 / 4])(
                    backend, nullptr, &self->field_0020, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x48 / 4])(
                    backend, nullptr, &self->field_0024, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, backend, 0)[0x44 / 4])(
                    backend, nullptr, &self->field_0004, archiveAddress) == -1;
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* backend = CSF_AT(void*, archive, 4);
        // The low byte controls success, but the whole result is returned.
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, backend, 0)[0x7C / 4])(
                    backend, nullptr, archiveAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x00526330, C_00841090_Serialize, "auto");
