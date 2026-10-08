#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static void* (__cdecl* const AllocateGameMemory)(std::size_t) =
    reinterpret_cast<void* (__cdecl*)(std::size_t)>(CSF_ADDR_FN(0x006910F0));

static std::uint32_t* __cdecl CreateVariantObject(int kind)
{
    switch (kind)
    {
    case 0:
    {
        auto* object = static_cast<std::uint32_t*>(AllocateGameMemory(4u));
        if (object)
            CSF_AT(void*, object, 0x00) = reinterpret_cast<void*>(CSF_ADDR_VT(0x00865E28));
        return object;
    }

    case 1:
    {
        auto* object = static_cast<std::uint32_t*>(AllocateGameMemory(4u));
        if (object)
            CSF_AT(void*, object, 0x00) = reinterpret_cast<void*>(CSF_ADDR_VT(0x00865E10));
        return object;
    }

    case 2:
    {
        auto* object = static_cast<std::uint32_t*>(AllocateGameMemory(4u));
        if (object)
            CSF_AT(void*, object, 0x00) = reinterpret_cast<void*>(CSF_ADDR_VT(0x00865E1C));
        return object;
    }

    default:
        return nullptr;
    }
}

HOOK(0x0072CB10, CreateVariantObject, "auto");
