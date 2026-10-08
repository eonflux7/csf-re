#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiStatic_InvokeTextVirtual, 0x00724D00,
         (CGuiChoiceButton* self, void* edx));
CSF_FUNC(int, __cdecl, ResolveChoiceText, 0x00692440,
         (int textId, int context));
CSF_FUNC(std::uint32_t*, __fastcall, AssignChoiceText, 0x00696F90,
         (std::uint32_t* self, void* edx, std::int16_t* text));

static int __fastcall CGuiChoiceButton_RefreshChoiceTexts(
    CGuiChoiceButton* self, void* /*edx*/)
{
    CGuiStatic_InvokeTextVirtual(self, nullptr);

    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x1F4);
    std::uint32_t remaining = 0;
    if (begin)
    {
        const std::uint32_t difference =
            CSF_AT(std::uint32_t, self, 0x1F8) - begin;
        remaining = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(difference) >> 5);
    }

    if (remaining != 0)
    {
        std::uint32_t byteOffset = 0;
        do
        {
            auto* record = reinterpret_cast<std::uint32_t*>(
                CSF_AT(std::uint32_t, self, 0x1F4) + byteOffset);

            const std::uint32_t textId = CSF_AT(std::uint32_t, record, 0);
            if (textId)
            {
                const int text = ResolveChoiceText(
                    static_cast<int>(textId),
                    CSF_AT(int, self, 0x204));
                AssignChoiceText(
                    reinterpret_cast<std::uint32_t*>(
                        reinterpret_cast<std::uint8_t*>(record) + 4),
                    nullptr,
                    reinterpret_cast<std::int16_t*>(text));
            }

            byteOffset += 0x20;
            --remaining;
        } while (remaining != 0);
    }

    return static_cast<int>(remaining);
}

HOOK(0x0072EB70, CGuiChoiceButton_RefreshChoiceTexts, "auto");
