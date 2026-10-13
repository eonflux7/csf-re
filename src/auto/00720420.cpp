#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00692b80_GetChildCount, 0x00692B80, (S_00692b80* self, void*));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(void, __fastcall, ForwardToEmbeddedObjectVtableSlot11, 0x00714280, (void* self, void* edx, float* color));
CSF_FUNC(void, __fastcall, GetOrCreateCachedObjectAndTailDispatch, 0x007142E0, (void* self, void* edx, float* color));
CSF_FUNC(int, __fastcall, SetFlagsAndUpdateNamedEntry, 0x00714880, (std::uint8_t* self, void* , int value));
CSF_FUNC(std::uint32_t, __fastcall, InvokeLazyIndexedResource, 0x007155E0, (void* self, void* , std::uint32_t index, int argument));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_SetColumnWidth, 0x00716520, (CGuiListBoxBase* self, void* , std::uint32_t columnIndex, std::int32_t width));
CSF_FUNC(char, __stdcall, CGuiBuilder_ReadFourComponents, 0x0071DA70, (std::uint8_t* node, float* output));
CSF_FUNC(int, __fastcall, CGuiBuilder_GetFont, 0x0071DB60, (CGuiBuilder* self, void* , std::uint8_t* properties));
CSF_FUNC(uint8_t*, __fastcall, CGuiBuilder_ReadControlProps, 0x0071EBD0, (CGuiBuilder* self, void* edx, void* control, uint8_t* node));
CSF_FUNC(void*, __fastcall, CGuiBuilder_CreateText, 0x0071F910, (CGuiBuilder* self, void* edx, uint8_t* node));

CSF_GLOBAL(const char*, g_pszKey_COLOR, 0x008A8628);
CSF_GLOBAL(const char*, g_pszKey_COLOR_NOFOCUS, 0x008A862C);
CSF_GLOBAL(const char*, g_pszKey_LINEAS, 0x008A8670);
CSF_GLOBAL(const char*, g_pszKey_COLOR_LINEAS, 0x008A8674);
CSF_GLOBAL(const char*, g_pszKey_AUTOSIZE, 0x008A8684);
CSF_GLOBAL(const char*, g_pszKey_ROWFILL, 0x008A8688);
CSF_GLOBAL(const char*, g_pszKey_INTERLINEA, 0x008A868C);
CSF_GLOBAL(const char*, g_pszKey_CABECERA, 0x008A8690);
CSF_GLOBAL(const char*, g_pszKey_COLUMNAS, 0x008A8694);
CSF_GLOBAL(const char*, g_pszKey_COLOR_FONDO, 0x008A8698);
CSF_GLOBAL(const char*, g_pszKey_WIDTH_gui_builder, 0x008A869C);
CSF_GLOBAL(const char*, g_pszKey_TEXTO, 0x008A86BC);
CSF_GLOBAL(const char*, g_pszKey_COLOR_FRAME, 0x008A8738);
CSF_GLOBAL(const char*, g_pszKey_CYCLE, 0x008A8748);
CSF_GLOBAL(const char*, g_pszKey_FIXED_COLORBLINKINGBORDER, 0x008A878C);

[[maybe_unused]] static const auto kPropertyNodeVtable = CSF_VTBL(0x00692B80);

static void CGuiBuilder_InitializeDefaultColor(float* color)
{
    color[0] = 0.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 1.0f;
}

