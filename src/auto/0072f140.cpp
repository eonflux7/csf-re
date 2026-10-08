#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_GLOBAL(std::uint32_t, g_ChoiceButtonVtable, 0x00866340);

CSF_FUNC(int, __cdecl, ResetStringRecordRange, 0x0072EF70,
         (int begin, int end, ...));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60,
         (void* block));
CSF_FUNC(int, __fastcall, CGuiButton_Destroy, 0x00725590,
         (CGuiChoiceButton* self, void* edx));

static int __fastcall CGuiChoiceButton_Destroy(
    CGuiChoiceButton* self, void* /*edx*/)
{
    auto* records = reinterpret_cast<std::uint8_t*>(self) + 0x1F0;
    CSF_AT(void**, self, 0) =
        reinterpret_cast<void**>(&g_ChoiceButtonVtable);

    const std::uint32_t begin = CSF_AT(std::uint32_t, records, 4);
    if (begin)
    {
        ResetStringRecordRange(
            static_cast<int>(begin),
            CSF_AT(int, records, 8),
            records,
            self);

        const std::uint32_t allocation = CSF_AT(std::uint32_t, records, 4);
        if (allocation)
            Mem_FreeDebug(reinterpret_cast<void*>(allocation));
    }

    CSF_AT(std::uint32_t, records, 4) = 0;
    CSF_AT(std::uint32_t, records, 8) = 0;
    CSF_AT(std::uint32_t, records, 0xC) = 0;

    return CGuiButton_Destroy(self, nullptr);
}

HOOK(0x0072F140, CGuiChoiceButton_Destroy, "auto");
