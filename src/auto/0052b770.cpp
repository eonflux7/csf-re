#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MenuMessageBox_ReleaseRasterAndReset,
         0x005289E0, (CMenuMessageBox* self, void* edx));
CSF_FUNC(char, __fastcall, MenuMessageBox_AdvanceActiveIndicator,
         0x00529E50, (CMenuMessageBox* self, void* edx));
CSF_FUNC(uint8_t, __fastcall, CMenuMessageBox_UpdateSlideAnimation,
         0x00529EE0, (CMenuMessageBox* self, void* edx, float deltaTime));
CSF_FUNC(char, __fastcall, CMenuMessageBox_UpdateButtonState,
         0x0052B550, (CMenuMessageBox* self, void* edx));
CSF_FUNC(void*, __cdecl, MenuMessageBox_GetGlobalSettings,
         0x00756A10, ());

static float MenuMessageBox_ReadFloatBits(const uint32_t& bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static void MenuMessageBox_WriteFloatBits(uint32_t& bits, float value)
{
    std::memcpy(&bits, &value, sizeof(value));
}

static int __fastcall CMenuMessageBox_UpdateTimersAndSelection(
    CMenuMessageBox* self, void* /*edx*/, float deltaTime)
{
    if (MenuMessageBox_ReadFloatBits(self->field_0998) <
        static_cast<double>(self->field_08b4))
    {
        // FST stores a rounded float, but the following comparison uses
        // the sum still held in the x87 register.
        const double elapsed =
            static_cast<double>(deltaTime) +
            static_cast<double>(MenuMessageBox_ReadFloatBits(self->field_0998));
        MenuMessageBox_WriteFloatBits(self->field_0998,
                                      static_cast<float>(elapsed));
        if (elapsed >= static_cast<double>(self->field_08b4))
            return MenuMessageBox_ReleaseRasterAndReset(self, nullptr);
    }

    CMenuMessageBox_UpdateButtonState(self, nullptr);
    CMenuMessageBox_UpdateSlideAnimation(self, nullptr, deltaTime);

    const double indicatorElapsed =
        static_cast<double>(deltaTime) +
        static_cast<double>(MenuMessageBox_ReadFloatBits(self->field_09a4));
    MenuMessageBox_WriteFloatBits(self->field_09a4,
                                  static_cast<float>(indicatorElapsed));

    void* settings = MenuMessageBox_GetGlobalSettings();
    // The timer is read again after the settings accessor.
    if (MenuMessageBox_ReadFloatBits(self->field_09a4) >=
        static_cast<double>(CSF_AT(float, settings, 0x2260)))
    {
        MenuMessageBox_AdvanceActiveIndicator(self, nullptr);
        self->field_09a4 = 0;
    }

    if (self->field_0078 != UINT32_MAX)
        return -1;

    if (self->field_0082)
        self->field_0078 = 1;
    else if (self->field_0083)
        self->field_0078 = 0x2000;
    else if (self->field_0084)
        self->field_0078 = 0x1000;
    else if (self->field_0085)
        self->field_0078 = 0x800;
    else if (self->field_0086)
        self->field_0078 = 2;
    else if (self->field_0087)
        self->field_0078 = 4;
    else if (self->field_0088)
        self->field_0078 = 8;
    else if (self->field_0089)
        self->field_0078 = 0x20;
    else if (self->field_008a)
        self->field_0078 = 0x10;
    else if (self->field_008b)
        self->field_0078 = 0x40;
    else if (self->field_008c)
        self->field_0078 = 0x80;
    else if (self->field_008d)
        self->field_0078 = 0x100;
    else if (self->field_008e)
        self->field_0078 = 0x200;
    else if (self->field_008f)
        self->field_0078 = 0x400;

    if (self->field_0078 != UINT32_MAX)
        return MenuMessageBox_ReleaseRasterAndReset(self, nullptr);

    return -1;
}

HOOK(0x0052B770, CMenuMessageBox_UpdateTimersAndSelection, "auto");
