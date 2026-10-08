#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiPanel_InvokeChildCallbacks(CGuiPanel* self, void* /*edx*/)
{
    const uint32_t initialBegin = self->m_childrenBegin;
    int result = static_cast<int32_t>(initialBegin);

    uint32_t childCount = 0;
    if (initialBegin != 0) {
        const uint32_t byteCount = self->m_childrenEnd - initialBegin;
        childCount = static_cast<uint32_t>(static_cast<int32_t>(byteCount) >> 2);
    }

    for (uint32_t index = 0; index < childCount; ++index) {
        const uint32_t slotAddress = self->m_childrenBegin + index * 4u;
        void* child = reinterpret_cast<void*>(
            CSF_AT(uint32_t, reinterpret_cast<void*>(slotAddress), 0));

        result = static_cast<int32_t>(slotAddress);
        if (child != nullptr) {
            result = ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x50 / 4])(child, nullptr);
        }
    }

    return result;
}

HOOK(0x0072A7E0, CGuiPanel_InvokeChildCallbacks, "auto");
