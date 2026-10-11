#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint64_t, __fastcall, CharActionFsm_GetPrimaryCapacity, 0x004769F0, (void* self, void* edx));
CSF_FUNC(uint64_t, __fastcall, GetItemCapacity, 0x00476A50, (void* self, void* edx));

static int __fastcall C_008319c0_GetTotalCapacity(C_008319c0* self, void* /*edx*/)
{
    if (!self->vSlot9C())
        return 1;

    auto* definition = reinterpret_cast<void*>(self->field_0040);
    switch (CSF_AT(int, definition, 0xD4))
    {
    case 0:
        return CSF_AT(int, definition, 0x104);
    case 1:
    {
        const uint32_t baseCapacity = CSF_AT(uint32_t, definition, 0x104);
        const uint32_t itemCapacity = static_cast<uint32_t>(GetItemCapacity(self, nullptr));
        return static_cast<int>(baseCapacity + itemCapacity);
    }
    case 2:
    {
        const uint32_t baseCapacity = CSF_AT(uint32_t, definition, 0x104);
        const uint32_t primaryCapacity =
            static_cast<uint32_t>(CharActionFsm_GetPrimaryCapacity(self, nullptr));
        return static_cast<int>(baseCapacity + baseCapacity * primaryCapacity);
    }
    default:
        return 0;
    }
}

HOOK(0x00476AB0, C_008319c0_GetTotalCapacity, "auto");
