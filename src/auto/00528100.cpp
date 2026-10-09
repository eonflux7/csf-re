#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_PrepareArchive, 0x00405450, (int a1));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(void**, __fastcall, SceneLoader_RegisterArchivedObject, 0x0069ACB0, (void* self, void* edx, int a2));

static void AudioSoundManager_SerializeArchiveWord(void* archive, void* field)
{
    const bool failed = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;

        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, stream, 0)[84 / 4])(
                    stream, nullptr, field, reinterpret_cast<int>(archive));
        CSF_AT(uint8_t, archive, 0x00) = result == -1;
    }
}

static int __fastcall C_008411a0_Serialize(
    C_008411a0* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);

    const bool failed =
        !CSF_AT(uint8_t, archive, 0x00) &&
        !reinterpret_cast<bool (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, CSF_AT(void*, archive, 0x04), 0)[128 / 4])(
                CSF_AT(void*, archive, 0x04), nullptr, 0, archiveAddress);

    const uint32_t savedState = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = failed;
    AudioSoundManager_PrepareArchive(archiveAddress);

    const bool referenceFailed = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!referenceFailed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            &self->field_0060, reinterpret_cast<bool*>(archive));
    }

    AudioSoundManager_SerializeArchiveWord(archive, &self->field_0040);
    AudioSoundManager_SerializeArchiveWord(archive, &self->field_0044);
    AudioSoundManager_SerializeArchiveWord(archive, &self->field_0048);
    AudioSoundManager_SerializeArchiveWord(archive, &self->field_004c);
    AudioSoundManager_SerializeArchiveWord(archive, &self->field_0050);

    const bool byteFailed = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!byteFailed)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;

        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, stream, 0)[68 / 4])(
                    stream, nullptr, &self->field_0054, archiveAddress);
        CSF_AT(uint8_t, archive, 0x00) = result == -1;
    }

    AudioSoundManager_SerializeArchiveWord(archive, &self->field_0058);
    AudioSoundManager_SerializeArchiveWord(archive, &self->field_005c);

    if (!CSF_AT(uint8_t, archive, 0x14) &&
        !CSF_AT(uint8_t, archive, 0x00))
    {
        self->field_003c = 1;
        SceneLoader_RegisterArchivedObject(
            CSF_AT(void*, archive, 0x10), nullptr, reinterpret_cast<int>(self));
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedState;
    if (CSF_AT(uint8_t, archive, 0x00))
    {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    void* stream = CSF_AT(void*, archive, 0x04);
    const int result =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, stream, 0)[124 / 4])(
                stream, nullptr, archiveAddress);

    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, archive, 0x00) = 0;
    return result;
}

HOOK(0x00528100, C_008411a0_Serialize, "auto");
