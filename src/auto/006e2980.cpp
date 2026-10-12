#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, ParticleEmitters_MatchesState, 0x006E1050, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, ParticleEmitters_AppendState, 0x006E2650, (void* self, void* edx, void* a3));
CSF_FUNC(bool, __fastcall, ParticleRender_CaptureStateAndQuery, 0x007051B0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_008631a8_InitParticleDefaults, 0x00705230, (void* self, void* edx));

static int __fastcall S_006e2150_FindOrCreateParticleState(
    S_006e2150* self, void* /*edx*/, std::uint32_t* parameters)
{
    alignas(8) std::uint8_t stateBytes[0x3C]{};
    C_0082d410& state = *reinterpret_cast<C_0082d410*>(stateBytes);

    CSF_AT(std::uint32_t, stateBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, stateBytes, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&state.field_0004)[0] = 0;
    CSF_AT(std::uint32_t, stateBytes, 0x20) = 0;
    CSF_AT(std::uint32_t, stateBytes, 0x24) = 0;
    CSF_AT(float, stateBytes, 0x28) = 1.0f;
    CSF_AT(float, stateBytes, 0x2C) = 1.0f;
    CSF_AT(std::uint32_t, stateBytes, 0x1C) = 0;
    CSF_AT(std::uint32_t, stateBytes, 0x30) = 0;
    CSF_AT(std::uint32_t, stateBytes, 0x34) = 0;
    CSF_AT(std::uint32_t, stateBytes, 0x38) = 0;

    AssignStringSubstring(&state, nullptr, parameters + 1, 0, -1);

    CSF_AT(std::uint32_t, stateBytes, 0x20) = parameters[26];
    CSF_AT(std::uint32_t, stateBytes, 0x24) = parameters[27];
    CSF_AT(std::uint32_t, stateBytes, 0x28) = parameters[28];
    CSF_AT(std::uint32_t, stateBytes, 0x2C) = parameters[29];
    CSF_AT(std::uint32_t, stateBytes, 0x34) = parameters[31];
    CSF_AT(std::uint32_t, stateBytes, 0x30) = parameters[30];

    auto* states = reinterpret_cast<S_006e2150*>(
        reinterpret_cast<std::uint8_t*>(self) + 0x70);

    std::uint32_t index = 0;
    std::uint32_t byteOffset = 0;
    bool found = false;
    int result = -1;

    for (;;)
    {
        const std::uint32_t begin = states->field_0004;
        if (!begin)
            break;

        const int count =
            static_cast<std::int32_t>(states->field_0008 - begin) / 60;
        if (index >= static_cast<std::uint32_t>(count))
            break;
        if (found)
            break;

        auto* entry = reinterpret_cast<void*>(states->field_0004 + byteOffset);
        if (static_cast<std::uint8_t>(ParticleEmitters_MatchesState(
                entry, nullptr, reinterpret_cast<int>(&state))) != 0)
        {
            if (CSF_AT(std::uint32_t, entry, 0x1C))
            {
                found = true;
                result = static_cast<int>(index);
                ++CSF_AT(std::uint32_t, entry, 0x38);
            }
        }

        ++index;
        byteOffset += 60;
    }

    if (!found)
    {
        CSF_AT(std::uint32_t, stateBytes, 0x38) = 1;

        alignas(8) std::uint8_t captureBytes[0x24]{};
        auto* capture = reinterpret_cast<std::uint32_t*>(captureBytes);

        capture[0] = CSF_AT(std::uint32_t, stateBytes, 0x18) < 0x10
            ? reinterpret_cast<std::uint32_t>(&state.field_0004)
            : state.field_0004;
        capture[1] = CSF_AT(std::uint32_t, stateBytes, 0x20);
        capture[2] = CSF_AT(std::uint32_t, stateBytes, 0x24);
        capture[3] = CSF_AT(std::uint32_t, stateBytes, 0x28);
        capture[4] = CSF_AT(std::uint32_t, stateBytes, 0x2C);
        capture[5] = CSF_AT(std::uint32_t, stateBytes, 0x30);
        capture[6] = CSF_AT(std::uint32_t, stateBytes, 0x34);

        void* allocation = Mem_Alloc(60);
        int particleState = allocation
            ? C_008631a8_InitParticleDefaults(allocation, nullptr)
            : 0;
        CSF_AT(int, stateBytes, 0x1C) = particleState;

        ParticleRender_CaptureStateAndQuery(
            reinterpret_cast<void*>(particleState), nullptr, capture);

        const std::uint32_t begin = states->field_0004;
        result = begin
            ? static_cast<std::int32_t>(states->field_0008 - begin) / 60
            : 0;

        ParticleEmitters_AppendState(states, nullptr, &state);
    }

    if (CSF_AT(std::uint32_t, stateBytes, 0x18) >= 0x10 &&
        state.field_0004)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(state.field_0004));
    }

    return result;
}

HOOK(0x006E2980, S_006e2150_FindOrCreateParticleState, "auto");
