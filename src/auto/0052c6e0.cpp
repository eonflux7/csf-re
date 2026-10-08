#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById,
         0x00652160, (void* self, void* edx));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_LayoutInset, 0x0082FEBC);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static void* MenuProgressInput_GetDisplayDimensions()
{
    void* display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    return ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
}

static void* MenuProgressInput_GetLayoutElement(uint32_t objectAddress)
{
    void* object = reinterpret_cast<void*>(objectAddress);
    return ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, object, 0)[0x110 / 4])(object, nullptr);
}

static int MenuProgressInput_GetElementHeight(void* element)
{
    return ((int (__fastcall*)(void*, void*))
        CSF_AT(void**, element, 0)[0xA0 / 4])(element, nullptr);
}

static int __fastcall C_008414e8_CenterContentPanel(
    C_008414e8* self, void* /*edx*/)
{
    int32_t size[2] = {0, 0};
    int32_t position[2] = {0, 0};

    void* dimensions = MenuProgressInput_GetDisplayDimensions();
    const double displayHeight =
        static_cast<double>(CSF_AT(int32_t, dimensions, 4));

    const int32_t screenHeightSign = static_cast<int32_t>(g_ScreenHeight);
    double screenHeight =
        static_cast<double>(static_cast<int32_t>(g_ScreenHeight));
    if (screenHeightSign < 0)
        screenHeight += static_cast<double>(g_UnsignedIntegerBias);

    const float scale = static_cast<float>(displayHeight / screenHeight);

    // The second dimensions query is intentional; its result is discarded.
    MenuProgressInput_GetDisplayDimensions();

    void* firstElement = MenuProgressInput_GetLayoutElement(self->field_0024);
    void* secondElement = MenuProgressInput_GetLayoutElement(self->field_0028);
    void* thirdElement = MenuProgressInput_GetLayoutElement(self->field_002c);
    void* fourthElement = MenuProgressInput_GetLayoutElement(self->field_0030);

    uint32_t totalHeight =
        static_cast<uint32_t>(MenuProgressInput_GetElementHeight(firstElement));

    // The original converts the unrounded x87 product to a 64-bit integer,
    // then uses only its low 32 bits.
    const int64_t scaledInset = static_cast<int64_t>(
        static_cast<double>(scale) * static_cast<double>(g_LayoutInset));
    totalHeight -= static_cast<uint32_t>(scaledInset) * 5u;

    totalHeight +=
        static_cast<uint32_t>(MenuProgressInput_GetElementHeight(thirdElement));
    totalHeight +=
        static_cast<uint32_t>(MenuProgressInput_GetElementHeight(fourthElement));
    totalHeight +=
        static_cast<uint32_t>(MenuProgressInput_GetElementHeight(secondElement));

    size[0] = static_cast<int32_t>(self->field_0128);
    size[1] = static_cast<int32_t>(totalHeight);

    dimensions = MenuProgressInput_GetDisplayDimensions();
    position[0] = static_cast<int32_t>(
        CSF_AT(uint32_t, dimensions, 0) -
        static_cast<uint32_t>(size[0])) / 2;

    dimensions = MenuProgressInput_GetDisplayDimensions();
    const uint32_t verticalDifference =
        CSF_AT(uint32_t, dimensions, 4) - static_cast<uint32_t>(size[1]);
    void* panel = reinterpret_cast<void*>(self->field_0034);
    position[1] = static_cast<int32_t>(verticalDifference) / 2;

    ((void (__fastcall*)(void*, void*, int32_t*))
        CSF_AT(void**, panel, 0)[0x1C / 4])(panel, nullptr, position);

    panel = reinterpret_cast<void*>(self->field_0034);
    return ((int (__fastcall*)(void*, void*, int32_t*))
        CSF_AT(void**, panel, 0)[0x24 / 4])(panel, nullptr, size);
}

HOOK(0x0052C6E0, C_008414e8_CenterContentPanel, "auto");
