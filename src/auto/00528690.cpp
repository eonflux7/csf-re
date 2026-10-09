#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, SoundManager_GetEntryNameString, 0x0051BA50, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, SoundManager_GetEntryRecord, 0x0075A480, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);

using SoundManagerSlotCFn = int(__fastcall*)(void* self, void* edx, std::uint32_t a2);

static bool __fastcall C_00840ff0_LoadSoundEntryFromManager(C_00840ff0* self, void* /*edx*/)
{
    void* gameBase = reinterpret_cast<void*>(GetValue8B0554());
    C_0085e7b8* soundManager = CSF_AT(C_0085e7b8*, gameBase, 0x1FD0);

    SoundManagerSlotCFn slotCFn = reinterpret_cast<SoundManagerSlotCFn>(CSF_AT(void**, soundManager, 0)[3]);
    int soundIndex = slotCFn(soundManager, nullptr, self->field_000c);
    if (soundIndex == -1)
        return false;

    void* entry = reinterpret_cast<void*>(SoundManager_GetEntryRecord(soundManager, nullptr, soundIndex));
    void* entryName = SoundManager_GetEntryNameString(soundManager, nullptr, static_cast<unsigned int>(soundIndex));

    AssignStringSubstring(
        reinterpret_cast<void*>(reinterpret_cast<std::uint8_t*>(self) + 0x40),
        nullptr,
        entryName,
        0u,
        static_cast<std::uint32_t>(-1));

    self->field_005c = CSF_AT(std::uint8_t, entry, 0x20);

    float progress = static_cast<float>(CSF_AT(std::int32_t, entry, 0x14));
    if (CSF_AT(std::int32_t, entry, 0x14) < 0)
        progress += g_UnsignedIntegerBias;
    self->field_0034 = progress * g_ProgressScale;

    self->field_005e = CSF_AT(std::uint8_t, entry, 0x23);
    return true;
}

HOOK(0x00528690, C_00840ff0_LoadSoundEntryFromManager, "auto");
