#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int32_t, g_PTankPluginOffset, 0x008B09EC);

static int __fastcall C_00862110_SetScaleAndFlag(C_00862110* self, void* /*edx*/, float a2, float a3)
{
    float scaleX;
    std::memcpy(&scaleX, &self->field_0028, sizeof(float));
    float scaledX = scaleX * a2;

    float scaleY;
    std::memcpy(&scaleY, &self->field_002c, sizeof(float));
    float scaledY = scaleY * a3;

    uint8_t* ptank = *reinterpret_cast<uint8_t**>(static_cast<uintptr_t>(g_PTankPluginOffset) + self->field_0004);
    CSF_AT(float, ptank, 0xB8) = scaledX;
    CSF_AT(float, ptank, 0xBC) = scaledY;

    uint8_t* result = *reinterpret_cast<uint8_t**>(static_cast<uintptr_t>(g_PTankPluginOffset) + self->field_0004);
    CSF_AT(uint32_t, result, 0x40) |= 0x4000u;
    return static_cast<int>(reinterpret_cast<uintptr_t>(result));
}

HOOK(0x006EEBA0, C_00862110_SetScaleAndFlag, "auto");
