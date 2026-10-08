#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiEdit_InsertCharacter, 0x00721DC0,
         (CGuiEdit* self, void* edx, std::int16_t character));
CSF_FUNC(unsigned char, __fastcall, CGuiEdit_HandleNavigationKey, 0x007227D0,
         (CGuiEdit* self, void* edx, int key));
CSF_FUNC(unsigned char, __fastcall, IsEditKeyAllowed, 0x00722880,
         (CGuiEdit* self, void* edx, int key));

CSF_GLOBAL(float, g_ValueScale, 0x0082D4EC);

// Keep the implicit EBX live-in intact for the original key handlers.
// Comparison uses the unrounded sum, as does the original x87 sequence.
static __declspec(noinline) __attribute__((optnone))
void __fastcall CGuiEdit_UpdateKeyRepeat(
    CGuiEdit* self, void* /*edx*/, float elapsedTime)
{
    std::uint32_t repeatMode = self->field_01b4;

    if (repeatMode == 1)
    {
        double accumulated =
            static_cast<double>(elapsedTime) +
            static_cast<double>(self->field_01b8);

        self->field_01b8 = static_cast<float>(accumulated);

        if (!(accumulated >= static_cast<double>(g_ValueScale)))
            return;

        std::uint16_t character = self->field_01c4;
        if (character < 0x20u ||
            CSF_AT(std::uint32_t, self, 0x164) >= self->m_maxLength)
        {
            self->field_01b4 = 0xFFFFFFFFu;
            self->field_01b8 = 0.0f;
            return;
        }

        CGuiEdit_InsertCharacter(self, nullptr, static_cast<std::int16_t>(character));
        self->field_01b8 = 0.0f;
        return;
    }

    if (repeatMode != 0)
        return;

    double accumulated =
        static_cast<double>(elapsedTime) +
        static_cast<double>(self->field_01b8);

    self->field_01b8 = static_cast<float>(accumulated);

    if (!(accumulated >= static_cast<double>(g_ValueScale)))
        return;

    std::uint32_t key = self->field_01bc;
    if (key == 0xFFFFFFFFu)
    {
        int alternateKey = static_cast<int>(self->field_01c0);
        IsEditKeyAllowed(self, nullptr, alternateKey);
    }
    else
    {
        CGuiEdit_HandleNavigationKey(self, nullptr, static_cast<int>(key));
    }

    self->field_01b8 = 0.0f;
}

HOOK(0x00722A40, CGuiEdit_UpdateKeyRepeat, "auto");
