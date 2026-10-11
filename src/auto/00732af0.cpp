#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(int, __fastcall, MenuAccount_GateCheck_696B60, 0x00696B60, (void* self, void* edx, void* a2));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

static std::uint32_t* __fastcall C_008670f0_ActivateMessageText(
    C_008670f0* self, void* /*edx*/)
{
    auto* input = reinterpret_cast<std::uint8_t*>(self) + 0x20;
    int result = MenuAccount_GateCheck_696B60(input, nullptr, &g_byte_82E65C);
    if (static_cast<std::uint8_t>(result) != 0)
    {
        const std::uint32_t code = self->field_0018;
        if (code != 0)
        {
            result = static_cast<int>(self->field_001c);
            if (result != 0)
            {
                self->field_0010 = 1;
                self->field_00d8 = 1;
                const int text = MenuMessageBox_ResolveEntryByKey(
                    static_cast<int>(code), result);
                return MenuProgressInput_AssignText(
                    reinterpret_cast<std::uint32_t*>(input), nullptr,
                    reinterpret_cast<std::int16_t*>(text));
            }
        }
    }
    return reinterpret_cast<std::uint32_t*>(result);
}

HOOK(0x00732AF0, C_008670f0_ActivateMessageText, "auto");
