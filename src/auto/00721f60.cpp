#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x00718700,
         (std::uint8_t* self, void* edx));
CSF_FUNC(int, __cdecl, IsIdNotInTable, 0x007210A0,
         (std::int16_t character));
CSF_FUNC(bool, __cdecl, IsNotAsciiAlphanumeric, 0x007210D0,
         (std::uint16_t character));
CSF_FUNC(int, __fastcall, CGuiEdit_InsertCharacter, 0x00721DC0,
         (CGuiEdit* self, void* edx, std::int16_t character));

static char __fastcall CGuiEdit_TryInsertCharacter(
    CGuiEdit* self, void* /*edx*/, std::uint16_t character)
{
    using ControlQuery = int (__fastcall*)(void*, void*);

    std::uint8_t result = static_cast<std::uint8_t>(
        reinterpret_cast<ControlQuery>(CSF_AT(void**, self, 0)[0x6C / 4])(
            self, nullptr));
    if (result == 0)
        return static_cast<char>(result);

    result = static_cast<std::uint8_t>(
        CGuiSlider_IsSelectableAndNotType10(
            reinterpret_cast<std::uint8_t*>(self), nullptr));
    if (result == 0)
        return static_cast<char>(result);

    result = self->field_014a;
    if (result != 0)
    {
        result = static_cast<std::uint8_t>(
            IsIdNotInTable(static_cast<std::int16_t>(character)));
        if (result != 0)
            return static_cast<char>(result);
    }

    result = self->field_01c6;
    if (result == 0)
    {
        result = static_cast<std::uint8_t>(
            IsNotAsciiAlphanumeric(character));
        if (result != 0)
            return static_cast<char>(result);
    }

    result = self->field_0148;
    if (result == 0)
    {
        result = static_cast<std::uint8_t>(
            reinterpret_cast<ControlQuery>(
                CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr));
        if (result == 0)
            return static_cast<char>(result);
    }

    if (character < 0x20u)
        return static_cast<char>(result);

    const std::uint32_t length = CSF_AT(std::uint32_t, self, 0x164);
    if (length >= self->m_maxLength)
        return static_cast<char>(length);

    CGuiEdit_InsertCharacter(
        self, nullptr, static_cast<std::int16_t>(character));
    self->field_01b4 = 1;
    self->field_01b8 = 0.0f;
    self->field_01c4 = character;
    self->field_013c = 1;
    return 1;
}

HOOK(0x00721F60, CGuiEdit_TryInsertCharacter, "auto");
