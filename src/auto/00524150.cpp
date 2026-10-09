#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SoundManager_RegisterActiveInstance, 0x00521DF0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, SoundRequest_CopyConstruct, 0x00522E20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SoundManager_AttachInstanceAtPosition, 0x00523320, (void* self, void* edx, float* a2, int a3, int a4));
CSF_FUNC(int, __fastcall, SoundManager_PlaySimpleAtPosition, 0x00523AD0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int*, __fastcall, SoundPool_ReleaseInstance, 0x00526CF0, (void* self, void* edx, unsigned int* a2));
CSF_FUNC(uint32_t*, __fastcall, SoundPool_CreateInstance, 0x00528340, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, SoundBank_GetEntryByIndex, 0x0075A480, (void* self, void* edx, int a2));

// Starts a positional sound described by `request` at `position`.
// Returns the created sound instance (or 0 / the fallback path's result).
static int __fastcall S_00521df0_PlaySoundAtPosition(S_00521df0* self, void* /*edx*/, int request, int position)
{
    int soundId = CSF_AT(int, request, 8);
    unsigned int instance = 0;

    if (soundId == -1)
        return 0;

    void* soundBank = CSF_AT(void*, GetValue8B0554(), 0x1FD0);

    int entryIndex = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, soundBank, 0)[0xC / 4])(soundBank, nullptr, soundId);
    if (entryIndex == -1)
        return (int)instance;

    int entry = SoundBank_GetEntryByIndex(soundBank, nullptr, entryIndex);

    if (CSF_AT(char, entry, 0x22) != 0)
        return (int)instance;

    if (CSF_AT(char, entry, 0x24) == 0)
        return SoundManager_PlaySimpleAtPosition(self, nullptr, request, position);

    uint32_t requestCopy[14]; // 0x38 bytes
    SoundRequest_CopyConstruct(requestCopy, nullptr, request);
    requestCopy[0] = 6;

    instance = (unsigned int)SoundPool_CreateInstance(reinterpret_cast<void*>(self->field_0040), nullptr, requestCopy);
    if (instance == 0)
        return 0;

    void* instanceObj = (void*)instance;
    bool started = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, instanceObj, 0)[0x20 / 4])(instanceObj, nullptr);
    if (!started)
    {
        SoundPool_ReleaseInstance(reinterpret_cast<void*>(self->field_0040), nullptr, &instance);
        return (int)instance;
    }

    SoundManager_AttachInstanceAtPosition((uint8_t*)self + 0x10, nullptr, (float*)position, (int)instance,
                                          CSF_AT(int, entry, 0x18));
    SoundManager_RegisterActiveInstance(self, nullptr, (int)instance);
    return (int)instance;
}

HOOK(0x00524150, S_00521df0_PlaySoundAtPosition, "auto");
