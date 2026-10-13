#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, C_00860a30_SetupRenderTargetFromDesc, 0x006CA410, (C_00860a30* self, void* /*edx*/, int a2));
CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, (std::uint32_t value));
CSF_FUNC(void, __fastcall, GfxParticleEmitters_ReleaseEntry, 0x006E17B0, (void* self, void* /*edx*/, int entryIndex));
CSF_FUNC(void, __fastcall, ParticleEmitters_ReleaseEntry, 0x006E1860, (void* self, void* /*edx*/, int index));
CSF_FUNC(int, __fastcall, S_006e20e0_RegisterEmitter, 0x006E27D0, (S_006e20e0* self, void* /*edx*/, std::uint32_t* definition));
CSF_FUNC(int, __fastcall, S_006e2150_FindOrCreateParticleState, 0x006E2980, (S_006e2150* self, void* /*edx*/, std::uint32_t* parameters));

// Replaces the emitter/particle-state handles of a C_00862598 from a descriptor:
// releases the old handles, re-registers both from the descriptor, copies two values
// and returns the last one.
static int __fastcall C_00862598_ReloadEmitterFromDesc(C_00862598* self, void* /*edx*/, std::uint32_t* a2)
{
    C_00860a30_SetupRenderTargetFromDesc(static_cast<C_00860a30*>(self), nullptr, reinterpret_cast<int>(a2));

    // Release the old gfx emitter entry.
    std::uint32_t oldEmitterEntry = self->field_0024;
    int gfxOwner = GetValue_8BB738(oldEmitterEntry);
    GfxParticleEmitters_ReleaseEntry(reinterpret_cast<void*>(static_cast<std::intptr_t>(gfxOwner)),
                                     nullptr, static_cast<int>(oldEmitterEntry));

    // Release the old particle emitter entry.
    std::uint32_t oldParticleState = self->field_0028;
    int particleOwner = GetValue_8BB738(oldParticleState);
    ParticleEmitters_ReleaseEntry(reinterpret_cast<void*>(static_cast<std::intptr_t>(particleOwner)),
                                  nullptr, static_cast<int>(oldParticleState));

    // Register a new emitter from the descriptor.
    int registerOwner = GetValue_8BB738(reinterpret_cast<std::uint32_t>(a2));
    self->field_0024 = static_cast<std::uint32_t>(S_006e20e0_RegisterEmitter(
        reinterpret_cast<S_006e20e0*>(static_cast<std::intptr_t>(registerOwner)), nullptr, a2));

    // Find or create the particle state from the descriptor.
    int stateOwner = GetValue_8BB738(reinterpret_cast<std::uint32_t>(a2));
    self->field_0028 = static_cast<std::uint32_t>(S_006e2150_FindOrCreateParticleState(
        reinterpret_cast<S_006e2150*>(static_cast<std::intptr_t>(stateOwner)), nullptr, a2));

    self->field_003c = a2[20];
    int result = static_cast<int>(a2[21]);
    self->field_0040 = static_cast<std::uint32_t>(result);
    return result;
}

HOOK(0x006F2550, C_00862598_ReloadEmitterFromDesc, "auto");
