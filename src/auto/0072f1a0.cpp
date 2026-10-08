#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, ResetStringRecordRange, 0x0072EF70,
         (int begin, int end, ...));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60,
         (void* block));

static int __fastcall CGuiChoiceButton_ClearChoices(
    CGuiChoiceButton* self, void* /*edx*/)
{
    auto* records = reinterpret_cast<std::uint8_t*>(self) + 0x1F0;
    CSF_AT(std::uint32_t, self, 0x200) = 0;

    std::uint32_t result = CSF_AT(std::uint32_t, records, 4);
    if (result)
    {
        ResetStringRecordRange(
            static_cast<int>(result),
            CSF_AT(int, records, 8),
            records,
            self);

        result = CSF_AT(std::uint32_t, records, 4);
        if (result)
        {
            result = static_cast<std::uint32_t>(
                Mem_FreeDebug(reinterpret_cast<void*>(result)));
        }
    }

    CSF_AT(std::uint32_t, records, 4) = 0;
    CSF_AT(std::uint32_t, records, 8) = 0;
    CSF_AT(std::uint32_t, records, 0xC) = 0;

    return static_cast<int>(result);
}

HOOK(0x0072F1A0, CGuiChoiceButton_ClearChoices, "auto");
