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

// Text input subobject initializer (__thiscall, 1 stack arg).
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));

// Spawn request processor (__thiscall, 1 stack arg): receives a pointer to the 2-dword bounds block.
CSF_FUNC(void*, __fastcall, SpawnRequest_Process, 0x00555E40, (void* self, void* edx, int a2));

// Game debug free (__cdecl).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static void* __fastcall MpSpawn_RunSpawnRequest(S_00555e40* self, void* /*edx*/, int a2, int a3, int a4, int a5)
{
    // 0x38-byte text input subobject (ebp-0x38 .. ebp-0x1).
    alignas(4) uint8_t textInput[0x38];

    // Bounds block passed to SpawnRequest_Process (ebp-0x40).
    int searchBounds[2] = {-1, -1};

    CSF_AT(uint32_t, textInput, 0x18) = 7;     // capacity (v11)
    CSF_AT(int, textInput, 0x14) = 0;          // v10
    CSF_AT(uint16_t, textInput, 0x04) = 0;     // Block (low word cleared)
    CSF_AT(uint8_t, textInput, 0x20) = 0;      // v13

    TextInputSubobject_Init(textInput, nullptr, &g_byte_82E65C);

    CSF_AT(uint8_t, textInput, 0x20) = 1;      // v13
    CSF_AT(int, textInput, 0x1C) = 1;          // v12
    CSF_AT(int, textInput, 0x2C) = a4;         // v16
    CSF_AT(int, textInput, 0x28) = a3;         // v15
    CSF_AT(uint8_t, textInput, 0x36) = 0;      // v20
    CSF_AT(uint8_t, textInput, 0x35) = 0;      // v19
    CSF_AT(uint8_t, textInput, 0x34) = 0;      // v18
    CSF_AT(int, textInput, 0x24) = a2;         // v14
    CSF_AT(int, textInput, 0x30) = a5;         // v17

    void* result = SpawnRequest_Process(self, nullptr, reinterpret_cast<int>(searchBounds));

    if (static_cast<uint32_t>(CSF_AT(int, textInput, 0x18)) >= 8) {
        void* block = CSF_AT(void*, textInput, 0x04);
        result = block;
        if (block != nullptr) {
            return reinterpret_cast<void*>(Mem_FreeDebug(block));
        }
    }
    return result;
}

HOOK(0x005567F0, MpSpawn_RunSpawnRequest, "auto");
