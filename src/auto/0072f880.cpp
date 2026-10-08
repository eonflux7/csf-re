#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB,
         (char* buffer, const char* format, ...));

static int __fastcall CGuiChoiceNumButton_AdjustValueAndUpdateText(
    CGuiChoiceNumButton* self, void* /*edx*/, int delta)
{
    const int32_t maximum = static_cast<int32_t>(self->field_01f8);
    const uint32_t adjustedBits =
        self->field_0204 + static_cast<uint32_t>(delta);
    int32_t value = static_cast<int32_t>(adjustedBits);

    self->field_0204 = adjustedBits;
    if (value > maximum)
        value = static_cast<int32_t>(self->m_minValue);

    const bool belowMinimum =
        value < static_cast<int32_t>(self->m_minValue);
    self->field_0204 = static_cast<uint32_t>(value);
    if (belowMinimum)
        value = maximum;
    self->field_0204 = static_cast<uint32_t>(value);

    void* textObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x110 / 4])(self, nullptr);
    if (!textObject)
        return 0;

    char buffer[256];
    _sprintf(buffer, CSF_STR(0x0083090C, "%d"),
                      static_cast<int32_t>(self->field_0204));

    value = static_cast<int32_t>(self->field_0204);
    const int32_t minimum = static_cast<int32_t>(self->m_minValue);

    if (value > minimum &&
        value < static_cast<int32_t>(self->field_01f8))
    {
        return reinterpret_cast<int (__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, textObject, 0)[0x3C / 4])(
                textObject, nullptr, buffer);
    }

    uint32_t endpointText;
    if (value == minimum)
    {
        endpointText = self->field_01f4;
    }
    else
    {
        if (value != static_cast<int32_t>(self->field_01f8))
            return value;
        endpointText = self->field_01fc;
    }

    if (endpointText)
    {
        return reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, textObject, 0)[0x40 / 4])(
                textObject, nullptr, endpointText);
    }

    return reinterpret_cast<int (__fastcall*)(void*, void*, char*)>(
        CSF_AT(void**, textObject, 0)[0x3C / 4])(
            textObject, nullptr, buffer);
}

HOOK(0x0072F880, CGuiChoiceNumButton_AdjustValueAndUpdateText, "auto");
