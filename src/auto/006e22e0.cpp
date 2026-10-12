#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __cdecl, GfxParticleEmitters_DestroyRecordRange, 0x006E1710, (int a1, int a2));
CSF_FUNC(int, __fastcall, GfxParticleEmitters_ResetEmbeddedState, 0x00703670, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxParticleEmitters_ClearEmbeddedState, 0x00703710, (int* a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static void GfxParticleEmitters_ResetRecord(void* record, void* temporary)
{
    CSF_AT(std::uint32_t, temporary, 0x18) = 15;
    CSF_AT(std::uint32_t, temporary, 0x14) = 0;
    CSF_AT(std::uint8_t, temporary, 0x04) = 0;
    CSF_AT(std::uint32_t, temporary, 0x20) = 0;
    CSF_AT(std::uint32_t, temporary, 0x24) = 0;
    CSF_AT(float, temporary, 0x28) = 1.0f;
    CSF_AT(float, temporary, 0x2C) = 1.0f;
    CSF_AT(std::uint32_t, temporary, 0x1C) = 0;
    CSF_AT(std::uint32_t, temporary, 0x30) = 0;
    CSF_AT(std::uint32_t, temporary, 0x34) = 0;
    CSF_AT(std::uint32_t, temporary, 0x38) = 0;

    AssignStringSubstring(record, nullptr, temporary, 0, -1);

    CSF_AT(std::uint32_t, record, 0x1C) = CSF_AT(std::uint32_t, temporary, 0x1C);
    CSF_AT(std::uint32_t, record, 0x20) = CSF_AT(std::uint32_t, temporary, 0x20);
    CSF_AT(std::uint32_t, record, 0x24) = CSF_AT(std::uint32_t, temporary, 0x24);
    CSF_AT(float, record, 0x28) = CSF_AT(float, temporary, 0x28);
    CSF_AT(float, record, 0x2C) = CSF_AT(float, temporary, 0x2C);
    CSF_AT(std::uint32_t, record, 0x30) = CSF_AT(std::uint32_t, temporary, 0x30);
    CSF_AT(std::uint32_t, record, 0x34) = CSF_AT(std::uint32_t, temporary, 0x34);
    CSF_AT(std::uint32_t, record, 0x38) = CSF_AT(std::uint32_t, temporary, 0x38);

    if (CSF_AT(std::uint32_t, temporary, 0x18) >= 0x10)
    {
        if (CSF_AT(void*, temporary, 0x04))
            Mem_FreeDebug(CSF_AT(void*, temporary, 0x04));
    }

    CSF_AT(std::uint32_t, temporary, 0x18) = 15;
    CSF_AT(std::uint32_t, temporary, 0x14) = 0;
    CSF_AT(std::uint8_t, temporary, 0x04) = 0;
}

static int* __fastcall GfxParticleEmitters_ClearEmitterResources(void* self, void* /*edx*/)
{
    alignas(8) std::uint8_t temporary[0x44]{};
    auto* bytes = static_cast<std::uint8_t*>(self);

    GfxParticleEmitters_ClearEmbeddedState(reinterpret_cast<int*>(bytes + 0x20));
    GfxParticleEmitters_ResetEmbeddedState(bytes + 0x20, nullptr);

    std::uint32_t index = 0;
    std::uint32_t offset = 0;
    for (;; ++index, offset += 60)
    {
        const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x64);
        if (!begin)
            break;
        const std::int32_t byteCount = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x68) - begin);
        if (index >= static_cast<std::uint32_t>(byteCount / 60))
            break;

        auto* record = CSF_AT(std::uint8_t*, self, 0x64) + offset;
        if (CSF_AT(std::int32_t, record, 0x38) > 0)
        {
            const int global = GetGlobalInt();
            void* registry = reinterpret_cast<void*>(
                SceneLoader_GetObjectRegistry(reinterpret_cast<void*>(global), nullptr));
            record = CSF_AT(std::uint8_t*, self, 0x64) + offset;
            void* object = CSF_AT(void*, record, 0x1C);
            (reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, registry, 0)[0x2C / 4]))(registry, record, object);

            record = CSF_AT(std::uint8_t*, self, 0x64) + offset;
            GfxParticleEmitters_ResetRecord(record, temporary);
        }
    }

    if (CSF_AT(std::uint32_t, self, 0x64))
    {
        GfxParticleEmitters_DestroyRecordRange(
            CSF_AT(int, self, 0x64), CSF_AT(int, self, 0x68));
        if (CSF_AT(void*, self, 0x64))
            Mem_FreeDebug(CSF_AT(void*, self, 0x64));
    }
    CSF_AT(std::uint32_t, self, 0x64) = 0;
    CSF_AT(std::uint32_t, self, 0x68) = 0;
    CSF_AT(std::uint32_t, self, 0x6C) = 0;

    index = 0;
    offset = 0;
    for (;; ++index, offset += 60)
    {
        const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x74);
        if (!begin)
            break;
        const std::int32_t byteCount = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x78) - begin);
        if (index >= static_cast<std::uint32_t>(byteCount / 60))
            break;

        auto* record = CSF_AT(std::uint8_t*, self, 0x74) + offset;
        if (CSF_AT(std::int32_t, record, 0x38) > 0)
        {
            void* object = CSF_AT(void*, record, 0x1C);
            if (object)
            {
                (reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, object, 0)[4 / 4]))(object, nullptr, 1);
                CSF_AT(std::uint32_t,
                       CSF_AT(std::uint8_t*, self, 0x74) + offset, 0x1C) = 0;
            }

            record = CSF_AT(std::uint8_t*, self, 0x74) + offset;
            GfxParticleEmitters_ResetRecord(record, temporary);
        }
    }

    int* result = CSF_AT(int*, self, 0x74);
    if (result)
    {
        GfxParticleEmitters_DestroyRecordRange(
            reinterpret_cast<int>(result), CSF_AT(int, self, 0x78));
        result = CSF_AT(int*, self, 0x74);
        if (result)
            result = reinterpret_cast<int*>(Mem_FreeDebug(CSF_AT(void*, self, 0x74)));
    }
    CSF_AT(std::uint32_t, self, 0x74) = 0;
    CSF_AT(std::uint32_t, self, 0x78) = 0;
    CSF_AT(std::uint32_t, self, 0x7C) = 0;
    return result;
}

HOOK(0x006E22E0, GfxParticleEmitters_ClearEmitterResources, "auto");
