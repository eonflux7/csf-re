#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, CGuiControl_SetSelectable, 0x007186D0, (void* self, void* , char a2));
CSF_FUNC(int, __fastcall, GetObjectAndSetComponents, 0x0071E2C0, (void* self, void* , std::uint8_t* attributes));
CSF_FUNC(int, __fastcall, CGuiBuilder_CreateColorQuad, 0x0071E370, (CGuiBuilder* self, void* , std::uint8_t* node));
CSF_FUNC(int, __fastcall, CGuiBuilder_CreateTextureQuad, 0x0071E4E0, (CGuiBuilder* self, void* edx, std::uint8_t* properties));
CSF_FUNC(void, __fastcall, CGuiBuilder_ApplyTextProperties, 0x0071E780, (void* self, void* , std::uint8_t* properties, void* target));
CSF_FUNC(void, __stdcall, ApplyConfiguredRectangle, 0x0071E960, (unsigned char* configuration, int objectAddress));

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
CSF_GLOBAL(const char*, g_pszKey_BORDER, 0x008A8724);
CSF_GLOBAL(const char*, g_pszKey_BORDER_TEX, 0x008A8728);
CSF_GLOBAL(const char*, g_pszKey_VIDEO_gui_builder, 0x008A8740);

static std::uint8_t* __fastcall CGuiBuilder_ReadControlAppearance(
    CGuiBuilder* self, void* /*edx*/, int objectAddress, S_00692b80* attributes)
{
    alignas(8) std::uint8_t textBytes[0x100];
    alignas(8) std::uint8_t videoBytes[0x100];
    alignas(8) std::uint8_t dimensionsBytes[0xC];
    std::memset(textBytes, 0, sizeof(textBytes));

    if (!attributes)
        return nullptr;

    auto* node = reinterpret_cast<std::uint8_t*>(attributes);
    void* control = reinterpret_cast<void*>(objectAddress);
    char* text = reinterpret_cast<char*>(textBytes);

    if (auto* property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_XY)))
    {
        auto* first = S_00692b80_GetComponent(
            reinterpret_cast<S_00692b80*>(property), nullptr, 0);
        auto* second = S_00692b80_GetComponent(
            reinterpret_cast<S_00692b80*>(property), nullptr, 1u);
        int y = S_00692b80_ParseIntegerComponent(second, nullptr);
        int x = S_00692b80_ParseIntegerComponent(first, nullptr);
        void** vtable = CSF_AT(void**, control, 0);
        CSF_AT(int, dimensionsBytes, 0) = x;
        CSF_AT(int, dimensionsBytes, 4) = y;
        ((void (__fastcall*)(void*, void*, int*))vtable[152 / 4])(
            control, nullptr, reinterpret_cast<int*>(dimensionsBytes));
    }

    if (auto* property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_SIZE_gui_builder)))
    {
        auto* first = S_00692b80_GetComponent(
            reinterpret_cast<S_00692b80*>(property), nullptr, 0);
        auto* second = S_00692b80_GetComponent(
            reinterpret_cast<S_00692b80*>(property), nullptr, 1u);
        int height = S_00692b80_ParseIntegerComponent(second, nullptr);
        int width = S_00692b80_ParseIntegerComponent(first, nullptr);
        void** vtable = CSF_AT(void**, control, 0);
        CSF_AT(int, dimensionsBytes, 0) = width;
        CSF_AT(int, dimensionsBytes, 4) = height;
        ((void (__fastcall*)(void*, void*, int*))vtable[160 / 4])(
            control, nullptr, reinterpret_cast<int*>(dimensionsBytes));
    }

    if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_ID_gui_builder)))
    {
        MenuExtras_ReadText(node, nullptr, const_cast<char*>(g_pszKey_ID_gui_builder), text, 256);
        ((void (__fastcall*)(void*, void*, char*))CSF_AT(void**, control, 0)[112 / 4])(
            control, nullptr, text);
    }
    if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_ID_LEFT)))
    {
        MenuExtras_ReadText(node, nullptr, const_cast<char*>(g_pszKey_ID_LEFT), text, 256);
        ((void (__fastcall*)(void*, void*, char*))CSF_AT(void**, control, 0)[120 / 4])(
            control, nullptr, text);
    }
    if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_ID_RIGHT)))
    {
        MenuExtras_ReadText(node, nullptr, const_cast<char*>(g_pszKey_ID_RIGHT), text, 256);
        ((void (__fastcall*)(void*, void*, char*))CSF_AT(void**, control, 0)[124 / 4])(
            control, nullptr, text);
    }
    if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_ID_UP)))
    {
        MenuExtras_ReadText(node, nullptr, const_cast<char*>(g_pszKey_ID_UP), text, 256);
        ((void (__fastcall*)(void*, void*, char*))CSF_AT(void**, control, 0)[128 / 4])(
            control, nullptr, text);
    }
    if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_ID_DOWN)))
    {
        MenuExtras_ReadText(node, nullptr, const_cast<char*>(g_pszKey_ID_DOWN), text, 256);
        ((void (__fastcall*)(void*, void*, char*))CSF_AT(void**, control, 0)[132 / 4])(
            control, nullptr, text);
    }

    if (auto* property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_SELECCIONABLE)))
    {
        int selectable = S_00692b80_ParseIntegerComponent(
            reinterpret_cast<S_00692b80*>(property), nullptr);
        CGuiControl_SetSelectable(control, nullptr, selectable == 1);
    }
    if (auto* property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_BORDER)))
        CGuiBuilder_ApplyTextProperties(self, nullptr, property, control);
    if (auto* property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_BORDER_TEX)))
        ApplyConfiguredRectangle(property, objectAddress);

    if (auto* property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_QUAD_VARIABLE)))
    {
        void** vtable = CSF_AT(void**, control, 0);
        int quad = CGuiBuilder_CreateColorQuad(self, nullptr, property);
        ((void (__fastcall*)(void*, void*, int))vtable[500 / 4])(control, nullptr, quad);
    }

    if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_MOUSE)))
    {
        void** vtable = CSF_AT(void**, control, 0);
        int value = ReadIntegerAttribute(node, nullptr, const_cast<char*>(g_pszKey_MOUSE));
        std::uint32_t enabled =
            (reinterpret_cast<std::uintptr_t>(node) & 0xFFFFFF00u) |
            static_cast<std::uint32_t>(value != 0);
        ((void (__fastcall*)(void*, void*, std::uint32_t))vtable[192 / 4])(
            control, nullptr, enabled);
    }
    if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_DRAG)))
    {
        void** vtable = CSF_AT(void**, control, 0);
        int value = ReadIntegerAttribute(node, nullptr, const_cast<char*>(g_pszKey_DRAG));
        std::uint32_t enabled =
            (reinterpret_cast<std::uintptr_t>(node) & 0xFFFFFF00u) |
            static_cast<std::uint32_t>(value != 0);
        ((void (__fastcall*)(void*, void*, std::uint32_t))vtable[176 / 4])(
            control, nullptr, enabled);
    }
    if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_DEBUG_RECT)))
    {
        void** vtable = CSF_AT(void**, control, 0);
        int value = ReadIntegerAttribute(node, nullptr, const_cast<char*>(g_pszKey_DEBUG_RECT));
        std::uint32_t enabled =
            (reinterpret_cast<std::uintptr_t>(node) & 0xFFFFFF00u) |
            static_cast<std::uint32_t>(value != 0);
        ((void (__fastcall*)(void*, void*, std::uint32_t))vtable[384 / 4])(
            control, nullptr, enabled);
    }

    if (auto* property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_QUAD_FOCUS)))
    {
        void** vtable = CSF_AT(void**, control, 0);
        int quad = GetObjectAndSetComponents(self, nullptr, property);
        ((void (__fastcall*)(void*, void*, int))vtable[484 / 4])(control, nullptr, quad);
    }
    if (auto* property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_QUAD_BLINKING_FOCUS)))
    {
        void** vtable = CSF_AT(void**, control, 0);
        int quad = GetObjectAndSetComponents(self, nullptr, property);
        ((void (__fastcall*)(void*, void*, int))vtable[488 / 4])(control, nullptr, quad);
    }
    if (auto* property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_QUAD_SELECTED)))
    {
        void** vtable = CSF_AT(void**, control, 0);
        int quad = GetObjectAndSetComponents(self, nullptr, property);
        ((void (__fastcall*)(void*, void*, int))vtable[496 / 4])(control, nullptr, quad);
    }
    if (auto* property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_QUAD_BACKGROUND)))
    {
        void** vtable = CSF_AT(void**, control, 0);
        int quad = GetObjectAndSetComponents(self, nullptr, property);
        ((void (__fastcall*)(void*, void*, int))vtable[492 / 4])(control, nullptr, quad);
    }

    if (auto* property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_FONDO)))
    {
        int quad = CGuiBuilder_CreateTextureQuad(self, nullptr, property);
        ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, control, 0)[216 / 4])(
            control, nullptr, quad);
        if (quad)
            ReleaseObjectReference(reinterpret_cast<std::uint8_t*>(quad) + 4, nullptr);
    }

    auto* videoProperty = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_VIDEO_gui_builder));
    if (videoProperty)
    {
        std::memset(videoBytes, 0, sizeof(videoBytes));
        char* video = reinterpret_cast<char*>(videoBytes);
        MenuExtras_ReadText(node, nullptr, const_cast<char*>(g_pszKey_VIDEO_gui_builder), video, 256);
        int result = ((int (__fastcall*)(void*, void*, char*))CSF_AT(void**, control, 0)[396 / 4])(
            control, nullptr, video);
        return reinterpret_cast<std::uint8_t*>(result);
    }
    return videoProperty;
}

HOOK(0x0071EFC0, CGuiBuilder_ReadControlAppearance, "auto");
