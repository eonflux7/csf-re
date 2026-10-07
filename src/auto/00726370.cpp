#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static const auto CGuiButton_ProcessSelectionChange =
    reinterpret_cast<void (__fastcall *)(CGuiButton*, void*, char)>(0x00726090);

static void __fastcall CGuiButton_SetSelected(
    CGuiButton* self, void* /*edx*/, char selected)
{
    const auto getSelected =
        reinterpret_cast<std::uint8_t (__fastcall *)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xBC / 4]);

    if (static_cast<std::uint8_t>(selected) == getSelected(self, nullptr))
        return;

    if (self->m_bSelectable != 0 || selected == 0)
    {
        self->m_bSelected = static_cast<std::uint8_t>(selected);
    }
    else
    {
        // Preserve the original read and write, even though the value is unchanged.
        volatile std::uint8_t& storedSelection = self->m_bSelected;
        storedSelection = storedSelection;
    }

    CGuiButton_ProcessSelectionChange(self, nullptr, selected);
}

HOOK(0x00726370, CGuiButton_SetSelected, "auto");
