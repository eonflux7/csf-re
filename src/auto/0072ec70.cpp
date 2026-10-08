#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint32_t __fastcall CGuiChoiceButton_SelectChoiceById(
    CGuiChoiceButton* self, void* /*edx*/, int32_t choiceId)
{
    const uint32_t begin = self->m_choicesBegin;
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->m_choicesEnd - begin) >> 5)
        : 0;

    if (count == 0)
        return count;

    auto* entry = reinterpret_cast<uint8_t*>(self->m_choicesBegin);
    uint32_t index = 0;

    do {
        if (CSF_AT(int32_t, entry, 0) == choiceId) {
            return reinterpret_cast<
                uint32_t(__fastcall*)(CGuiChoiceButton*, void*, int32_t)>(
                CSF_AT(void**, self, 0)[0x2A8 / 4])(
                    self, nullptr, static_cast<int32_t>(index));
        }

        ++index;
        entry += 0x20;
    } while (index < count);

    return count;
}

HOOK(0x0072EC70, CGuiChoiceButton_SelectChoiceById, "auto");
