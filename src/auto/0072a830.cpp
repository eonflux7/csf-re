#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Iterates the child-pointer array at +0x138..+0x13C of a CGuiPanel and, for each
// non-null child, invokes vtable slot +0x54 (index 0x15) with the child as `this`.
// Returns the address of the last visited slot, or the last virtual call's result.
static int __fastcall CGuiPanel_InvokeChildVirtual54(CGuiPanel* self, void* /*edx*/)
{
    const uint32_t initialBegin = self->field_0138;
    int result = static_cast<int32_t>(initialBegin);

    uint32_t childCount = 0;
    if (initialBegin != 0) {
        const uint32_t byteCount = self->field_013c - initialBegin;
        childCount = static_cast<uint32_t>(static_cast<int32_t>(byteCount) >> 2);
    }

    for (uint32_t index = 0; index < childCount; ++index) {
        const uint32_t slotAddress = self->field_0138 + index * 4u;
        void* child = reinterpret_cast<void*>(
            CSF_AT(uint32_t, reinterpret_cast<void*>(slotAddress), 0));

        result = static_cast<int32_t>(slotAddress);
        if (child != nullptr) {
            result = ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x54 / 4])(child, nullptr);
        }
    }

    return result;
}

HOOK(0x0072A830, CGuiPanel_InvokeChildVirtual54, "auto");

