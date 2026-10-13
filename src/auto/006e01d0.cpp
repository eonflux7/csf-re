#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, ParticleAtomic_ProcessEmitterSlots, 0x00702910, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_006dfeb0_RefreshState, 0x006DFEB0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, C_008656d0_ProcessIfFlagged, 0x006C6830, (C_008656d0* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, SkinAppendEntryIfEnabled, 0x006C62F0, (void* self, void* edx, int value));

static char __fastcall GfxWater_PrepareSceneObjects(S_006dfeb0* self, void* /*edx*/)
{
    // The emitter-slot pass takes the field value itself as `this`.
    if (self->field_0044)
        ParticleAtomic_ProcessEmitterSlots(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0044)), nullptr);

    S_006dfeb0_RefreshState(self, nullptr);

    uint32_t count = self->field_002c;
    for (uint32_t i = 0; i < count; ++i)
    {
        uint32_t entry = reinterpret_cast<uint32_t*>(self->field_0024)[i];
        if (entry)
        {
            C_008656d0* globalValue = reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760());
            C_008656d0_ProcessIfFlagged(globalValue, nullptr, static_cast<int>(entry));
        }
        count = self->field_002c;
    }

    uint32_t skinValue = self->field_0048;
    if (skinValue)
    {
        // One GetGlobalValue call; its result is SkinAppend's `this`.
        void* skinOwner = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_6C5760()));
        bool appended = SkinAppendEntryIfEnabled(skinOwner, nullptr, static_cast<int>(skinValue));
        // LOBYTE(v2) = result: only the low byte of the running value is replaced.
        count = (count & 0xFFFFFF00u) | static_cast<uint8_t>(appended);
    }
    return static_cast<char>(count);
}

HOOK(0x006E01D0, GfxWater_PrepareSceneObjects, "auto");