static void __fastcall CGuiBuilder_ReadTextControlProps(
    CGuiBuilder* self, void* /*edx*/, S_00692b80* properties, void* control)
{
    if (!properties || !control)
        return;

    auto* node = reinterpret_cast<std::uint8_t*>(properties);
    alignas(8) std::uint8_t temporaryColorBytes[0x10];
    alignas(8) std::uint8_t persistentColorBytes[0x20];
    auto* temporaryColor = reinterpret_cast<float*>(temporaryColorBytes);
    auto* persistentColor = reinterpret_cast<float*>(persistentColorBytes);
    CGuiBuilder_InitializeDefaultColor(persistentColor);

    auto* property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_COLOR_FRAME));
    if (property)
    {
        float* frameColor;
        if (FindProperty(property, nullptr, const_cast<char*>(g_pszKey_FIXED_COLORBLINKINGBORDER)))
        {
            frameColor = &CSF_AT(float, self, 0x70);
        }
        else
        {
            CGuiBuilder_InitializeDefaultColor(temporaryColor);
            auto* colorNode = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_COLOR_FRAME));
            CGuiBuilder_ReadFourComponents(colorNode, temporaryColor);
            frameColor = temporaryColor;
        }
        SetFlagsAndUpdateNamedEntry(
            reinterpret_cast<std::uint8_t*>(control), nullptr,
            reinterpret_cast<int>(frameColor));
    }

    CGuiBuilder_ReadControlProps(self, nullptr, control, node);

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_CYCLE));
    if (property)
    {
        CSF_AT(std::uint8_t, control, 0x1F4) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;
    }

    char* linesKeyword = const_cast<char*>(g_pszKey_LINEAS);
    property = FindProperty(node, linesKeyword, linesKeyword);
    if (property)
    {
        auto* vtable = CSF_AT(void**, control, 0);
        int value = S_00692b80_ParseIntegerComponent(
            reinterpret_cast<S_00692b80*>(property), linesKeyword);
        // The original replaces only the low byte of the integer argument.
        int argument = (value & ~0xFF) | (value != 0);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            vtable[0x240 / 4])(control, linesKeyword, argument);
    }

    property = FindProperty(node, linesKeyword, const_cast<char*>(g_pszKey_AUTOSIZE));
    if (property)
    {
        auto* vtable = CSF_AT(void**, control, 0);
        int value = S_00692b80_ParseIntegerComponent(
            reinterpret_cast<S_00692b80*>(property), linesKeyword);
        std::uint32_t argument =
            (reinterpret_cast<std::uint32_t>(linesKeyword) & ~0xFFu) |
            static_cast<std::uint32_t>(value != 0);
        reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t)>(
            vtable[0x244 / 4])(control, nullptr, argument);
    }

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_ROWFILL));
    if (property)
    {
        auto* vtable = CSF_AT(void**, control, 0);
        int value = S_00692b80_ParseIntegerComponent(
            reinterpret_cast<S_00692b80*>(property), nullptr);
        std::uint32_t argument =
            (reinterpret_cast<std::uint32_t>(property) & ~0xFFu) |
            static_cast<std::uint32_t>(value != 0);
        reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t)>(
            vtable[0x248 / 4])(control, nullptr, argument);
    }

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_INTERLINEA));
    if (property)
    {
        auto* vtable = CSF_AT(void**, control, 0);
        int spacing = S_00692b80_ParseIntegerComponent(
            reinterpret_cast<S_00692b80*>(property), nullptr);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            vtable[0x24C / 4])(control, nullptr, spacing);
    }

    auto* header = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_CABECERA));
    if (header)
    {
        auto* textNode = FindProperty(header, nullptr, const_cast<char*>(g_pszKey_TEXTO));
        if (textNode)
        {
            void* text = CGuiBuilder_CreateText(self, nullptr, textNode);
            if (text)
            {
                reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, control, 0)[0x250 / 4])(control, nullptr, text);
            }
            CGuiBuilder_InitializeDefaultColor(temporaryColor);
            auto* background = FindProperty(
                header, nullptr, const_cast<char*>(g_pszKey_COLOR_FONDO));
            if (CGuiBuilder_ReadFourComponents(background, temporaryColor))
                GetOrCreateCachedObjectAndTailDispatch(control, nullptr, temporaryColor);
        }
    }

    auto* columns = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_COLUMNAS));
    if (columns)
    {
        int font = CGuiBuilder_GetFont(self, nullptr, columns);
        if (font)
        {
            auto* columnList = reinterpret_cast<S_00692b80*>(
                FindProperty(columns, nullptr, const_cast<char*>(g_pszKey_COLUMNAS)));
            int childCount = S_00692b80_GetChildCount(columnList, nullptr);
            for (int columnIndex = 0; columnIndex < childCount; ++columnIndex)
            {
                auto* component = S_00692b80_GetComponent(
                    columnList, nullptr, columnIndex);
                auto* componentNode = reinterpret_cast<std::uint8_t*>(component);
                int width = ReadIntegerProperty(
                    componentNode, nullptr, const_cast<char*>(g_pszKey_WIDTH_gui_builder));
                if (width > 0)
                {
                    CGuiListBoxBase_SetColumnWidth(
                        reinterpret_cast<CGuiListBoxBase*>(control),
                        nullptr, columnIndex, width);
                }

                auto* textNode = FindProperty(
                    componentNode, nullptr, const_cast<char*>(g_pszKey_TEXTO));
                if (textNode)
                {
                    void* text = CGuiBuilder_CreateText(self, nullptr, textNode);
                    if (text)
                    {
                        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                            CSF_AT(void**, text, 0)[0x24 / 4])(text, nullptr, font);
                        reinterpret_cast<void (__fastcall*)(void*, void*, int, void*)>(
                            CSF_AT(void**, control, 0)[0x25C / 4])(
                                control, nullptr, columnIndex, text);
                    }
                }

                CGuiBuilder_InitializeDefaultColor(temporaryColor);
                auto* background = FindProperty(
                    componentNode, nullptr, const_cast<char*>(g_pszKey_COLOR_FONDO));
                if (CGuiBuilder_ReadFourComponents(background, temporaryColor))
                {
                    InvokeLazyIndexedResource(
                        control, nullptr, columnIndex,
                        reinterpret_cast<int>(temporaryColor));
                }
            }
            ReleaseObjectReference(reinterpret_cast<void*>(font), nullptr);
        }
    }

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_COLOR));
    if (CGuiBuilder_ReadFourComponents(property, persistentColor))
    {
        CSF_AT(float, control, 0x144) = persistentColor[0];
        CSF_AT(float, control, 0x148) = persistentColor[1];
        CSF_AT(float, control, 0x14C) = persistentColor[2];
        CSF_AT(float, control, 0x150) = persistentColor[3];
    }

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_COLOR_NOFOCUS));
    if (CGuiBuilder_ReadFourComponents(property, persistentColor))
    {
        CSF_AT(float, control, 0x154) = persistentColor[0];
        CSF_AT(float, control, 0x158) = persistentColor[1];
        CSF_AT(float, control, 0x15C) = persistentColor[2];
        CSF_AT(float, control, 0x160) = persistentColor[3];
    }

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_COLOR_LINEAS));
    if (CGuiBuilder_ReadFourComponents(property, persistentColor))
        ForwardToEmbeddedObjectVtableSlot11(control, nullptr, persistentColor);
}

HOOK(0x00720420, CGuiBuilder_ReadTextControlProps, "auto");
