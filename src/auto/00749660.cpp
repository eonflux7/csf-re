// Stack-slot dwords are forwarded unchanged; the callees' own declarations
// decide how many low bits they actually read.
#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SpawnPoint_HandleMultiplayerRequest, 0x00749510,
         (void* self, void* edx, int arg0, int a3, int a4, int a5, int a6, int a7, uint16_t a2));
CSF_FUNC(char, __fastcall, SpawnPoint_HandleSingleplayerRequest, 0x00750D00,
         (void* self, void* edx, char arg0, char a3, char a4, char a5, char a6, char a7, char a2));

// Picks the multiplayer or singleplayer spawn point handler based on Sub_64B250.
// Seven stack arguments (ret 28) are forwarded unchanged.
static char __fastcall C_00858180_DispatchSpawnRequest(C_00858180* self, void* /*edx*/,
                                                       int arg0, int a3, int a4, int a5,
                                                       int a6, int a7, int a2)
{
    if (Sub_64B250(self, nullptr))
        return SpawnPoint_HandleMultiplayerRequest(self, nullptr, arg0, a3, a4, a5, a6, a7,
                                                   static_cast<uint16_t>(a2));
    return SpawnPoint_HandleSingleplayerRequest(self, nullptr, static_cast<char>(arg0),
                                                static_cast<char>(a3), static_cast<char>(a4),
                                                static_cast<char>(a5), static_cast<char>(a6),
                                                static_cast<char>(a7), static_cast<char>(a2));
}

HOOK(0x749660, C_00858180_DispatchSpawnRequest, "auto");
