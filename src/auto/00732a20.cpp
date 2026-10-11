#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_ValidateName, 0x00696BB0, (void* name, void* edx, const uint8_t* separator));
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static int __fastcall C_008670f0_SetDimensions(
    C_008670f0* self, void* /*edx*/, int width, int height)
{
    int result = SaveGame_ValidateName(
        reinterpret_cast<uint8_t*>(self) + 0x20, nullptr, &g_byte_82E65C);

    if (static_cast<uint8_t>(result) == 0)
    {
        result = width;
        if (static_cast<uint32_t>(width) != self->field_0018 ||
            static_cast<uint32_t>(height) != self->field_001c)
        {
            if (width && height)
            {
                self->field_0018 = width;
                self->field_001c = height;
            }
            else
            {
                self->field_0018 = 0;
                self->field_001c = 0;
            }
        }
    }
    return result;
}

HOOK(0x00732A20, C_008670f0_SetDimensions, "auto");
