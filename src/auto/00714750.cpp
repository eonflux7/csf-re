#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiListBoxBase_SetItemHeight(
    CGuiListBoxBase* self, void* /*edx*/, int itemHeight)
{
    self->field_01bc = 0;
    const uint32_t initialSpacing = self->m_lineSpacing;
    self->field_01c0 = 0;

    void** controlVtable = CSF_AT(void**, self, 0);
    const uint32_t initialPaddedHeight =
        static_cast<uint32_t>(itemHeight) + 2u * initialSpacing;

    self->m_bLayoutDirty = 1;
    self->m_rowHeight = static_cast<uint32_t>(itemHeight);

    const uint32_t firstExtent =
        *reinterpret_cast<uint32_t* (__fastcall*)(CGuiListBoxBase*, void*)>(
            controlVtable[0xA4 / 4])(self, nullptr);

    controlVtable = CSF_AT(void**, self, 0);
    self->field_01c4 = firstExtent;
    self->field_01d0 = 0;

    const uint32_t currentHeight = self->m_rowHeight;
    self->field_01cc = 0;
    const uint32_t currentSpacing = self->m_lineSpacing;
    self->field_01c8 = initialPaddedHeight;

    const uint32_t currentPaddedHeight = currentHeight + 2u * currentSpacing;
    const uint32_t secondExtent =
        *reinterpret_cast<uint32_t* (__fastcall*)(CGuiListBoxBase*, void*)>(
            controlVtable[0xA4 / 4])(self, nullptr);

    void* auxiliaryControl = reinterpret_cast<void*>(self->field_013c);
    self->field_01d4 = secondExtent;
    self->field_01d8 = currentPaddedHeight;

    uint32_t parameters[4] = {0, 0, 0, 0x42C80000u};
    return reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, auxiliaryControl, 0)[0x2C / 4])(
            auxiliaryControl, nullptr, parameters);
}

HOOK(0x00714750, CGuiListBoxBase_SetItemHeight, "auto");
