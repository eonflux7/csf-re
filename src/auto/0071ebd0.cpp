#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_FindProperty, 0x006932B0, (S_00692b80* self, void* , char* name));
CSF_FUNC(S_00692b80*, __fastcall, LookupNamedElement, 0x00693360, (S_00692b80* self, void* , char* String1));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (S_00692b80* self, void* , char* attributeName));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, CGuiControl_SetSelectable, 0x007186D0, (void* self, void* , char a2));
CSF_FUNC(int, __fastcall, GetObjectAndSetComponents, 0x0071E2C0, (void* self, void* , std::uint8_t* attributes));
CSF_FUNC(int, __fastcall, CGuiBuilder_CreateColorQuad, 0x0071E370, (CGuiBuilder* self, void* , std::uint8_t* node));
CSF_FUNC(int, __fastcall, CGuiBuilder_CreateTextureQuad, 0x0071E4E0, (CGuiBuilder* self, void* , S_00692b80* properties));
CSF_FUNC(void, __fastcall, CGuiBuilder_ApplyTextProperties, 0x0071E780, (void* self, void* , std::uint8_t* properties, void* target));
CSF_FUNC(void, __stdcall, ApplyConfiguredRectangle, 0x0071E960, (unsigned char* configuration, int objectAddress));
CSF_FUNC(void, __fastcall, GuiBuilder_ApplyColorProperties, 0x0071EAC0, (void* self, void* , unsigned char* node, void* target));

CSF_GLOBAL(const char*, g_pszKey_XY, 0x008A85EC);
CSF_GLOBAL(const char*, g_pszKey_SIZE_gui_builder, 0x008A85F0);
CSF_GLOBAL(const char*, g_pszKey_ID_gui_builder, 0x008A8600);
CSF_GLOBAL(const char*, g_pszKey_ID_LEFT, 0x008A8608);
CSF_GLOBAL(const char*, g_pszKey_ID_RIGHT, 0x008A860C);
CSF_GLOBAL(const char*, g_pszKey_ID_UP, 0x008A8610);
CSF_GLOBAL(const char*, g_pszKey_ID_DOWN, 0x008A8614);
CSF_GLOBAL(const char*, g_pszKey_MOUSE, 0x008A8618);
CSF_GLOBAL(const char*, g_pszKey_DRAG, 0x008A861C);
CSF_GLOBAL(const char*, g_pszKey_DEBUG_RECT, 0x008A8620);
CSF_GLOBAL(const char*, g_pszKey_FONDO, 0x008A8664);
CSF_GLOBAL(const char*, g_pszKey_QUAD_BACKGROUND, 0x008A86EC);
CSF_GLOBAL(const char*, g_pszKey_QUAD_VARIABLE, 0x008A86F0);
CSF_GLOBAL(const char*, g_pszKey_QUAD_FOCUS, 0x008A86F4);
CSF_GLOBAL(const char*, g_pszKey_QUAD_BLINKING_FOCUS, 0x008A86F8);
CSF_GLOBAL(const char*, g_pszKey_QUAD_SELECTED, 0x008A86FC);
CSF_GLOBAL(const char*, g_pszKey_SELECCIONABLE, 0x008A8708);
CSF_GLOBAL(const char*, g_pszKey_BORDER_FOCUS, 0x008A871C);
CSF_GLOBAL(const char*, g_pszKey_BORDER, 0x008A8724);
CSF_GLOBAL(const char*, g_pszKey_BORDER_TEX, 0x008A8728);
CSF_GLOBAL(const char*, g_pszKey_VIDEO_gui_builder, 0x008A8740);

