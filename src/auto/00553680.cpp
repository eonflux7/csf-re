#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address; see packet callee list.
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());

// Takes one argument in ecx (mov ecx, eax; call), returns a pointer-sized handle.
// Stack pops nothing (ret 0). Packet proto is unclear; bound as fastcall with an int in ecx.
CSF_FUNC(int, __fastcall, LookupSpawnHandleByIndex_73E5F0, 0x0073E5F0, (int index, void* edx));

static int __fastcall SpawnPoints_ResetSlots(S_00553680* self, void* /*edx*/)
{
    int globalIndex = GetGlobalDword_73C020();
    uint32_t handle = static_cast<uint32_t>(LookupSpawnHandleByIndex_73E5F0(globalIndex, nullptr));

    // handle != 0 and handle != 128: the object lives at handle - 0x80.
    if (handle != 0 && handle != 128) {
        void* entry = reinterpret_cast<void*>(static_cast<uintptr_t>(handle - 128));
        // Virtual slot 0 on the entry, called with a single argument of 1.
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, entry, 0)[0])(entry, nullptr, 1);
    }

    self->field_1408 = 0xFFFFFFFFu;
    self->field_140c = 0xFFFFFFFFu;
    self->field_1414 = 0xFFFFFFFFu;
    self->field_1418 = 0xFFFFFFFFu;
    return -1;
}

HOOK(0x00553680, SpawnPoints_ResetSlots, "auto");
