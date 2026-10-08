#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiButton_Ctor, 0x00725390,
         (CGuiButton* self, void* edx, int argument1, int argument2));
CSF_FUNC(int, __cdecl, ResetStringRecordRange, 0x0072EF70,
         (std::uint32_t begin, std::uint32_t end, void* container, int argument));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

CSF_GLOBAL(std::uint32_t, g_ChoiceButtonVtable, 0x00866340);

static CGuiChoiceButton* __fastcall CGuiChoiceButton_Construct(
    CGuiChoiceButton* self, void* /*edx*/, int argument1, int argument2)
{
    CGuiButton_Ctor(reinterpret_cast<CGuiButton*>(self), nullptr,
                   argument1, argument2);

    auto* records = reinterpret_cast<std::uint8_t*>(self) + 0x1F0;
    CSF_AT(std::uint32_t, self, 0) =
        reinterpret_cast<std::uint32_t>(&g_ChoiceButtonVtable);

    CSF_AT(std::uint32_t, records, 4) = 0;
    CSF_AT(std::uint32_t, records, 8) = 0;
    CSF_AT(std::uint32_t, records, 12) = 0;
    CSF_AT(std::uint32_t, self, 0x200) = 0;

    const auto begin = CSF_AT(std::uint32_t, records, 4);
    if (begin != 0) {
        ResetStringRecordRange(begin, CSF_AT(std::uint32_t, records, 8),
                               records, argument2);
        const auto block = CSF_AT(std::uint32_t, records, 4);
        if (block != 0)
            Mem_FreeDebug(reinterpret_cast<void*>(block));
    }

    CSF_AT(std::uint32_t, records, 4) = 0;
    CSF_AT(std::uint32_t, records, 8) = 0;
    CSF_AT(std::uint32_t, records, 12) = 0;
    CSF_AT(std::uint32_t, self, 0x204) = 1;
    return self;
}

HOOK(0x0072F4E0, CGuiChoiceButton_Construct, "auto");