static std::uint8_t* __fastcall CGuiBuilder_ReadControlProps(
    CGuiBuilder* self, void* /*edx*/, int objectAddress, S_00692b80* properties)
{
    void* control = reinterpret_cast<void*>(objectAddress);
    auto* propertyBytes = reinterpret_cast<std::uint8_t*>(properties);
    alignas(8) std::uint8_t pairBytes[0xC];
    alignas(8) char textBuffer[0x100];
    alignas(8) char videoBuffer[0x100];
    std::memset(textBuffer, 0, sizeof(textBuffer));

    S_00692b80* position = LookupNamedElement(
        properties, nullptr, const_cast<char*>(g_pszKey_XY));
    S_00692b80* xComponent = S_00692b80_GetComponent(position, nullptr, 0);
    S_00692b80* yComponent = S_00692b80_GetComponent(position, nullptr, 1u);
    int y = S_00692b80_ParseIntegerComponent(yComponent, nullptr);
    CSF_AT(int, pairBytes, 0) = S_00692b80_ParseIntegerComponent(xComponent, nullptr);
    CSF_AT(int, pairBytes, 4) = y;
    ((void(__fastcall*)(void*, void*, int*))CSF_AT(void**, control, 0)[38])(
        control, nullptr, reinterpret_cast<int*>(pairBytes));

    S_00692b80* property = S_00692b80_FindProperty(
        properties, nullptr, const_cast<char*>(g_pszKey_SIZE_gui_builder));
    if (property)
    {
        S_00692b80* widthComponent = S_00692b80_GetComponent(property, nullptr, 0);
        S_00692b80* heightComponent = S_00692b80_GetComponent(property, nullptr, 1u);
        int height = S_00692b80_ParseIntegerComponent(heightComponent, nullptr);
        int width = S_00692b80_ParseIntegerComponent(widthComponent, nullptr);
        void** vtable = CSF_AT(void**, control, 0);
        CSF_AT(int, pairBytes, 0) = width;
        CSF_AT(int, pairBytes, 4) = height;
        ((void(__fastcall*)(void*, void*, int*))vtable[40])(
            control, nullptr, reinterpret_cast<int*>(pairBytes));
    }

    MenuExtras_ReadText(propertyBytes, nullptr,
        const_cast<char*>(g_pszKey_ID_gui_builder), textBuffer, 256);
    ((void(__fastcall*)(void*, void*, char*))CSF_AT(void**, control, 0)[28])(
        control, nullptr, textBuffer);

    MenuExtras_ReadText(propertyBytes, nullptr,
        const_cast<char*>(g_pszKey_ID_LEFT), textBuffer, 256);
    ((void(__fastcall*)(void*, void*, char*))CSF_AT(void**, control, 0)[30])(
        control, nullptr, textBuffer);

    MenuExtras_ReadText(propertyBytes, nullptr,
        const_cast<char*>(g_pszKey_ID_RIGHT), textBuffer, 256);
    ((void(__fastcall*)(void*, void*, char*))CSF_AT(void**, control, 0)[31])(
        control, nullptr, textBuffer);

    MenuExtras_ReadText(propertyBytes, nullptr,
        const_cast<char*>(g_pszKey_ID_UP), textBuffer, 256);
    ((void(__fastcall*)(void*, void*, char*))CSF_AT(void**, control, 0)[32])(
        control, nullptr, textBuffer);

    MenuExtras_ReadText(propertyBytes, nullptr,
        const_cast<char*>(g_pszKey_ID_DOWN), textBuffer, 256);
    ((void(__fastcall*)(void*, void*, char*))CSF_AT(void**, control, 0)[33])(
        control, nullptr, textBuffer);

    property = S_00692b80_FindProperty(
        properties, nullptr, const_cast<char*>(g_pszKey_SELECCIONABLE));
    if (property)
    {
        int selectable = S_00692b80_ParseIntegerComponent(property, nullptr);
        CGuiControl_SetSelectable(control, nullptr, selectable == 1);
    }

    property = S_00692b80_FindProperty(
        properties, nullptr, const_cast<char*>(g_pszKey_BORDER));
    if (property)
        CGuiBuilder_ApplyTextProperties(
            self, nullptr, reinterpret_cast<std::uint8_t*>(property), control);

    property = S_00692b80_FindProperty(
        properties, nullptr, const_cast<char*>(g_pszKey_BORDER_TEX));
    if (property)
        ApplyConfiguredRectangle(
            reinterpret_cast<unsigned char*>(property), objectAddress);

    if (S_00692b80_FindProperty(
            properties, nullptr, const_cast<char*>(g_pszKey_MOUSE)))
    {
        void** vtable = CSF_AT(void**, control, 0);
        bool enabled = ReadIntegerAttribute(
            properties, nullptr, const_cast<char*>(g_pszKey_MOUSE)) != 0;
        ((void(__fastcall*)(void*, void*, bool))vtable[48])(
            control, nullptr, enabled);
    }

    if (S_00692b80_FindProperty(
            properties, nullptr, const_cast<char*>(g_pszKey_DRAG)))
    {
        void** vtable = CSF_AT(void**, control, 0);
        bool enabled = ReadIntegerAttribute(
            properties, nullptr, const_cast<char*>(g_pszKey_DRAG)) != 0;
        ((void(__fastcall*)(void*, void*, bool))vtable[44])(
            control, nullptr, enabled);
    }

    if (S_00692b80_FindProperty(
            properties, nullptr, const_cast<char*>(g_pszKey_DEBUG_RECT)))
    {
        void** vtable = CSF_AT(void**, control, 0);
        bool enabled = ReadIntegerAttribute(
            properties, nullptr, const_cast<char*>(g_pszKey_DEBUG_RECT)) != 0;
        ((void(__fastcall*)(void*, void*, bool))vtable[96])(
            control, nullptr, enabled);
    }

    property = S_00692b80_FindProperty(
        properties, nullptr, const_cast<char*>(g_pszKey_QUAD_SELECTED));
    if (property)
    {
        void** vtable = CSF_AT(void**, control, 0);
        int quad = GetObjectAndSetComponents(
            self, nullptr, reinterpret_cast<std::uint8_t*>(property));
        ((void(__fastcall*)(void*, void*, int))vtable[124])(control, nullptr, quad);
    }

    property = S_00692b80_FindProperty(
        properties, nullptr, const_cast<char*>(g_pszKey_QUAD_FOCUS));
    if (property)
    {
        void** vtable = CSF_AT(void**, control, 0);
        int quad = GetObjectAndSetComponents(
            self, nullptr, reinterpret_cast<std::uint8_t*>(property));
        ((void(__fastcall*)(void*, void*, int))vtable[121])(control, nullptr, quad);
    }

    property = S_00692b80_FindProperty(
        properties, nullptr, const_cast<char*>(g_pszKey_QUAD_BLINKING_FOCUS));
    if (property)
    {
        void** vtable = CSF_AT(void**, control, 0);
        int quad = GetObjectAndSetComponents(
            self, nullptr, reinterpret_cast<std::uint8_t*>(property));
        ((void(__fastcall*)(void*, void*, int))vtable[122])(control, nullptr, quad);
    }

    property = S_00692b80_FindProperty(
        properties, nullptr, const_cast<char*>(g_pszKey_BORDER_FOCUS));
    if (property)
        GuiBuilder_ApplyColorProperties(
            self, nullptr, reinterpret_cast<unsigned char*>(property), control);

    property = S_00692b80_FindProperty(
        properties, nullptr, const_cast<char*>(g_pszKey_QUAD_VARIABLE));
    if (property)
    {
        void** vtable = CSF_AT(void**, control, 0);
        int quad = CGuiBuilder_CreateColorQuad(
            self, nullptr, reinterpret_cast<std::uint8_t*>(property));
        ((void(__fastcall*)(void*, void*, int))vtable[125])(control, nullptr, quad);
    }

    property = S_00692b80_FindProperty(
        properties, nullptr, const_cast<char*>(g_pszKey_QUAD_BACKGROUND));
    if (property)
    {
        void** vtable = CSF_AT(void**, control, 0);
        int quad = GetObjectAndSetComponents(
            self, nullptr, reinterpret_cast<std::uint8_t*>(property));
        ((void(__fastcall*)(void*, void*, int))vtable[123])(control, nullptr, quad);
    }

    property = S_00692b80_FindProperty(
        properties, nullptr, const_cast<char*>(g_pszKey_FONDO));
    if (property)
    {
        int textureQuad = CGuiBuilder_CreateTextureQuad(self, nullptr, property);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, control, 0)[54])(
            control, nullptr, textureQuad);
        if (textureQuad)
            ReleaseObjectReference(
                reinterpret_cast<std::uint8_t*>(textureQuad) + 4, nullptr);
    }

    property = S_00692b80_FindProperty(
        properties, nullptr, const_cast<char*>(g_pszKey_VIDEO_gui_builder));
    if (property)
    {
        std::memset(videoBuffer, 0, sizeof(videoBuffer));
        MenuExtras_ReadText(propertyBytes, nullptr,
            const_cast<char*>(g_pszKey_VIDEO_gui_builder), videoBuffer, 256);
        return ((std::uint8_t*(__fastcall*)(void*, void*, char*))
            CSF_AT(void**, control, 0)[99])(control, nullptr, videoBuffer);
    }
    return nullptr;
}

HOOK(0x0071EBD0, CGuiBuilder_ReadControlProps, "auto");
