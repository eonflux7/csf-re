#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, NetProfile_InitializeRecord, 0x00518850,
         (void* self, void* edx));

using ProfileReadFunction = int (__fastcall*)(void*, void*, void*, int);

static int __stdcall NetProfile_ReadProfile(void* stream, S_00518780* profile)
{
    if (!stream || !profile)
        return 5;

    NetProfile_InitializeRecord(profile, nullptr);

    if (reinterpret_cast<ProfileReadFunction>(CSF_AT(void**, stream, 0)[3])(
            stream, nullptr, profile, 1) != 1)
        return 8;

    if (reinterpret_cast<ProfileReadFunction>(CSF_AT(void**, stream, 0)[3])(
            stream, nullptr, &profile->field_0004, 4) != 4)
        return 8;

    if (reinterpret_cast<ProfileReadFunction>(CSF_AT(void**, stream, 0)[3])(
            stream, nullptr, &CSF_AT(uint8_t, profile, 0x08), 33) != 33)
        return 8;

    if (reinterpret_cast<ProfileReadFunction>(CSF_AT(void**, stream, 0)[3])(
            stream, nullptr, &CSF_AT(uint8_t, profile, 0x29), 17) != 17)
        return 8;

    if (reinterpret_cast<ProfileReadFunction>(CSF_AT(void**, stream, 0)[3])(
            stream, nullptr, &profile->field_003a, 1) != 1)
        return 8;

    if (reinterpret_cast<ProfileReadFunction>(CSF_AT(void**, stream, 0)[3])(
            stream, nullptr, &profile->field_0040, 4) != 4)
        return 8;

    if (reinterpret_cast<ProfileReadFunction>(CSF_AT(void**, stream, 0)[3])(
            stream, nullptr, &profile->field_0044, 4) != 4)
        return 8;

    if (reinterpret_cast<ProfileReadFunction>(CSF_AT(void**, stream, 0)[3])(
            stream, nullptr, &CSF_AT(uint8_t, profile, 0x48), 513) != 513)
        return 8;

    if (reinterpret_cast<ProfileReadFunction>(CSF_AT(void**, stream, 0)[3])(
            stream, nullptr, &profile->field_0249, 1) != 1)
        return 8;

    if (reinterpret_cast<ProfileReadFunction>(CSF_AT(void**, stream, 0)[3])(
            stream, nullptr, &profile->field_024a, 1) != 1)
        return 8;

    return reinterpret_cast<ProfileReadFunction>(CSF_AT(void**, stream, 0)[3])(
               stream, nullptr, &profile->field_024b, 1) != 1
               ? 8
               : 0;
}

HOOK(0x00518660, NetProfile_ReadProfile, "auto");
