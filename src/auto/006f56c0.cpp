#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, GfxCollision_Reset, 0x006EC2A0, (void* self, void* /*edx*/));

// Calls virtual slot 2 (byte offset 8) on every non-null entry of the child
// array, then resets the collision block embedded at +0x14.
static char __fastcall C_008627d8_NotifyChildrenAndResetCollision(C_008627d8* self, void* /*edx*/)
{
    uint32_t childCount = self->field_000c;
    for (uint32_t index = 0; index < childCount; ++index)
    {
        uint32_t entry = reinterpret_cast<uint32_t*>(self->field_0004)[index];
        if (entry)
        {
            void* child = reinterpret_cast<void*>(entry);
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[2])(child, nullptr);
        }
    }
    return GfxCollision_Reset(reinterpret_cast<uint8_t*>(self) + 0x14, nullptr);
}

HOOK(0x6F56C0, C_008627d8_NotifyChildrenAndResetCollision, "auto");
