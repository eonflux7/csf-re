#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x747830: __thiscall-style body with six char stack args (ret 24).
CSF_FUNC(char, __fastcall, SpawnPoint_LaunchSpawnRequest, 0x00747830,
         (void* self, void* edx, char a2, char a3, char a4, char a5, char a6, char a7));

// Original 0x551000 (__stdcall, ret 4): walks owner -> child -> spawn object, asks the
// embedded sub-object (vtable slot 2) whether it is ready, and if so launches the spawn request.
static char __stdcall MpSpawn_ActivateSpawnPointIfReady(void* self)
{
    uint8_t* owner = CSF_AT(uint8_t*, self, 0x40);
    uint32_t eaxValue = reinterpret_cast<uint32_t>(owner);

    if (owner != nullptr)
    {
        uint8_t* child = CSF_AT(uint8_t*, owner, 0x18);
        eaxValue = reinterpret_cast<uint32_t>(child);

        if (child != nullptr)
        {
            // Original computes edi = child - 0x70; a null result returns the raw low byte of child (0x70).
            uint8_t* spawn = child - 0x70;
            if (spawn != nullptr)
            {
                // Embedded sub-object at +0x70: its vtable is its first dword; this = its address.
                uint8_t* readyCheck = spawn + 0x70;
                void** readyVtbl = CSF_AT(void**, readyCheck, 0);
                char isReady = ((char(__fastcall*)(void*, void*))readyVtbl[8 / 4])(readyCheck, nullptr);

                if (isReady != 0)
                {
                    char a2 = static_cast<char>(CSF_AT(uint32_t, self, 0x34));
                    char a3 = static_cast<char>(CSF_AT(uint32_t, self, 0x38));
                    char a4 = static_cast<char>(CSF_AT(uint32_t, self, 0x3C));
                    char a5 = static_cast<char>(CSF_AT(uint32_t, self, 0x48));
                    char a6 = static_cast<char>(CSF_AT(uint32_t, self, 0x4C));
                    char a7 = static_cast<char>(CSF_AT(uint32_t, self, 0x50));
                    return SpawnPoint_LaunchSpawnRequest(spawn, nullptr, a2, a3, a4, a5, a6, a7);
                }

                // Not ready: the original returns the low byte of the vtable call's eax (al == 0).
                return 0;
            }
        }
    }

    // Null path: returns low byte of the last value left in eax.
    return static_cast<char>(eaxValue);
}

HOOK(0x00551000, MpSpawn_ActivateSpawnPointIfReady, "auto");
