#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MenuAccount_GateCheck_696B60,
         0x00696B60, (std::uint16_t* self, void* edx, char* name));

static int __fastcall CGuiChoiceButton_SelectChoiceByName(
    CGuiChoiceButton* self, void* /*edx*/, char* name)
{
    if (!name)
        return 0;

    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x1F4);
    int result = static_cast<std::int32_t>(begin);

    const std::uint32_t choiceCount = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, self, 0x1F8) - begin) >> 5)
        : 0;

    if (!choiceCount)
        return result;

    std::uint32_t choiceIndex = 0;
    std::uint32_t byteOffset = 0;

    do
    {
        const std::uint32_t nameAddress =
            CSF_AT(std::uint32_t, self, 0x1F4) + byteOffset + 4;

        result = MenuAccount_GateCheck_696B60(
            reinterpret_cast<std::uint16_t*>(nameAddress), nullptr, name);

        if (static_cast<std::uint8_t>(result) != 0)
        {
            return ((int(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, self, 0)[0x2A8 / 4])(
                self, nullptr, static_cast<std::int32_t>(choiceIndex));
        }

        ++choiceIndex;
        byteOffset += 32;
    }
    while (choiceIndex < choiceCount);

    return result;
}

HOOK(0x0072ECD0, CGuiChoiceButton_SelectChoiceByName, "auto");
