#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiListBox_ApplyRowTextParameters(
    CGuiListBox* self, void* /*edx*/, int firstValue, int secondValue)
{
    if (self->m_pRowText == 0)
        return 0;

    void* rowText = reinterpret_cast<void*>(self->m_pRowText);
    int parameters[2];
    parameters[1] = secondValue;
    parameters[0] = firstValue;

    return reinterpret_cast<int(__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, rowText, 0)[0x98 / 4])(
            rowText, nullptr, parameters);
}

HOOK(0x00722E60, CGuiListBox_ApplyRowTextParameters, "auto");
