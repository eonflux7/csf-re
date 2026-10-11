#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Game heap allocator (cdecl, one size argument).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Playability check for a sound request. Disassembly pushes three arguments
// (a2, a3, a4) and the caller pops 0xC, so the real signature takes three args. The first one is
// passed as a full dword (mov edx, [ebp+8]; push edx), not a char.
CSF_FUNC(char, __cdecl, SoundManager_CanPlayRequest, 0x0070DE80, (int a1, int a2, int a3));

// Sound instance initializer: thiscall with ecx = object, stack args (100, position pointer), ret 8.
CSF_FUNC(void**, __fastcall, SoundInstance_Init, 0x005282B0,
         (void* self, void* edx, unsigned int a2, void* a3));

// Starts a positional sound: checks playability, allocates a 24-byte instance and
// stores it in self->field_0040 (0 if allocation fails or there is no owner).
static char __fastcall CSoundInstance_StartSoundWithHandle(void* self, void* /*edx*/,
                                                           int a2, int a3, int a4)
{
    char canPlay = SoundManager_CanPlayRequest(a2, a3, a4);
    if (!canPlay)
        return canPlay;

    void* instanceMemory = Mem_Alloc(24);
    if (!instanceMemory)
    {
        CSF_AT(void*, self, 0x40) = nullptr;
        return canPlay;
    }

    // With an owner, pass its position block (self+8); without one, pass nullptr.
    if (self)
    {
        C_00841090* ownerPosition = reinterpret_cast<C_00841090*>(static_cast<uint8_t*>(self) + 8);
        CSF_AT(void*, self, 0x40) = reinterpret_cast<void*>(
            SoundInstance_Init(instanceMemory, nullptr, 0x64u, ownerPosition));
    }
    else
    {
        CSF_AT(void*, self, 0x40) = reinterpret_cast<void*>(
            SoundInstance_Init(instanceMemory, nullptr, 0x64u, nullptr));
    }
    return canPlay;
}

HOOK(0x521CF0, CSoundInstance_StartSoundWithHandle, "auto");

