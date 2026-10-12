#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee: TextInputSubobject_Init (__thiscall, 1 stack arg, ret 4)
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Resets a SpawnPoints request object to its idle state and returns -1.
// Field offsets are not declared in types/auto_classes.h, so they are accessed by raw offset.
static int __fastcall SpawnPoints_InitializeRequest(uint8_t* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x28) = 0;
    TextInputSubobject_Init(self + 8, nullptr, &g_byte_82E65C);
    CSF_AT(int32_t, self, 0x24) = 0;
    CSF_AT(int32_t, self, 0x2C) = 0;
    CSF_AT(uint8_t, self, 0x3E) = 0;
    CSF_AT(uint8_t, self, 0x3D) = 0;
    CSF_AT(uint8_t, self, 0x3C) = 0;
    CSF_AT(int32_t, self, 0x30) = -1;
    CSF_AT(int32_t, self, 0x34) = -1;
    CSF_AT(int32_t, self, 0x00) = -1;
    CSF_AT(int32_t, self, 0x04) = -1;
    CSF_AT(int32_t, self, 0x38) = -1;
    return -1;
}

HOOK(0x00550AA0, SpawnPoints_InitializeRequest, "auto");
