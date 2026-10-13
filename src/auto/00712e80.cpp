#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

[[maybe_unused]] static const auto kGrowArrayVtable = CSF_VTBL(0x0041E5F0);

static int __fastcall MenuSystem_AssignDialogPanel(C_0085ec18* self, void* /*edx*/, int panel)
{
    auto* list = reinterpret_cast<S_0041e5f0*>(
        static_cast<std::uint8_t*>(static_cast<void*>(self)) + 0x8);

    if (CSF_AT(std::uint32_t, self, 0x10) >=
        CSF_AT(std::uint32_t, self, 0xC))
    {
        S_0041e5f0_GrowArrayCapacity(
            list, nullptr, CSF_AT(int, self, 0x14));
    }

    const std::uint32_t index = CSF_AT(std::uint32_t, self, 0x10);
    int* slots = CSF_AT(int*, self, 0x8);
    int* destination = slots + index;
    CSF_AT(std::uint32_t, self, 0x10) = index + 1;
    *destination = panel;
    return panel;
}

HOOK(0x00712E80, MenuSystem_AssignDialogPanel, "auto");
