#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

static std::uint8_t __fastcall MenuScenarioSelect_GetPrimarySelectionCode(
    void* self, void* /*edx*/, std::uint32_t* selectionCode)
{
    using GetSelection = int (__fastcall*)(void*, void*);
    using GetSelectionId = std::uint32_t (__fastcall*)(void*, void*, int);

    if (!CSF_AT(void*, self, 0x20))
        return 0;

    void* control = CSF_AT(void*, self, 0x20);
    void** dispatchTable = CSF_AT(void**, control, 0);
    void** idTable = CSF_AT(void**, control, 0);

    int selection =
        reinterpret_cast<GetSelection>(dispatchTable[0x2AC / 4])(
            control, nullptr);

    control = CSF_AT(void*, self, 0x20);
    std::uint32_t identifier =
        reinterpret_cast<GetSelectionId>(idTable[0x2B0 / 4])(
            control, nullptr, selection);

    if (identifier == 0x494E3033u)
        *selectionCode = 2;
    else
        *selectionCode = identifier == 0x494E3032u ? 1u : 0u;

    return 1;
}

HOOK(0x0052EF80, MenuScenarioSelect_GetPrimarySelectionCode, "auto");
