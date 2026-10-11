#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MenuProgressInput_InitializeText, 0x00696B80,
         (void* self, void* edx, wchar_t* String));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90,
         (std::uint32_t* self, void* edx, std::int16_t* text));

static std::uint32_t* __fastcall C_008670f0_InitializeText(
    C_008670f0* self, void* /*edx*/, wchar_t* text)
{
    auto* input = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::uint8_t*>(self) + 0x20);

    self->field_0018 = 0;
    self->field_001c = 0;

    int result = MenuProgressInput_InitializeText(input, nullptr, text);
    if (static_cast<std::uint8_t>(result) != 0)
    {
        self->field_0010 = 1;
        self->field_00d8 = 1;
        if (*text != 0)
            return MenuProgressInput_AssignText(
                input, nullptr, reinterpret_cast<std::int16_t*>(text));
        return static_cast<std::uint32_t*>(self->vSlot1C());
    }

    return reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(result));
}

HOOK(0x00732B40, C_008670f0_InitializeText, "auto");
