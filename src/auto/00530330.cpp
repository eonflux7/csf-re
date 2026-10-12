#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(unsigned int, __cdecl, CopyTrucoStringToWide, 0x00695590, (const char* src, uint16_t* dst, int maxCount));
CSF_FUNC(void, __cdecl, MenuLoading_CopyLocalizedText, 0x006956C0, (uint16_t* destination, const uint16_t* source, int capacity));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, CGuiTextureListBox_ExtendToRowIndex, 0x00717970, (CGuiTextureListBox* self, void* , int rowIndex));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static uint32_t MenuMpServerInfo_FindChild(void* panel, const char* name)
{
    return ((uint32_t(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, panel, 0)[0xFC / 4])(panel, nullptr, name);
}

static const uint16_t* MenuMpServerInfo_GetControlText(void* control)
{
    void* textObject = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, control, 0)[0x110 / 4])(control, nullptr);
    return ((const uint16_t*(__fastcall*)(void*, void*))
        CSF_AT(void**, textObject, 0)[0x44 / 4])(textObject, nullptr);
}

static int __fastcall CMenuMpServerInfo_InitializePanel(
    CMenuMpServerInfo* self, void* /*edx*/, void* gui,
    S_00692b80** elements)
{
    if (self->field_01ec)
        return static_cast<int>(self->field_01ec);

    void* factory = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, gui, 0)[0x54 / 4])(gui, nullptr);
    void** factoryVtable = CSF_AT(void**, factory, 0);
    uint8_t* element = LookupNamedElement(
        reinterpret_cast<uint8_t*>(*elements), nullptr,
        CSF_STR(0x00841304, ".PANEL_PRINCIPAL"));
    uint32_t panelResult = ((uint32_t(__fastcall*)(void*, void*, uint8_t*))
        factoryVtable[0x18 / 4])(factory, nullptr, element);
    self->field_01ec = panelResult;
    if (!panelResult)
        return 0;

    void* display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    unsigned int width = CSF_AT(unsigned int, dimensions, 0);

    display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    unsigned int height = CSF_AT(unsigned int, dimensions, 0x4);

    void* panel = reinterpret_cast<void*>(self->field_01ec);
    ((void(__fastcall*)(void*, void*, float, float))
        CSF_AT(void**, panel, 0)[0x150 / 4])(
            panel, nullptr,
            static_cast<float>(width) / static_cast<float>(g_ScreenWidth),
            static_cast<float>(height) / static_cast<float>(g_ScreenHeight));

    self->field_0028 = MenuMpServerInfo_FindChild(
        reinterpret_cast<void*>(self->field_01ec),
        CSF_STR(0x00841A8C, "ClavesTeam1"));
    self->field_002c = MenuMpServerInfo_FindChild(
        reinterpret_cast<void*>(self->field_01ec),
        CSF_STR(0x00841A80, "ClavesTeam2"));
    self->field_0030 = MenuMpServerInfo_FindChild(
        reinterpret_cast<void*>(self->field_01ec),
        CSF_STR(0x00841A78, "Tiempo"));
    self->field_0034 = MenuMpServerInfo_FindChild(
        reinterpret_cast<void*>(self->field_01ec),
        CSF_STR(0x00841A70, "Mapa"));
    self->field_0038 = MenuMpServerInfo_FindChild(
        reinterpret_cast<void*>(self->field_01ec),
        CSF_STR(0x00841A64, "TiempoValor"));
    self->field_003c = MenuMpServerInfo_FindChild(
        reinterpret_cast<void*>(self->field_01ec),
        CSF_STR(0x00841A58, "MapaValor"));
    self->field_0040 = MenuMpServerInfo_FindChild(
        reinterpret_cast<void*>(self->field_01ec),
        CSF_STR(0x00841A50, "Server"));
    self->field_0044 = MenuMpServerInfo_FindChild(
        reinterpret_cast<void*>(self->field_01ec),
        CSF_STR(0x00841A48, "Frase"));
    self->field_0048 = MenuMpServerInfo_FindChild(
        reinterpret_cast<void*>(self->field_01ec),
        CSF_STR(0x00841A40, "Titulo"));
    self->field_0010 = MenuMpServerInfo_FindChild(
        reinterpret_cast<void*>(self->field_01ec),
        CSF_STR(0x00841A34, "StatDMatch"));
    self->field_0014 = MenuMpServerInfo_FindChild(
        reinterpret_cast<void*>(self->field_01ec),
        CSF_STR(0x00841A24, "StatRojoTDMatch"));
    self->field_0018 = MenuMpServerInfo_FindChild(
        reinterpret_cast<void*>(self->field_01ec),
        CSF_STR(0x00841A14, "StatAzulTDMatch"));
    self->field_001c = MenuMpServerInfo_FindChild(
        reinterpret_cast<void*>(self->field_01ec),
        CSF_STR(0x00841A00, "StatEspectadores"));

    if (self->field_0010)
        CGuiTextureListBox_ExtendToRowIndex(
            reinterpret_cast<CGuiTextureListBox*>(self->field_0010), nullptr, 12);
    if (self->field_0014)
        CGuiTextureListBox_ExtendToRowIndex(
            reinterpret_cast<CGuiTextureListBox*>(self->field_0014), nullptr, 12);
    if (self->field_0018)
        CGuiTextureListBox_ExtendToRowIndex(
            reinterpret_cast<CGuiTextureListBox*>(self->field_0018), nullptr, 12);
    if (self->field_001c)
        CGuiTextureListBox_ExtendToRowIndex(
            reinterpret_cast<CGuiTextureListBox*>(self->field_001c), nullptr, 12);

    if (!self->field_0030) return 0;
    if (!self->field_0034) return 0;
    if (!self->field_0038) return 0;
    if (!self->field_003c) return 0;
    if (!self->field_0028) return 0;
    if (!self->field_002c) return 0;

    CopyTrucoStringToWide(
        CSF_STR(0x008419F8, "{s0}"), &self->field_0050, 5);
    CopyTrucoStringToWide(
        CSF_STR(0x008419F8, "{s0}"), &CSF_AT(uint16_t, self, 0xB4), 5);

    const uint16_t* firstText = MenuMpServerInfo_GetControlText(
        reinterpret_cast<void*>(self->field_0028));
    MenuLoading_CopyLocalizedText(
        &CSF_AT(uint16_t, self, 0x118), firstText, 50);

    const uint16_t* secondText = MenuMpServerInfo_GetControlText(
        reinterpret_cast<void*>(self->field_002c));
    uint16_t* destination = &CSF_AT(uint16_t, self, 0x17C);
    MenuLoading_CopyLocalizedText(destination, secondText, 50);

    // The original exposes the copy routine's advanced source pointer in EAX.
    if (!secondText)
        return 0;
    int copied = 0;
    do {
        ++copied;
    } while (destination[copied - 1] != 0 && copied < 50);
    return static_cast<int>(reinterpret_cast<uintptr_t>(secondText + copied));
}

HOOK(0x00530330, CMenuMpServerInfo_InitializePanel, "auto");
