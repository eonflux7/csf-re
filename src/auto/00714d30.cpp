#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiListBoxBase_ProcessHeaderAndColumnText(
    CGuiListBoxBase* self, void* /*edx*/)
{
    void* headerText = reinterpret_cast<void*>(self->m_pHeaderText);
    if (headerText) {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, headerText, 0)[7])(headerText, nullptr);
    }

    uint32_t result = self->m_columnsBegin;
    if (result) {
        const int32_t byteDifference =
            static_cast<int32_t>(self->m_columnsEnd - result);
        result = static_cast<uint32_t>(byteDifference / 12);
    }

    if (result) {
        uint32_t offset = 0;
        uint32_t remaining = result;
        do {
            // The preceding virtual call may have changed the column storage.
            result = self->m_columnsBegin + offset;
            void* columnText = reinterpret_cast<void*>(
                CSF_AT(uint32_t, reinterpret_cast<void*>(result), 4));
            if (columnText) {
                result = static_cast<uint32_t>(
                    reinterpret_cast<int(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, columnText, 0)[7])(
                            columnText, nullptr));
            }
            offset += 12;
            --remaining;
        } while (remaining != 0);
    }

    return static_cast<int32_t>(result);
}

HOOK(0x00714D30, CGuiListBoxBase_ProcessHeaderAndColumnText, "auto");
