#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(unsigned int, __cdecl, FillScreenScaleFactors, 0x0042DE60, (float* a1));

// Multiplies an int by a screen scale factor in double precision, truncates the product
// toward zero to a 64-bit integer, and keeps the low 32 bits. Out-of-range or NaN products
// give the 64-bit indefinite value 0x8000000000000000, whose low 32 bits are zero.
static int HudScaleToIntTruncated(int value, float scale)
{
    double product = static_cast<double>(value) * static_cast<double>(scale);
    int64_t truncated = INT64_MIN;
    if (product >= -9223372036854775808.0 && product < 9223372036854775808.0)
        truncated = static_cast<int64_t>(product);
    return static_cast<int>(truncated);
}

static int __fastcall CHud_LayoutDebugOverlayWidget(CHud* self, void* /*edx*/)
{
    // Re-reads the stored widget handle every time the original does.
    auto currentWidget = [self]() {
        return reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_02c8));
    };

    using FindWidgetByNameFn = int(__fastcall*)(void*, void*, const char*);
    using WidgetAnchorFn = void(__fastcall*)(void*, void*, float*);
    using WidgetSetIntPairFn = void(__fastcall*)(void*, void*, int*);
    using WidgetGetIntPairFn = int(__fastcall*)(void*, void*, int*);

    void* globalIntObject = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
    void* menuControl = MenuControls_ResolveObject(globalIntObject, nullptr);

    uint8_t* globalValue = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
    uint32_t nameCapacity = CSF_AT(uint32_t, globalValue, 6164);
    const char* widgetName = (nameCapacity < 0x10)
        ? reinterpret_cast<const char*>(globalValue + 6144)
        : CSF_AT(const char*, globalValue, 6144);

    int foundWidget = reinterpret_cast<FindWidgetByNameFn>(CSF_AT(void**, menuControl, 0)[20])(menuControl, nullptr, widgetName);
    self->field_02c8 = static_cast<uint32_t>(foundWidget);
    if (foundWidget == 0)
        return foundWidget;

    // Pass 1: query the widget's anchor offset (screen-scaled).
    float anchorOffset[2] = {0.0f, 0.0f};
    FillScreenScaleFactors(anchorOffset);
    void* anchorWidget = currentWidget();
    reinterpret_cast<WidgetAnchorFn>(CSF_AT(void**, anchorWidget, 0)[38])(anchorWidget, nullptr, anchorOffset);

    // Pass 2: size, base 108x95 scaled by the screen factors.
    int sizePx[2] = {108, 95};
    float scale[2] = {0.0f, 0.0f};
    FillScreenScaleFactors(scale);
    sizePx[0] = HudScaleToIntTruncated(sizePx[0], scale[0]);
    sizePx[1] = HudScaleToIntTruncated(sizePx[1], scale[1]);
    void* sizeWidget = currentWidget();
    reinterpret_cast<WidgetSetIntPairFn>(CSF_AT(void**, sizeWidget, 0)[20])(sizeWidget, nullptr, sizePx);

    // Pass 3: position, base 40x20 scaled by the screen factors.
    int posPx[2] = {40, 20};
    scale[0] = 0.0f;
    scale[1] = 0.0f;
    FillScreenScaleFactors(scale);
    posPx[0] = HudScaleToIntTruncated(posPx[0], scale[0]);
    posPx[1] = HudScaleToIntTruncated(posPx[1], scale[1]);
    void* posWidget = currentWidget();
    return reinterpret_cast<WidgetGetIntPairFn>(CSF_AT(void**, posWidget, 0)[22])(posWidget, nullptr, posPx);
}

HOOK(0x0042FE00, CHud_LayoutDebugOverlayWidget, "auto");
