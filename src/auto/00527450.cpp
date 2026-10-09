#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

struct SoundRequestParameters {
    float value;
    int special;
    uint8_t option;
    uint8_t specialOption;
    uint8_t reserved[2];
};
static_assert(sizeof(SoundRequestParameters) == 0xC);

static int __fastcall C_00840ff0_SubmitSoundRequest(
    C_00840ff0* self, void* /*edx*/, int /*a2*/, int /*a3*/)
{
    void* manager = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    void* soundService = reinterpret_cast<void*>(
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, manager, 0)[0x70 / 4])(manager, nullptr));

    if (!soundService)
        return 0;

    SoundRequestParameters parameters;
    parameters.value = self->field_0034;
    parameters.option = self->field_005c;
    parameters.reserved[0] = 0;
    parameters.reserved[1] = 0;

    if (self->field_0004 == 5) {
        parameters.specialOption = self->field_005e;
        parameters.special = 1;

        uint32_t* name = self->field_0058 < 0x10
            ? &self->field_0044
            : reinterpret_cast<uint32_t*>(self->field_0044);

        reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, float*)>(
            CSF_AT(void**, soundService, 0)[0x4 / 4])(
                soundService, nullptr, name, &parameters.value);

        self->vRefreshEmitterPosition(0);
        // The typed slot is void; its incidental EAX is not a defined result.
        return 0;
    }

    parameters.special = 0;
    parameters.specialOption = 0;

    uint32_t* name = self->field_0058 < 0x10
        ? &self->field_0044
        : reinterpret_cast<uint32_t*>(self->field_0044);

    return reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, float*)>(
        CSF_AT(void**, soundService, 0)[0x4 / 4])(
            soundService, nullptr, name, &parameters.value);
}

HOOK(0x00527450, C_00840ff0_SubmitSoundRequest, "auto");
