#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

static std::uint8_t __fastcall MenuScenarioSelect_GetSecondarySelectionCode(
    void* self, void* /*edx*/, std::uint32_t* selectionCode)
{
    using GetSelection = int (__fastcall*)(void*, void*);
    using GetSelectionId = std::uint32_t (__fastcall*)(void*, void*, int);

    if (!CSF_AT(void*, self, 0x28))
        return 0;

    void* control = CSF_AT(void*, self, 0x28);
    void** dispatchTable = CSF_AT(void**, control, 0);
    void** idTable = CSF_AT(void**, control, 0);

    int selection =
        reinterpret_cast<GetSelection>(dispatchTable[0x2AC / 4])(
            control, nullptr);

    control = CSF_AT(void*, self, 0x28);
    std::uint32_t identifier =
        reinterpret_cast<GetSelectionId>(idTable[0x2B0 / 4])(
            control, nullptr, selection);

    if (identifier == 0x494E3036u || identifier == 0x494E3139u)
        *selectionCode = 1;
    else if (identifier == 0x494E3037u || identifier == 0x494E3230u)
        *selectionCode = 2;
    else
        *selectionCode = 3;

    return 1;
}

HOOK(0x0052EFE0, MenuScenarioSelect_GetSecondarySelectionCode, "auto");
