#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound original callees (signatures copied from the packet).
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SpawnPoint_HandleMultiplayerRequest, 0x00749510,
         (void* self, void* edx, int arg0, int a3, int a4, int a5, int a6, int a7, uint16_t a2));
CSF_FUNC(char, __fastcall, SpawnPoint_HandleSingleplayerRequest, 0x00750D00,
         (void* self, void* edx, char arg0, char a3, char a4, char a5, char a6, char a7, char a2));

// Original: char __thiscall SpawnPoint_DispatchRequest(this, arg0, a3, a4, a5, a6, a7, a2)
// Seven stack arguments, callee pops 28 bytes (ret 0x1C).
static char __fastcall SpawnPoint_DispatchRequest(C_00858180* self, void* /*edx*/,
                                                  char arg0, char a3, char a4, char a5,
                                                  char a6, char a7, char a2)
{
    if (Sub_64B250(self, nullptr)) {
        return SpawnPoint_HandleMultiplayerRequest(self, nullptr, arg0, a3, a4, a5, a6, a7, a2);
    }
    return SpawnPoint_HandleSingleplayerRequest(self, nullptr, arg0, a3, a4, a5, a6, a7, a2);
}

HOOK(0x00749660, SpawnPoint_DispatchRequest, "auto");
