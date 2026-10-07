#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Walks the list box's header text object and the text object of every column
// entry and invokes virtual slot +0x38 (index 14) on each of them. The headers
// and column vector are reloaded after every call because the callee may have
// modified the control's storage. Returns the last value produced (the last
// virtual call's result, or the last column entry address when no call was
// made).
static int __fastcall CGuiListBoxBase_ReleaseHeaderAndColumnTexts(
    CGuiListBoxBase* self, void* /*edx*/)
{
    void* headerText = reinterpret_cast<void*>(self->m_pHeaderText);
    if (headerText) {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, headerText, 0)[14])(headerText, nullptr);
    }

    uint32_t result = self->m_columnsBegin;
    if (result) {
        // Signed divide by 12 of the byte length of the column vector
        // (matches the 0x2AAAAAAB magic in the original).
        const int32_t byteDifference =
            static_cast<int32_t>(self->m_columnsEnd - result);
        result = static_cast<uint32_t>(byteDifference / 12);
    }

    if (result) {
        uint32_t offset = 0;
        uint32_t remaining = result;
        do {
            // Re-read the vector base: the virtual call above may have
            // reallocated / emptied the column storage.
            result = self->m_columnsBegin + offset;
            void* columnText = reinterpret_cast<void*>(
                CSF_AT(uint32_t, reinterpret_cast<void*>(result), 4));
            if (columnText) {
                result = static_cast<uint32_t>(
                    reinterpret_cast<int(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, columnText, 0)[14])(
                            columnText, nullptr));
            }
            offset += 12; // each column entry is 12 bytes
            --remaining;
        } while (remaining != 0);
    }

    return static_cast<int32_t>(result);
}

HOOK(0x00714E10, CGuiListBoxBase_ReleaseHeaderAndColumnTexts, "auto");
