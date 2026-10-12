#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, MissionScript_DestroyElementRange, 0x00469760, (int* first, int* last));
CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, ());
CSF_FUNC(int, __fastcall, GfxParticleEmitters_ResolveParticleResource, 0x006E15F0, (void* self, void* , C_0082d410* a2));

static int __stdcall GfxPtank_RemoveUnresolvedParticleResources(void* collection)
{
    auto* begin = CSF_AT(std::uint8_t*, collection, 0x4);
    int count = 0;
    if (begin)
    {
        const auto byteCount = static_cast<std::int32_t>(
            reinterpret_cast<std::uintptr_t>(
                CSF_AT(std::uint8_t*, collection, 0x8)) -
            reinterpret_cast<std::uintptr_t>(begin));
        count = byteCount / 28;
    }

    int result = count - 1;
    if (result >= 0)
    {
        int offset = 28 * result;
        int remaining = result + 1;
        do
        {
            auto* entry = reinterpret_cast<C_0082d410*>(
                CSF_AT(std::uint8_t*, collection, 0x4) + offset);
            const int resourceManager = GetValue_8BB738();
            if (!GfxParticleEmitters_ResolveParticleResource(
                    reinterpret_cast<void*>(resourceManager), nullptr, entry))
            {
                auto* element = CSF_AT(std::uint8_t*, collection, 0x4) + offset;
                auto* end = CSF_AT(std::uint8_t*, collection, 0x8);
                for (auto* source = element + 28; source != end; source += 28)
                {
                    AssignStringSubstring(source - 28, nullptr, source, 0, -1);
                }

                auto* last = CSF_AT(std::uint8_t*, collection, 0x8);
                MissionScript_DestroyElementRange(
                    reinterpret_cast<int*>(last - 28),
                    reinterpret_cast<int*>(last));
                CSF_AT(std::uint8_t*, collection, 0x8) =
                    CSF_AT(std::uint8_t*, collection, 0x8) - 28;
            }

            offset -= 28;
            result = --remaining;
        } while (remaining != 0);
    }

    return result;
}

HOOK(0x006D97D0, GfxPtank_RemoveUnresolvedParticleResources, "auto");
