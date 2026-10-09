#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_PrepareArchive, 0x00405450, (int a1));
CSF_FUNC(void**, __fastcall, SceneLoader_RegisterArchivedObject, 0x0069ACB0, (void* self, void* edx, int a2));

static int __fastcall CSoundInstance_SerializePlaybackState(
    CSoundInstance* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, archiveAddress);
    }

    const uint32_t savedArchiveValue = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = failed;
    AudioSoundManager_PrepareArchive(archiveAddress);

    if (CSF_AT(uint8_t, archive, 0x14))
    {
        if (self->m_playing)
        {
            self->field_005f =
                (self->vShouldKeepAlive() && self->m_state != 2) || self->vSlot44();
            if (self->field_005f)
            {
                void* voice = reinterpret_cast<void*>(self->m_voice);
                self->field_0060 = ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, voice, 0)[0x3C / 4])(voice, nullptr);
            }
        }
        else
        {
            self->field_005f = 0;
        }
    }

    bool canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                CSF_AT(void**, serializer, 0)[0x44 / 4])(
                    serializer, nullptr, &self->field_005f, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr, &self->field_0060, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                CSF_AT(void**, serializer, 0)[0x44 / 4])(
                    serializer, nullptr, &self->m_loop, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, float*, int))
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_0064, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, float*, int))
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_004c, archiveAddress) == -1;
    }

    if (!CSF_AT(uint8_t, archive, 0x14) &&
        !CSF_AT(uint8_t, archive, 0x00))
    {
        self->m_playing = 1;
        SceneLoader_RegisterArchivedObject(
            CSF_AT(void*, archive, 0x10), nullptr, reinterpret_cast<int>(self));
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedArchiveValue;
    if (!CSF_AT(uint8_t, archive, 0x00))
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        const int result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, archiveAddress);
        if (static_cast<uint8_t>(result))
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x00527E60, CSoundInstance_SerializePlaybackState, "auto");
