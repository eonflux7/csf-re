#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall CGuiListBoxBase_SetHeaderText(
    CGuiListBoxBase* self, void* /*edx*/, uint32_t headerText)
{
    void* previousHeader = reinterpret_cast<void*>(self->m_pHeaderText);
    self->m_bLayoutDirty = 1;

    if (previousHeader != nullptr)
    {
        auto destroyHeader = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, previousHeader, 0)[0]);
        destroyHeader(previousHeader, nullptr, 1);

        static_cast<volatile CGuiListBoxBase*>(self)->m_pHeaderText = 0;
    }

    self->m_pHeaderText = headerText;
}

HOOK(0x00714290, CGuiListBoxBase_SetHeaderText, "auto");
