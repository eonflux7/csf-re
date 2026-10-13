#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, QuerySkinTargetCompletion, 0x00405820, (void* self, void* edx));

static char __fastcall C_00835140_UpdateSkinTargetFlag(C_00835140* self, void* /*edx*/, int /*a2*/)
{
    // Object reached through self->field_0014, then its skin-target provider at +0x4C.
    uint8_t* linkedObject = reinterpret_cast<uint8_t*>(self->field_0014);
    C_0082dd78* targetProvider = reinterpret_cast<C_0082dd78*>(CSF_AT(void*, linkedObject, 0x4C));

    C_008630e0* skinTarget = nullptr;
    if (targetProvider) {
        // Virtual slot 0x3C / 4 = 15: GetSkinTargetObject(), no stack arguments.
        skinTarget = reinterpret_cast<C_008630e0*>(
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, targetProvider, 0)[0x3C / 4])(targetProvider, nullptr));
    }

    char result = static_cast<char>(reinterpret_cast<uintptr_t>(skinTarget));

    if (self->field_0040 == 1) {
        result = QuerySkinTargetCompletion(skinTarget, nullptr);
        if (!result) {
            self->field_0018 &= ~1u;
            self->field_0040 = 0;
        }
    }

    return result;
}

HOOK(0x004A8BF0, C_00835140_UpdateSkinTargetFlag, "auto");
