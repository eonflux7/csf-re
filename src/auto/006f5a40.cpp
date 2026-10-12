#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

[[maybe_unused]] static auto* const kGrowArrayVtable = CSF_VTBL(0x0041E5F0);

static int __fastcall GfxSceneNode_LinkChild(void* self, void* /*edx*/, int child)
{
    auto* array = reinterpret_cast<S_0041e5f0*>(
        static_cast<std::uint8_t*>(self) + 0x4);

    if (CSF_AT(std::uint32_t, self, 0xC) >=
        CSF_AT(std::uint32_t, self, 0x8))
    {
        S_0041e5f0_GrowArrayCapacity(
            array, nullptr, CSF_AT(int, self, 0x10));
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0xC);
    int* destination = CSF_AT(int*, self, 0x4) + count;
    CSF_AT(std::uint32_t, self, 0xC) = count + 1;
    *destination = child;
    return child;
}

HOOK(0x006F5A40, GfxSceneNode_LinkChild, "auto");
