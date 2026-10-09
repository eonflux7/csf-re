#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static int __fastcall MenuMessageBox_ResetEmbeddedState(char* self, void* /*edx*/)
{
    CSF_AT(int, self, 0x00) = 1;
    CSF_AT(uint8_t, self, 0xA8) = 1;
    CSF_AT(uint8_t, self, 0x1A) = 1;
    CSF_AT(uint8_t, self, 0xA9) = 1;
    CSF_AT(int, self, 0x04) = 0;
    CSF_AT(int, self, 0x08) = 0;
    CSF_AT(uint8_t, self, 0x0C) = 0;
    CSF_AT(int, self, 0x10) = 0;
    CSF_AT(int, self, 0x14) = 0;
    CSF_AT(uint8_t, self, 0x18) = 0;
    CSF_AT(uint8_t, self, 0x19) = 0;
    CSF_AT(int, self, 0xB0) = -1;
    CSF_AT(int, self, 0xAC) = -1;

    AssignStringRange_429B60(self + 0x1C, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);
    AssignStringRange_429B60(self + 0x38, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);
    AssignStringRange_429B60(self + 0x54, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);
    AssignStringRange_429B60(self + 0x70, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);
    void* lastResult = AssignStringRange_429B60(self + 0x8C, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);
    return static_cast<int>(reinterpret_cast<intptr_t>(lastResult));
}

HOOK(0x005017E0, MenuMessageBox_ResetEmbeddedState, "auto");
