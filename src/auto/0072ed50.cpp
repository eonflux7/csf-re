#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall CGuiChoiceButton_SelectChoice(
    CGuiChoiceButton* self, void* /*edx*/, std::uint32_t choiceIndex)
{
    const std::uint32_t choicesBegin = self->field_01f4;
    if (choicesBegin == 0)
        return;

    // Preserve the original wrapping subtraction and arithmetic shift.
    const std::uint32_t choiceCount = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(self->field_01f8 - choicesBegin) >> 5);
    if (choiceIndex >= choiceCount)
        return;

    self->field_0200 = choiceIndex;
    void* textObject =
        (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x110 / 4]))(self, nullptr);
    if (textObject == nullptr)
        return;

    void* choiceRecord = reinterpret_cast<void*>(
        self->field_01f4 + (self->field_0200 << 5));

    if (CSF_AT(std::uint32_t, choiceRecord, 0) != 0)
    {
        (reinterpret_cast<void (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, textObject, 0)[0x3C / 4]))(
                textObject, nullptr, CSF_STR(0x0082E65C, ""));

        // The preceding call may change the selection or its storage.
        choiceRecord = reinterpret_cast<void*>(
            self->field_01f4 + (self->field_0200 << 5));
        const std::uint32_t parameter = self->field_0204;
        const std::uint32_t resource = CSF_AT(std::uint32_t, choiceRecord, 0);

        (reinterpret_cast<void (__fastcall*)(
            void*, void*, std::uint32_t, std::uint32_t)>(
                CSF_AT(void**, textObject, 0)[0x30 / 4]))(
                    textObject, nullptr, resource, parameter);

        (reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, textObject, 0)[0x34 / 4]))(
                textObject, nullptr);
    }
    else
    {
        const char* text;
        if (CSF_AT(std::uint32_t, choiceRecord, 0x1C) < 8u)
        {
            text = reinterpret_cast<const char*>(
                reinterpret_cast<std::uint8_t*>(choiceRecord) + 8);
        }
        else
        {
            text = reinterpret_cast<const char*>(
                CSF_AT(std::uint32_t, choiceRecord, 8));
        }

        (reinterpret_cast<void (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, textObject, 0)[0x40 / 4]))(
                textObject, nullptr, text);
    }
}

HOOK(0x0072ED50, CGuiChoiceButton_SelectChoice, "auto");
