#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160,
         (void* self, void* edx));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_LayoutInset, 0x0082FEBC);
CSF_GLOBAL(uint32_t, g_ScreenHeight, 0x00899BA0);

static void* MenuMessageBox_CallPointerMethod(void* object, unsigned slot)
{
    return reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[slot])(object, nullptr);
}

static int MenuMessageBox_GetElementHeight(void* object)
{
    return reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0xA0 / 4])(object, nullptr);
}

static void* MenuMessageBox_GetCurrentDisplay()
{
    const int context = GetGlobalInt();
    return LookupObjectById(
        reinterpret_cast<void*>(context), nullptr);
}

static int __fastcall CMenuMessageBox_UpdateLayout(
    CMenuMessageBox* self, void* /*edx*/)
{
    int32_t size[2] = {0, 0};
    int32_t position[2] = {0, 0};

    void* layout = MenuMessageBox_CallPointerMethod(
        reinterpret_cast<void*>(self->field_0894), 0x1CC / 4);
    if (!layout)
        return 0;

    void* display = MenuMessageBox_GetCurrentDisplay();
    void* dimensions = MenuMessageBox_CallPointerMethod(display, 0x18 / 4);
    const int32_t displayHeight = CSF_AT(int32_t, dimensions, 4);

    const uint32_t screenHeightBits = g_ScreenHeight;
    double screenHeight = static_cast<double>(
        static_cast<int32_t>(g_ScreenHeight));
    if (static_cast<int32_t>(screenHeightBits) < 0)
        screenHeight += static_cast<double>(g_UnsignedIntegerBias);

    const float scale = static_cast<float>(
        static_cast<double>(displayHeight) / screenHeight);

    display = MenuMessageBox_GetCurrentDisplay();
    MenuMessageBox_CallPointerMethod(display, 0x18 / 4);

    uint32_t firstHeight = 0;
    uint32_t secondHeight = 0;
    void* element = reinterpret_cast<void*>(self->m_staticControl);
    if (element) {
        void* content = MenuMessageBox_CallPointerMethod(element, 0x110 / 4);
        if (content)
            firstHeight = static_cast<uint32_t>(
                MenuMessageBox_GetElementHeight(content));
    }

    element = reinterpret_cast<void*>(self->field_005c);
    if (element) {
        void* content = MenuMessageBox_CallPointerMethod(element, 0x110 / 4);
        if (content)
            secondHeight = static_cast<uint32_t>(
                MenuMessageBox_GetElementHeight(content));
    }

    const uint32_t inset = static_cast<uint32_t>(
        static_cast<int64_t>(
            static_cast<double>(scale) * static_cast<double>(g_LayoutInset)));

    // Unsigned arithmetic preserves the original wrapping 32-bit operations.
    uint32_t totalHeight = firstHeight - 2u * inset;
    if (CSF_AT(uint8_t, self, 0x8B8))
        totalHeight += secondHeight - inset;

    if (CSF_AT(uint8_t, self, 0x8B9)) {
        void* extraDimensions = MenuMessageBox_CallPointerMethod(
            reinterpret_cast<void*>(self->field_006c), 0xA4 / 4);
        totalHeight += CSF_AT(uint32_t, extraDimensions, 4) - inset;
    }

    const uint32_t buttonMask = self->field_007c;
    if (buttonMask) {
        unsigned buttonCount = 0;
        for (unsigned bit = 0; bit != 10; ++bit)
            buttonCount += (buttonMask >> bit) & 1u;

        const unsigned rows = buttonCount / 4 + (buttonCount % 4 != 0);
        totalHeight += rows * (self->field_096c - inset);
    }

    size[0] = static_cast<int32_t>(self->field_0970);
    size[1] = static_cast<int32_t>(totalHeight);

    display = MenuMessageBox_GetCurrentDisplay();
    dimensions = MenuMessageBox_CallPointerMethod(display, 0x18 / 4);
    position[0] = static_cast<int32_t>(
        CSF_AT(uint32_t, dimensions, 0) -
        static_cast<uint32_t>(size[0])) / 2;

    display = MenuMessageBox_GetCurrentDisplay();
    dimensions = MenuMessageBox_CallPointerMethod(display, 0x18 / 4);
    const uint32_t heightDifference =
        CSF_AT(uint32_t, dimensions, 4) - static_cast<uint32_t>(size[1]);
    const int32_t horizontalOffset = CSF_AT(int32_t, self, 0x94C);
    position[1] = static_cast<int32_t>(heightDifference) / 2;

    if (horizontalOffset != -1) {
        position[0] = static_cast<int32_t>(
            static_cast<uint32_t>(position[0]) +
            static_cast<uint32_t>(horizontalOffset));
    }

    const int32_t verticalOffset = CSF_AT(int32_t, self, 0x950);
    if (verticalOffset != -1) {
        position[1] = static_cast<int32_t>(
            static_cast<uint32_t>(position[1]) +
            static_cast<uint32_t>(verticalOffset));
    }

    reinterpret_cast<void (__fastcall*)(void*, void*, int32_t*)>(
        CSF_AT(void**, layout, 0)[0x1C / 4])(layout, nullptr, position);

    return reinterpret_cast<int (__fastcall*)(void*, void*, int32_t*)>(
        CSF_AT(void**, layout, 0)[0x24 / 4])(layout, nullptr, size);
}

HOOK(0x00529360, CMenuMessageBox_UpdateLayout, "auto");
