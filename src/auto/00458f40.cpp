#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, HudStatusIcons_UpdateIcon, 0x00458210,
         (int a1, float a2, int a3));
CSF_FUNC(void**, __fastcall, HudStatusIcons_EraseIcon, 0x00458DD0,
         (void* self, void* edx, void** a2, char* a3));

static int __fastcall S_00458f40_UpdateIcons(
    S_00458f40* self, void* /*edx*/, float deltaTime)
{
    int count = 0;
    if (self->field_001c)
    {
        count = static_cast<int>(
            CSF_AT(uint32_t, self, 0x20) - self->field_001c) / 40;
    }

    int result = count - 1;
    if (result < 0)
        return result;

    alignas(8) uint8_t eraseResultBytes[0x8] = {};
    CSF_AT(int, eraseResultBytes, 0x4) = result + 1;

    int iconOffset = 40 * result;
    void* icons = reinterpret_cast<uint8_t*>(self) + 0x18;

    do
    {
        auto* icon =
            reinterpret_cast<uint8_t*>(self->field_001c) + iconOffset;

        bool update = CSF_AT(int, icon, 0x10) == -1082130432;
        if (!update)
        {
            CSF_AT(float, icon, 0x14) =
                deltaTime + CSF_AT(float, icon, 0x14);

            icon = reinterpret_cast<uint8_t*>(self->field_001c) + iconOffset;
            update = !(CSF_AT(float, icon, 0x14) >=
                       CSF_AT(float, icon, 0x10));
        }

        if (update)
        {
            HudStatusIcons_UpdateIcon(
                reinterpret_cast<int>(icon),
                deltaTime,
                reinterpret_cast<int>(icon + 0x1C));
        }
        else
        {
            HudStatusIcons_EraseIcon(
                icons, nullptr,
                reinterpret_cast<void**>(eraseResultBytes),
                reinterpret_cast<char*>(icon));
        }

        iconOffset -= 40;
        result = CSF_AT(int, eraseResultBytes, 0x4) - 1;
        CSF_AT(int, eraseResultBytes, 0x4) = result;
    } while (result);

    return result;
}

HOOK(0x00458F40, S_00458f40_UpdateIcons, "auto");
