#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sets or clears flag bit 0x100 in field_0064. If bit 0x200 is then set, it
// queries the child object at field_006c (vtbl slot 0x28) and forwards the
// matching notification (slot 0x20 when enabling, slot 0x24 when disabling).
static char __fastcall C_00860288_SetBit100AndNotifyChild(C_00860288* self, void* /*edx*/, char enable)
{
    char result = enable;

    uint32_t flags = self->field_0064;
    if (enable)
        flags |= 0x100;
    else
        flags &= 0xFFFFFEFFu;
    self->field_0064 = flags;

    if ((flags & 0x200) != 0)
    {
        if (enable)
        {
            void* child = reinterpret_cast<void*>(self->field_006c);
            result = ((char(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x28 / 4])(child, nullptr);
            if (!result)
            {
                child = reinterpret_cast<void*>(self->field_006c);
                return ((char(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x20 / 4])(child, nullptr);
            }
        }
        else
        {
            void* child = reinterpret_cast<void*>(self->field_006c);
            result = ((char(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x28 / 4])(child, nullptr);
            if (result)
            {
                child = reinterpret_cast<void*>(self->field_006c);
                return ((char(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x24 / 4])(child, nullptr);
            }
        }
    }

    return result;
}

HOOK(0x006B3690, C_00860288_SetBit100AndNotifyChild, "auto");
