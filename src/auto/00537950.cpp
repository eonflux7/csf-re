#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_GLOBAL(uint8_t, g_EmptyStringByte, 0x0082E65C);

static int __fastcall C_00843720_ResetState(C_00843720* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x273) = 1;
    CSF_AT(uint8_t, self, 0x150) = 0;
    CSF_AT(uint8_t, self, 0x151) = 0;
    CSF_AT(uint8_t, self, 0xFC) = 0;
    CSF_AT(uint8_t, self, 0x17C) = 1;
    CSF_AT(uint8_t, self, 0x17D) = 0;
    CSF_AT(uint8_t, self, 0x17E) = 0;
    CSF_AT(uint8_t, self, 0x186) = 0;

    AssignStringRange_429B60(
        reinterpret_cast<uint8_t*>(self) + 0x160, nullptr,
        reinterpret_cast<char*>(&g_EmptyStringByte), 0);

    CSF_AT(uint32_t, self, 0x100) = 0;
    CSF_AT(uint32_t, self, 0x104) = 0;
    CSF_AT(uint8_t, self, 0x108) = 0;
    CSF_AT(uint8_t, self, 0x109) = 0;
    CSF_AT(uint32_t, self, 0x10C) = 0;
    CSF_AT(uint8_t, self, 0x110) = 1;
    CSF_AT(uint8_t, self, 0x111) = 0;
    CSF_AT(uint8_t, self, 0x112) = 0;
    CSF_AT(uint8_t, self, 0x17F) = 0;
    CSF_AT(uint8_t, self, 0x181) = 0;
    CSF_AT(uint8_t, self, 0x182) = 0;
    CSF_AT(uint8_t, self, 0x183) = 0;
    CSF_AT(uint8_t, self, 0x184) = 0;
    CSF_AT(uint8_t, self, 0x188) = 0;
    CSF_AT(uint8_t, self, 0x185) = 0;
    CSF_AT(uint8_t, self, 0x142) = 0;
    CSF_AT(uint8_t, self, 0x180) = 0;
    CSF_AT(uint32_t, self, 0x130) = 2;
    CSF_AT(uint32_t, self, 0x134) = 0;

    void* result = AssignStringRange_429B60(
        reinterpret_cast<uint8_t*>(self) + 0x114, nullptr,
        reinterpret_cast<char*>(&g_EmptyStringByte), 0);

    CSF_AT(uint8_t, self, 0x141) = 0;
    CSF_AT(uint8_t, self, 0x271) = 0;
    CSF_AT(uint8_t, self, 0x272) = 0;
    CSF_AT(uint8_t, self, 0x140) = 1;
    CSF_AT(int32_t, self, 0x148) = -1;

    return static_cast<int>(reinterpret_cast<intptr_t>(result));
}

HOOK(0x00537950, C_00843720_ResetState, "auto");
