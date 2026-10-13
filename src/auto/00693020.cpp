#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

using ComponentPoolEntries = std::uint32_t[256];
CSF_GLOBAL(ComponentPoolEntries, g_ComponentPools, 0x008BAF48);

CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));

static int __fastcall DataNode_ReadVector3(
    std::uint8_t* self, void* /*edx*/, float* output)
{
    const auto nodeAddress = reinterpret_cast<std::uint32_t>(self);
    std::uint32_t component;

    if (CSF_AT(std::uint8_t, self, 0xB) == 0xFF)
    {
        auto* node = reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0));
        component = CSF_AT(std::uint32_t, node, 0x18);
    }
    else
    {
        auto* pool = reinterpret_cast<void*>(
            g_ComponentPools[CSF_AT(std::uint8_t, self, 0xB)]);
        const auto base = CSF_AT(std::uint32_t, pool, 0);
        component = base + 12 * ((nodeAddress - base) / 0xC + 1);
        if (!CSF_AT(std::uint8_t, pool, 0x14))
        {
            CSF_AT(std::uint8_t, reinterpret_cast<void*>(component), 0xB) =
                CSF_AT(std::uint8_t, pool, 0x15);
        }
    }
    output[0] = static_cast<float>(ReadComponentFloat(component));

    if (CSF_AT(std::uint8_t, self, 0xB) == 0xFF)
    {
        auto* node = reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0));
        component = CSF_AT(std::uint32_t, node, 0x1C);
    }
    else
    {
        auto* pool = reinterpret_cast<void*>(
            g_ComponentPools[CSF_AT(std::uint8_t, self, 0xB)]);
        const auto base = CSF_AT(std::uint32_t, pool, 0);
        const auto first = base + 12 * ((nodeAddress - base) / 0xC + 1);
        component = base + 12 * CSF_AT(
            std::uint32_t, reinterpret_cast<void*>(first), 0);
        if (!CSF_AT(std::uint8_t, pool, 0x14))
        {
            CSF_AT(std::uint8_t, reinterpret_cast<void*>(component), 0xB) =
                CSF_AT(std::uint8_t, pool, 0x15);
        }
    }
    output[1] = static_cast<float>(ReadComponentFloat(component));

    if (CSF_AT(std::uint8_t, self, 0xB) == 0xFF)
    {
        auto* node = reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0));
        component = CSF_AT(std::uint32_t, node, 0x20);
    }
    else
    {
        auto* pool = reinterpret_cast<void*>(
            g_ComponentPools[CSF_AT(std::uint8_t, self, 0xB)]);
        const auto base = CSF_AT(std::uint32_t, pool, 0);
        component = base + 12 * ((nodeAddress - base) / 0xC + 1);
        int remaining = 2;
        do
        {
            --remaining;
            component = base + 12 * CSF_AT(
                std::uint32_t, reinterpret_cast<void*>(component), 0);
        } while (remaining);

        if (!CSF_AT(std::uint8_t, pool, 0x14))
        {
            CSF_AT(std::uint8_t, reinterpret_cast<void*>(component), 0xB) =
                CSF_AT(std::uint8_t, pool, 0x15);
        }
    }
    output[2] = static_cast<float>(ReadComponentFloat(component));

    return 0;
}

HOOK(0x00693020, DataNode_ReadVector3, "auto");
