#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ParticleEmitters_MatchesEmitter, 0x006E1050, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, ParticleEmitters_AppendEmitter, 0x006E25C0, (void* self, void* edx, void* a3));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static int __fastcall S_006e20e0_RegisterEmitter(
    S_006e20e0* self, void* /*edx*/, std::uint32_t* definition)
{
    alignas(8) std::uint8_t emitterBytes[0x3C];
    C_0082d410& emitter = *reinterpret_cast<C_0082d410*>(emitterBytes);
    alignas(8) std::uint8_t requestBytes[0x30];

    CSF_AT(std::uint32_t, emitterBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, emitterBytes, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&emitter.field_0004)[0] = 0;
    CSF_AT(std::uint32_t, emitterBytes, 0x20) = 0;
    CSF_AT(std::uint32_t, emitterBytes, 0x24) = 0;
    CSF_AT(float, emitterBytes, 0x28) = 1.0f;
    CSF_AT(float, emitterBytes, 0x2C) = 1.0f;
    CSF_AT(std::uint32_t, emitterBytes, 0x1C) = 0;
    CSF_AT(std::uint32_t, emitterBytes, 0x30) = 0;
    CSF_AT(std::uint32_t, emitterBytes, 0x34) = 0;
    CSF_AT(std::uint32_t, emitterBytes, 0x38) = 0;

    AssignStringSubstring(&emitter, nullptr, definition + 1, 0, -1);

    CSF_AT(std::uint32_t, emitterBytes, 0x20) = definition[22];
    CSF_AT(std::uint32_t, emitterBytes, 0x24) = definition[23];
    CSF_AT(std::uint32_t, emitterBytes, 0x28) = definition[24];
    CSF_AT(std::uint32_t, emitterBytes, 0x2C) = definition[25];
    CSF_AT(std::uint32_t, emitterBytes, 0x34) = definition[31];
    CSF_AT(std::uint32_t, emitterBytes, 0x30) = definition[30];

    auto* emitters = reinterpret_cast<S_006e20e0*>(
        reinterpret_cast<std::uint8_t*>(self) + 0x60);

    std::uint32_t index = 0;
    std::uint32_t byteOffset = 0;
    int result = -1;
    bool found = false;

    for (;;)
    {
        const std::uint32_t begin = emitters->field_0004;
        if (!begin ||
            index >= static_cast<std::uint32_t>(
                static_cast<std::int32_t>(emitters->field_0008 - begin) / 60))
            break;

        if (found)
            break;

        auto* entry = reinterpret_cast<void*>(emitters->field_0004 + byteOffset);
        if (static_cast<std::uint8_t>(ParticleEmitters_MatchesEmitter(
                entry, nullptr, reinterpret_cast<int>(&emitter))) != 0)
        {
            if (CSF_AT(std::uint32_t, entry, 0x1C))
            {
                const std::uint32_t references =
                    CSF_AT(std::uint32_t, entry, 0x38) + 1;
                found = true;
                result = static_cast<int>(index);
                CSF_AT(std::uint32_t, entry, 0x38) = references;
            }
        }
        ++index;
        byteOffset += 60;
    }

    if (!found)
    {
        CSF_AT(std::uint32_t, emitterBytes, 0x38) = 1;
        CSF_AT(std::uint32_t, requestBytes, 0x04) = 0;
        CSF_AT(std::uint32_t, requestBytes, 0x00) =
            CSF_AT(std::uint32_t, emitterBytes, 0x18) < 0x10
                ? reinterpret_cast<std::uint32_t>(&emitter.field_0004)
                : emitter.field_0004;

        CSF_AT(std::uint32_t, requestBytes, 0x18) =
            CSF_AT(std::uint32_t, emitterBytes, 0x30);
        CSF_AT(std::uint32_t, requestBytes, 0x1C) =
            CSF_AT(std::uint32_t, emitterBytes, 0x34);
        CSF_AT(std::uint32_t, requestBytes, 0x08) =
            CSF_AT(std::uint32_t, emitterBytes, 0x20);
        CSF_AT(std::uint32_t, requestBytes, 0x0C) =
            CSF_AT(std::uint32_t, emitterBytes, 0x24);
        CSF_AT(std::uint32_t, requestBytes, 0x10) =
            CSF_AT(std::uint32_t, emitterBytes, 0x28);
        CSF_AT(std::uint32_t, requestBytes, 0x14) =
            CSF_AT(std::uint32_t, emitterBytes, 0x2C);
        CSF_AT(std::uint32_t, requestBytes, 0x20) = 0;

        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* registry = reinterpret_cast<void*>(
            SceneLoader_GetObjectRegistry(global, nullptr));
        CSF_AT(int, emitterBytes, 0x1C) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, registry, 0)[0x28 / 4])(
                    registry, nullptr, requestBytes);

        const std::uint32_t begin = emitters->field_0004;
        result = begin
            ? static_cast<std::int32_t>(emitters->field_0008 - begin) / 60
            : 0;
        ParticleEmitters_AppendEmitter(emitters, nullptr, &emitter);
    }

    if (CSF_AT(std::uint32_t, emitterBytes, 0x18) >= 0x10 &&
        emitter.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(emitter.field_0004));

    return result;
}

HOOK(0x006E27D0, S_006e20e0_RegisterEmitter, "auto");
