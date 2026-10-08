#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CMenuMessageBox_InitializeFourControls, 0x0052A0F0,
         (CMenuMessageBox* self, void* edx, void* context));
CSF_FUNC(int, __fastcall, CMenuMessageBox_InitializeSlotButtons, 0x0052A1B0,
         (CMenuMessageBox* self, void* edx, void* context));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160,
         (void* self, void* edx));
CSF_FUNC(void*, __fastcall, LookupNamedElement, 0x00693360,
         (void* self, void* edx, const char* name));
CSF_FUNC(void*, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_Const20, 0x0082E0F0);
CSF_GLOBAL(std::uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(std::uint32_t, g_ScreenHeight, 0x00899BA0);

static double MenuMessageBox_ConvertUnsigned(std::uint32_t value)
{
    double converted = static_cast<double>(static_cast<std::int32_t>(value));
    if (static_cast<std::int32_t>(value) < 0)
        converted += static_cast<double>(g_UnsignedIntegerBias);
    return converted;
}

static int __fastcall CMenuMessageBox_InitializePanel(
    CMenuMessageBox* self, void* /*edx*/, void* context, void* nameContext)
{
    if (self->field_0894 != 0)
        return static_cast<int>(self->field_0894);

    void* resource = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, context, 0)[0x54 / 4])(context, nullptr);
    void** resourceVtable = CSF_AT(void**, resource, 0);
    void* nameResolver = CSF_AT(void*, nameContext, 0);
    void* panelName = LookupNamedElement(
        nameResolver, nullptr, CSF_STR(0x00841304, ".PANEL_PRINCIPAL"));

    void* panel = ((void* (__fastcall*)(void*, void*, void*))
        resourceVtable[0x18 / 4])(resource, nullptr, panelName);
    self->field_0894 = reinterpret_cast<std::uint32_t>(panel);
    if (!panel)
        return 0;

    ((void (__fastcall*)(void*, void*, int))
        CSF_AT(void**, panel, 0)[0xC8 / 4])(panel, nullptr, 1);

    CMenuMessageBox_InitializeFourControls(self, nullptr, context);
    CMenuMessageBox_InitializeSlotButtons(self, nullptr, context);

    void* globalState = GetGlobalValue_8BC1E0();
    panel = reinterpret_cast<void*>(self->field_0894);
    std::uint32_t panelStyle = CSF_AT(std::uint32_t, globalState, 0x18F4);
    ((void (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, panel, 0)[0x170 / 4])(panel, nullptr, panelStyle);

    void* display = LookupObjectById(
        GetGlobalInt(), nullptr);
    void* dimensions = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    std::uint32_t displayWidth = CSF_AT(std::uint32_t, dimensions, 0);

    display = LookupObjectById(
        GetGlobalInt(), nullptr);
    dimensions = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    std::uint32_t displayHeight = CSF_AT(std::uint32_t, dimensions, 4);

    panel = reinterpret_cast<void*>(self->field_0894);
    void** panelVtable = CSF_AT(void**, panel, 0);
    double heightNumerator = MenuMessageBox_ConvertUnsigned(displayHeight);
    double heightDenominator = MenuMessageBox_ConvertUnsigned(g_ScreenHeight);
    float heightScale = static_cast<float>(heightNumerator / heightDenominator);
    double widthNumerator = MenuMessageBox_ConvertUnsigned(displayWidth);
    double widthDenominator = MenuMessageBox_ConvertUnsigned(g_ScreenWidth);
    float widthScale = static_cast<float>(widthNumerator / widthDenominator);
    ((void (__fastcall*)(void*, void*, float, float))
        panelVtable[0x150 / 4])(panel, nullptr, widthScale, heightScale);

    display = LookupObjectById(
        GetGlobalInt(), nullptr);
    dimensions = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    double signedWidth = static_cast<double>(CSF_AT(std::int32_t, dimensions, 0));
    double screenWidth = MenuMessageBox_ConvertUnsigned(g_ScreenWidth);
    float positionScale = static_cast<float>(signedWidth / screenWidth);

    panel = reinterpret_cast<void*>(self->field_0894);
    void* movable = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, panel, 0)[0x1CC / 4])(panel, nullptr);
    if (movable)
    {
        void* currentPosition = ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, movable, 0)[0x28 / 4])(movable, nullptr);
        std::uint32_t position[2];
        position[0] = CSF_AT(std::uint32_t, currentPosition, 0);
        double displacement = static_cast<double>(positionScale)
                            * static_cast<double>(g_Const20);
        position[1] = CSF_AT(std::uint32_t, currentPosition, 4);
        position[0] += static_cast<std::uint32_t>(
            static_cast<std::int64_t>(displacement));
        self->field_0970 = position[0];
        ((void (__fastcall*)(void*, void*, std::uint32_t*))
            CSF_AT(void**, movable, 0)[0x24 / 4])(movable, nullptr, position);
    }

    panel = reinterpret_cast<void*>(self->field_0894);
    void* okButton = ((void* (__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, panel, 0)[0xFC / 4])(
            panel, nullptr, CSF_STR(0x008412F4, "BotonOK"));
    self->field_002c = reinterpret_cast<std::uint32_t>(okButton);
    if (okButton)
    {
        void* buttonPosition = ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, okButton, 0)[0xA4 / 4])(okButton, nullptr);
        self->field_0968 = CSF_AT(std::uint32_t, buttonPosition, 0);
        self->field_096c = CSF_AT(std::uint32_t, buttonPosition, 4);
    }

    panel = reinterpret_cast<void*>(self->field_0894);
    void* staticControl = ((void* (__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, panel, 0)[0xFC / 4])(
            panel, nullptr, CSF_STR(0x00841274, "Static"));
    self->m_staticControl = reinterpret_cast<std::uint32_t>(staticControl);
    return static_cast<int>(self->m_staticControl);
}

HOOK(0x0052A280, CMenuMessageBox_InitializePanel, "auto");
