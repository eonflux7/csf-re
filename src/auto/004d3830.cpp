#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d844_SetDirectionalValue, 0x004093F0, (C_0082d844* self, void* , float a2, char a3));

static uint32_t __fastcall C_0083bc28_ApplySkinTargetDirection(
    C_0083bc28* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0014);
    void* skinOwner = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x4C));

    C_0082d844* skinTarget =
        reinterpret_cast<C_0082d844* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, skinOwner, 0)[0x3C / 4])(skinOwner, nullptr);

    C_0082d844_SetDirectionalValue(skinTarget, nullptr, 90.0f, 1);

    uint32_t flags = (self->field_0044 & 0xFFFFFF3Fu) | 2;
    self->field_0044 = flags;

    void* pendingObject = reinterpret_cast<void*>(self->field_0054);
    if (pendingObject)
        CSF_AT(uint32_t, pendingObject, 0x04) &= ~1u;

    return flags;
}

HOOK(0x004D3830, C_0083bc28_ApplySkinTargetDirection, "auto");
