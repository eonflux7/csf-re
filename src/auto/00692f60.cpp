#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

using ComponentPoolEntries = std::uint32_t[256];

CSF_GLOBAL(ComponentPoolEntries, g_ComponentPools, 0x008BAF48);
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));

static void __fastcall CoreZlibLog_ReadVector2(
    std::uint8_t* self, void* /*edx*/, float* output)
{
    std::uint32_t component;

    if (CSF_AT(std::uint8_t, self, 0xB) == 0xFF)
    {
        auto* object = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, self, 0));
        component = CSF_AT(std::uint32_t, object, 0x18);
    }
    else
    {
        auto* pool = reinterpret_cast<std::uint8_t*>(
            g_ComponentPools[CSF_AT(std::uint8_t, self, 0xB)]);
        const std::uint32_t base = CSF_AT(std::uint32_t, pool, 0);
        const std::uint32_t nextIndex =
            (reinterpret_cast<std::uint32_t>(self) - base) / 0xC + 1;

        component = CSF_AT(std::uint32_t, pool, 0) + 12 * nextIndex;
        if (!CSF_AT(std::uint8_t, pool, 0x14))
        {
            CSF_AT(std::uint8_t,
                   reinterpret_cast<std::uint8_t*>(component), 0xB) =
                CSF_AT(std::uint8_t, pool, 0x15);
        }
    }

    output[0] = static_cast<float>(ReadComponentFloat(component));

    if (CSF_AT(std::uint8_t, self, 0xB) == 0xFF)
    {
        auto* object = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, self, 0));
        component = CSF_AT(std::uint32_t, object, 0x1C);
    }
    else
    {
        auto* pool = reinterpret_cast<std::uint8_t*>(
            g_ComponentPools[CSF_AT(std::uint8_t, self, 0xB)]);
        const std::uint32_t base = CSF_AT(std::uint32_t, pool, 0);
        const std::uint32_t nextIndex =
            (reinterpret_cast<std::uint32_t>(self) - base) / 0xC + 1;
        auto* nextEntry =
            reinterpret_cast<std::uint8_t*>(base + 12 * nextIndex);

        component = base + 12 * CSF_AT(std::uint32_t, nextEntry, 0);
        if (!CSF_AT(std::uint8_t, pool, 0x14))
        {
            CSF_AT(std::uint8_t,
                   reinterpret_cast<std::uint8_t*>(component), 0xB) =
                CSF_AT(std::uint8_t, pool, 0x15);
        }
    }

    output[1] = static_cast<float>(ReadComponentFloat(component));
}

HOOK(0x00692F60, CoreZlibLog_ReadVector2, "auto");
