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

CSF_FUNC(void, __fastcall, C_0082d844_SetDirectionalValue, 0x004093F0, (C_0082d844* self, void* , float a2, char a3));

static void __fastcall C_0083bc28_ResetSkinDirectionAndFlags(C_0083bc28* self, void* /*edx*/)
{
    // this->field_0014->field_004c->vGetSkinTargetObject()
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0014);
    void* target = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x4C));
    void* skinObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, target, 0)[0x3C / 4])(target, nullptr);

    C_0082d844_SetDirectionalValue(reinterpret_cast<C_0082d844*>(skinObject), nullptr, 0.0f, 1);

    self->field_0044 &= 0xFFFFFF3Du;

    uint32_t flagsObject = self->field_0054;
    if (flagsObject) {
        uint8_t* flagsBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(flagsObject));
        CSF_AT(uint32_t, flagsBase, 4) |= 1u;
    }
}

HOOK(0x4D3870, C_0083bc28_ResetSkinDirectionAndFlags, "auto");
