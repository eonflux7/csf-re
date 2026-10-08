#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiChoiceButton_GetCurrentChoiceValue(
    CGuiChoiceButton* self, void* /*edx*/, unsigned int index)
{
    const std::uint32_t choicesBegin = self->m_choicesBegin;
    if (choicesBegin == 0)
        return 0;

    const std::int32_t byteCount =
        static_cast<std::int32_t>(self->m_choicesEnd - choicesBegin);
    const std::uint32_t choiceCount =
        static_cast<std::uint32_t>(byteCount >> 5);

    if (index >= choiceCount)
        return 0;

    const std::uint32_t currentIndex = self->m_currentChoiceIndex;
    const std::uint32_t currentBegin = self->m_choicesBegin;
    const std::uint32_t choiceAddress = currentBegin + (currentIndex << 5);
    return CSF_AT(int, reinterpret_cast<void*>(choiceAddress), 0);
}

HOOK(0x0072EBE0, CGuiChoiceButton_GetCurrentChoiceValue, "auto");
