#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(unsigned int, __fastcall, GameHeapSubsystem_Reset, 0x0069DDE0, (void* self, void* edx));

CSF_GLOBAL(int32_t, g_dword_8BAD64, 0x008BAD64);
CSF_GLOBAL(uint32_t, g_dword_8BAD68, 0x008BAD68);
CSF_GLOBAL(uint32_t, g_dword_8BAD6C, 0x008BAD6C);
CSF_GLOBAL(uint32_t, g_dword_8BAD70, 0x008BAD70);

// Initializes the game heap bookkeeping: resets the heap subsystem object,
// clears its 0xA0-byte state block, and installs the allocator entry points.
static int GameHeap_Initialize()
{
    GameHeapSubsystem_Reset(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008BAD98)), nullptr);
    memset(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008BACC0)), 0, 0xA0);
    g_dword_8BAD64 = (int32_t)CSF_ADDR_FN(0x00691120);
    g_dword_8BAD68 = (uint32_t)CSF_ADDR_FN(0x00691180);
    g_dword_8BAD6C = (uint32_t)CSF_ADDR_FN(0x00691190);
    g_dword_8BAD70 = (uint32_t)CSF_ADDR_FN(0x00691260);
    return 0;
}

HOOK(0x006912A0, GameHeap_Initialize, "auto");
