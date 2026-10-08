#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall CGuiChoiceButton_CycleChoice(
    CGuiChoiceButton* self, void* /*edx*/, char forward)
{
    const std::uint32_t choicesBegin = self->m_choicesBegin;
    if (choicesBegin == 0)
        return;

    if ((static_cast<std::int32_t>(self->m_choicesEnd - choicesBegin) >> 5) == 0)
        return;

    const std::uint32_t choiceCount = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(self->m_choicesEnd - choicesBegin) >> 5);

    const std::uint32_t currentChoice = self->m_currentChoiceIndex;
    const std::uint32_t nextChoice = forward
        ? (currentChoice + 1u) % choiceCount
        : (currentChoice + choiceCount - 1u) % choiceCount;

    ((void(__fastcall*)(CGuiChoiceButton*, void*, std::uint32_t))
        CSF_AT(void**, self, 0)[0x2A8 / 4])(self, nullptr, nextChoice);
}

HOOK(0x0072EA20, CGuiChoiceButton_CycleChoice, "auto");
