#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiChoiceButton_GetCurrentChoiceText(
    CGuiChoiceButton* self, void* /*edx*/, unsigned int choiceIndex)
{
    const uint32_t recordsBegin = self->field_01f4;
    if (recordsBegin == 0)
        return 0;

    const uint32_t recordCount = static_cast<uint32_t>(
        static_cast<int32_t>(self->field_01f8 - recordsBegin) >> 5);
    if (choiceIndex >= recordCount)
        return 0;

    const uint32_t recordOffset = self->field_0200 << 5;
    const uint32_t recordAddress = self->field_01f4 + recordOffset;
    void* const record = reinterpret_cast<void*>(recordAddress);

    if (CSF_AT(uint32_t, record, 0x1C) < 8U)
        return static_cast<int>(recordAddress + 8U);

    return static_cast<int>(CSF_AT(uint32_t, record, 0x08));
}

HOOK(0x0072EC20, CGuiChoiceButton_GetCurrentChoiceText, "auto");
