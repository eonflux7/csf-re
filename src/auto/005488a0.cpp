#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(std::uint8_t, __cdecl, MenuExtras_IsExitInput, 0x007A8ED0,
         (int input));
CSF_FUNC(void, __fastcall, MenuExtras_ProcessSelection, 0x005464E0,
         (void* self, void* edx, int selection));
CSF_FUNC(char, __fastcall, MenuExtras_PerformAlternateAction, 0x00548650,
         (void* self, void* edx));

static void __fastcall MenuExtras_HandleInput(
    void* self, void* /*edx*/, int input, int /*unused*/)
{
    if (MenuExtras_IsExitInput(input) != 0)
    {
        CSF_AT(std::uint8_t, self, 0x42) = 1;
        return;
    }

    switch (input)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    {
        void* selectionObject = CSF_AT(void*, self, 0x14);
        const int selection =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, selectionObject, 0)[0x104 / 4])(
                    selectionObject, nullptr);

        MenuExtras_ProcessSelection(
            static_cast<std::uint8_t*>(self) - 4, nullptr, selection);
        break;
    }

    case 6:
        MenuExtras_PerformAlternateAction(
            static_cast<std::uint8_t*>(self) - 4, nullptr);
        break;

    case 13:
        CSF_AT(std::uint8_t, self, 0x42) = 1;
        break;

    default:
        break;
    }
}

HOOK(0x005488A0, MenuExtras_HandleInput, "auto");
