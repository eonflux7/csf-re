#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, NetProfile_ValidateProfile, 0x00518860,
         (void* self, void* edx));

static int __stdcall NetProfile_ReadProfile(void* stream, S_00518780* profile)
{
    if (!stream || !profile)
        return 5;

    using Transfer = int (__fastcall*)(void*, void*, void*, int);

    if (reinterpret_cast<Transfer>(CSF_AT(void**, stream, 0)[2])(
            stream, nullptr, profile, 1) != 1)
        return 9;

    if (reinterpret_cast<Transfer>(CSF_AT(void**, stream, 0)[2])(
            stream, nullptr, &profile->field_0004, 4) != 4)
        return 9;

    if (reinterpret_cast<Transfer>(CSF_AT(void**, stream, 0)[2])(
            stream, nullptr, &CSF_AT(uint8_t, profile, 0x08), 33) != 33)
        return 9;

    if (reinterpret_cast<Transfer>(CSF_AT(void**, stream, 0)[2])(
            stream, nullptr, &CSF_AT(uint8_t, profile, 0x29), 17) != 17)
        return 9;

    if (reinterpret_cast<Transfer>(CSF_AT(void**, stream, 0)[2])(
            stream, nullptr, &profile->field_003a, 1) != 1)
        return 9;

    if (reinterpret_cast<Transfer>(CSF_AT(void**, stream, 0)[2])(
            stream, nullptr, &profile->field_0040, 4) != 4)
        return 9;

    if (reinterpret_cast<Transfer>(CSF_AT(void**, stream, 0)[2])(
            stream, nullptr, &profile->field_0044, 4) != 4)
        return 9;

    if (reinterpret_cast<Transfer>(CSF_AT(void**, stream, 0)[2])(
            stream, nullptr, &CSF_AT(uint8_t, profile, 0x48), 513) != 513)
        return 9;

    if (reinterpret_cast<Transfer>(CSF_AT(void**, stream, 0)[2])(
            stream, nullptr, &profile->field_0249, 1) != 1)
        return 9;

    if (reinterpret_cast<Transfer>(CSF_AT(void**, stream, 0)[2])(
            stream, nullptr, &profile->field_024a, 1) != 1)
        return 9;

    if (reinterpret_cast<Transfer>(CSF_AT(void**, stream, 0)[2])(
            stream, nullptr, &profile->field_024b, 1) != 1)
        return 9;

    return static_cast<uint8_t>(NetProfile_ValidateProfile(profile, nullptr)) ? 0 : 3;
}

HOOK(0x00518530, NetProfile_ReadProfile, "auto");
