#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Iterates the child array of the panel (vector at +0x138..+0x13c) and invokes
// virtual slot +0x5C on every non-null child.  Returns the address of the last
// visited slot (or the begin pointer when the array is empty).
static int __fastcall CGuiPanel_InvokeChildCallbacks23(CGuiPanel* self, void* /*edx*/)
{
    const uint32_t begin = self->field_0138;
    int result = static_cast<int32_t>(begin);

    uint32_t childCount = 0;
    if (begin != 0) {
        const uint32_t byteCount = self->field_013c - begin;
        childCount = static_cast<uint32_t>(static_cast<int32_t>(byteCount) >> 2);
    }

    for (uint32_t index = 0; index < childCount; ++index) {
        const uint32_t slotAddress = self->field_0138 + index * 4u;
        void* child = reinterpret_cast<void*>(
            CSF_AT(uint32_t, reinterpret_cast<void*>(slotAddress), 0));

        result = static_cast<int32_t>(slotAddress);
        if (child != nullptr) {
            result = ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x5c / 4])(child, nullptr);
        }
    }

    return result;
}

HOOK(0x0072A8D0, CGuiPanel_InvokeChildCallbacks23, "auto");
