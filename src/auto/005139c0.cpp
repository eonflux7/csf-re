#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

// sub_5136B0: __thiscall with 3 stack args (ret 12). Bound as __fastcall form (self, edx, stack args...).
CSF_FUNC(unsigned int, __fastcall, SaveSlots_InsertEntry, 0x005136B0,
         (void* self, void* edx, void* a1, unsigned int a3, void* a2));

static std::uint32_t* __fastcall S_005139c0_InsertAndGetSlotAddress(
    S_005139c0* self, void* /*edx*/, std::uint32_t* result,
    int position, int extraArg)
{
    std::uint32_t begin = self->field_0004;
    int slotIndex = 0;

    if (begin != 0) {
        const int byteCount = static_cast<int>(self->field_0008 - begin);

        if (byteCount / 200 != 0) {
            slotIndex = (position - static_cast<int>(begin)) / 200;
        }
    }

    SaveSlots_InsertEntry(
        self,
        nullptr,
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(position)),
        1u,
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(extraArg)));

    *result = self->field_0004
            + static_cast<std::uint32_t>(slotIndex) * 200u;
    return result;
}

HOOK(0x005139C0, S_005139c0_InsertAndGetSlotAddress, "auto");
