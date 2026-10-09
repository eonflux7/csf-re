#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (int a1));
CSF_FUNC(void**, __fastcall, SceneLoader_RegisterArchivedObject, 0x0069ACB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

static int __fastcall C_00840ff0_ArchiveSoundState(
    C_00840ff0* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, archiveAddress);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    AudioSoundManager_ProcessArchiveContext(archiveAddress);

    self->field_005d = 0;
    void* globalObject = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    void* provider = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, globalObject, 0)[0x70 / 4])(globalObject, nullptr);

    if (provider)
    {
        self->field_005d =
            reinterpret_cast<int(__fastcall*)(void*, void*, bool)>(
                CSF_AT(void**, provider, 0)[0x3C / 4])(
                    provider, nullptr, self->field_0004 != 3) != 0;
    }

    const bool canArchive = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canArchive)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, backend, 0)[0x44 / 4])(
                    backend, nullptr, &self->field_005d, archiveAddress) == -1;
    }

    if (CSF_AT(uint8_t, archive, 0x14) == 0 &&
        CSF_AT(uint8_t, archive, 0) == 0)
    {
        SceneLoader_RegisterArchivedObject(
            CSF_AT(void*, archive, 0x10), nullptr,
            reinterpret_cast<int>(self));
    }

    const bool canFinish = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (canFinish)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, backend, 0)[0x7C / 4])(
                    backend, nullptr, archiveAddress))
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x00527CE0, C_00840ff0_ArchiveSoundState, "auto");
