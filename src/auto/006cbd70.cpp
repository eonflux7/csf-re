#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __fastcall, GfxScene_TestObjectSphereOverlap, 0x006CA870,
         (void* self, void* edx, float* a2, float a3));

static char __fastcall GfxScene_HasSphereOverlap(
    void* self, void* /*edx*/, float* sphere, std::uint32_t* cache)
{
    if (CSF_AT(std::uint8_t, self, 0x94) == 0)
        cache = nullptr;

    if (cache)
    {
        const std::int32_t cachedIndex = CSF_AT(std::int32_t, cache, 0);
        void* cachedObject = nullptr;

        if (cachedIndex >= 0 &&
            cachedIndex < CSF_AT(std::int32_t, self, 0x7C))
        {
            auto* entry = CSF_AT(std::uint8_t*, self, 0x74) +
                          8 * cachedIndex;
            if (CSF_AT(std::uint8_t, entry, 4))
                cachedObject = CSF_AT(void*, entry, 0);
        }

        if (cachedObject)
        {
            if (GfxScene_TestObjectSphereOverlap(
                    cachedObject, nullptr, sphere, sphere[3]))
            {
                CSF_AT(std::uint32_t, cache, 4) =
                    CSF_AT(std::uint32_t, self, 0xF4);
                return 1;
            }
            CSF_AT(std::int32_t, cache, 0) = -1;
        }
        else if (CSF_AT(std::uint32_t, self, 0xF4) -
                     CSF_AT(std::uint32_t, cache, 4) <= 0x28)
        {
            return 0;
        }
    }

    if (CSF_AT(std::uint32_t, self, 0x8C) == 0)
        return 0;

    std::uint32_t index = 0;
    auto* entry = CSF_AT(std::uint8_t*, self, 0x84);
    for (;;)
    {
        if (GfxScene_TestObjectSphereOverlap(
                CSF_AT(void*, entry, 0), nullptr, sphere, sphere[3]))
            break;

        const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x8C);
        ++index;
        entry += 8;
        if (index >= count)
            return 0;
    }

    if (cache)
    {
        CSF_AT(std::uint32_t, cache, 4) =
            CSF_AT(std::uint32_t, self, 0xF4);
        auto* entries = CSF_AT(std::uint8_t*, self, 0x84);
        CSF_AT(std::uint32_t, cache, 0) =
            CSF_AT(std::uint32_t, entries + 8 * index, 4);
    }

    return 1;
}

HOOK(0x006CBD70, GfxScene_HasSphereOverlap, "auto");
