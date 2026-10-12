#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(unsigned int, __fastcall, C_00860288_ConfigureObjectRenderRegions, 0x006C6C60, (C_00860288* self, void* , C_0085e7b8* object));

static int __fastcall C_00860288_SetRenderFlag(
    C_00860288* self, void* /*edx*/, int flag, int value)
{
    switch (flag)
    {
    case 8:
    case 16:
        if (static_cast<uint8_t>(value))
            self->field_0064 |= static_cast<uint32_t>(flag);
        else
            self->field_0064 &= static_cast<uint32_t>(~flag);

        return static_cast<int>(C_00860288_ConfigureObjectRenderRegions(
            reinterpret_cast<C_00860288*>(GetGlobalValue_6C5760()),
            nullptr,
            reinterpret_cast<C_0085e7b8*>(self)));

    case 32:
    case 0x8000:
        if (static_cast<uint8_t>(value))
        {
            self->field_0064 |= static_cast<uint32_t>(flag);
            return flag;
        }
        self->field_0064 &= static_cast<uint32_t>(~flag);
        return ~flag;

    case 64:
        return static_cast<int>(self->vSlot3C(static_cast<char>(value)));

    case 128:
        return static_cast<int>(self->vSlot48(value));

    case 256:
        return self->vSlot34(static_cast<char>(value));

    default:
        return flag;
    }
}

HOOK(0x006B35F0, C_00860288_SetRenderFlag, "auto");
