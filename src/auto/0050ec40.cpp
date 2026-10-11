#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(int, __fastcall, AudioSoundManager_GetSoundDescriptor, 0x0075A480, (void* self, void* edx, int a2));

static int __fastcall GameBddLoader_SetSoundSlot(
    void* self, void* /*edx*/, int soundIndex, int slot)
{
    void* owner = CSF_AT(void*, self, 0x14);
    void* slots =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, owner, 0)[0x54 / 4])(owner, nullptr);

    ((int (__fastcall*)(void*, void*, int, int))
        CSF_AT(void**, slots, 0)[4 / 4])(slots, nullptr, slot, 0);

    if (!CSF_AT(void*, self, 0xE4))
        return 0;
    if (soundIndex == -1)
        return -1;

    void* soundManager = CSF_AT(void*, self, 0xE4);
    int soundId =
        ((int (__fastcall*)(void*, void*, int))
            CSF_AT(void**, soundManager, 0)[0xC / 4])(
                soundManager, nullptr, soundIndex);
    if (soundId == -1)
        return soundId;

    void* descriptor = reinterpret_cast<void*>(
        AudioSoundManager_GetSoundDescriptor(
            CSF_AT(void*, self, 0xE4), nullptr, soundId));

    uint32_t entriesBegin = CSF_AT(uint32_t, descriptor, 8);
    if (!entriesBegin)
        return 0;

    int entryCount = static_cast<int32_t>(
        CSF_AT(uint32_t, descriptor, 0xC) - entriesBegin) / 28;
    if (!entryCount)
        return entryCount;

    int specialMode = CSF_AT(uint8_t, descriptor, 0x25) == 1;
    void* audio = reinterpret_cast<void*>(GetGlobalValue_70DDE0());

    void* entry = CSF_AT(void*, descriptor, 8);
    const char* filename = CSF_AT(uint32_t, entry, 0x18) < 0x10u
        ? reinterpret_cast<const char*>(entry) + 4
        : CSF_AT(const char*, entry, 4);

    bool available =
        ((bool (__fastcall*)(void*, void*, const char*, int, int))
            CSF_AT(void**, audio, 0)[0x30 / 4])(
                audio, nullptr, filename, 0, specialMode);
    if (!available)
        return 0;

    audio = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    owner = CSF_AT(void*, self, 0x14);
    slots =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, owner, 0)[0x54 / 4])(owner, nullptr);

    entry = CSF_AT(void*, descriptor, 8);
    filename = CSF_AT(uint32_t, entry, 0x18) < 0x10u
        ? reinterpret_cast<const char*>(entry) + 4
        : CSF_AT(const char*, entry, 4);

    void** slotsVtable = CSF_AT(void**, slots, 0);
    int soundHandle =
        ((int (__fastcall*)(void*, void*, const char*, int))
            CSF_AT(void**, audio, 0)[0x34 / 4])(
                audio, nullptr, filename, 0);

    return ((int (__fastcall*)(void*, void*, int, int))
        slotsVtable[4 / 4])(slots, nullptr, slot, soundHandle);
}

HOOK(0x0050EC40, GameBddLoader_SetSoundSlot, "auto");
