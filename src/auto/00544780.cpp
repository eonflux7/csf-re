#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(uint32_t, g_ScreenHeight, 0x00899BA0);

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160,
         (int objectId, void* edx));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360,
         (uint8_t* self, void* edx, const char* name));

static double MenuSaveGame_ConvertUnsignedAnchor(uint32_t value)
{
    const int32_t signedValue = static_cast<int32_t>(value);
    double converted = static_cast<double>(signedValue);
    if (signedValue < 0)
        converted += static_cast<double>(g_UnsignedIntegerBias);
    return converted;
}

// The original's push ebx / push esi save registers (restored at 0054484D),
// not arguments to the owner's slot 6. That getter is 0068B5F0 on the render
// engine: lea eax,[ecx+4]; ret. Giving it stack arguments misbalances ESP.
static uint32_t __fastcall CMenuSaveGame_ResolvePanelAndApplyScale(
    CMenuSaveGame* self, void* /*edx*/, void* screen, void* registryHolder)
{
    const uint32_t cachedPanel = self->field_0044;
    if (cachedPanel != 0)
        return cachedPanel;

    void* rootObject =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, screen, 0)[21])(
            screen, nullptr);

    void* registry = CSF_AT(void*, registryHolder, 0);
    void** rootVtable = CSF_AT(void**, rootObject, 0);

    uint8_t* namedElement = LookupNamedElement(
        reinterpret_cast<uint8_t*>(registry), nullptr,
        CSF_STR(0x00841304, ".PANEL_PRINCIPAL"));

    void* panel =
        ((void* (__fastcall*)(void*, void*, void*))rootVtable[6])(
            rootObject, nullptr, namedElement);

    const uint32_t panelValue =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(panel));
    self->field_0044 = panelValue;
    if (panelValue == 0)
        return panelValue;

    const int firstObjectId = GetGlobalInt();
    void* firstOwner = LookupObjectById(firstObjectId, nullptr);
    void* firstAnchor =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, firstOwner, 0)[6])(
            firstOwner, nullptr);
    const uint32_t anchorX = CSF_AT(uint32_t, firstAnchor, 0);

    const int secondObjectId = GetGlobalInt();
    void* secondOwner = LookupObjectById(secondObjectId, nullptr);
    void* secondAnchor =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, secondOwner, 0)[6])(
            secondOwner, nullptr);
    const uint32_t anchorY = CSF_AT(uint32_t, secondAnchor, 4);

    void* currentPanel =
        reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0044));
    void** panelVtable = CSF_AT(void**, currentPanel, 0);

    // Sign test and signed conversion read the screen dimension separately,
    // matching the original's two loads of each global.
    const double convertedY =
        MenuSaveGame_ConvertUnsignedAnchor(anchorY);
    const uint32_t heightForSign = g_ScreenHeight;
    const int32_t heightForConversion =
        static_cast<int32_t>(g_ScreenHeight);
    double convertedHeight = static_cast<double>(heightForConversion);
    if (static_cast<int32_t>(heightForSign) < 0)
        convertedHeight += static_cast<double>(g_UnsignedIntegerBias);
    const float ratioY = static_cast<float>(convertedY / convertedHeight);

    const double convertedX =
        MenuSaveGame_ConvertUnsignedAnchor(anchorX);
    const uint32_t widthForSign = g_ScreenWidth;
    const int32_t widthForConversion =
        static_cast<int32_t>(g_ScreenWidth);
    double convertedWidth = static_cast<double>(widthForConversion);
    if (static_cast<int32_t>(widthForSign) < 0)
        convertedWidth += static_cast<double>(g_UnsignedIntegerBias);
    const float ratioX = static_cast<float>(convertedX / convertedWidth);

    return static_cast<uint32_t>(
        ((int (__fastcall*)(void*, void*, float, float))panelVtable[84])(
            currentPanel, nullptr, ratioX, ratioY));
}

HOOK(0x00544780, CMenuSaveGame_ResolvePanelAndApplyScale, "auto");
