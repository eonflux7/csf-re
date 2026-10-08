#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

static CSF_FUNC(std::uint32_t*, __fastcall, MenuExtras_InitializeFile, 0x00692B10,
                (std::uint32_t* self, void* edx));
static CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770,
                (void* self, void* edx, int source));
static CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380,
                (void* self, void* edx));
static CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0,
                (std::uint8_t* self, void* edx, char* name));
static CSF_FUNC(void, __fastcall, LoadGuiScalarProperties, 0x0071D890,
                (void* self, void* edx, std::uint8_t* properties));
static CSF_FUNC(void, __fastcall, GuiBuilder_LoadScalarProperties, 0x0071D980,
                (void* self, void* edx, std::uint8_t* properties));
static CSF_FUNC(void, __fastcall, GuiBuilder_LoadColorProperties, 0x0071E130,
                (void* self, void* edx, std::uint8_t* properties));
static CSF_FUNC(int, __fastcall, CGuiBuilder_CreateTextureQuad, 0x0071E4E0,
                (void* self, void* edx, std::uint8_t* properties));

static CSF_GLOBAL(char*, g_ScalarSectionName, 0x008A8754);
static CSF_GLOBAL(char*, g_SecondarySectionName, 0x008A8770);
static CSF_GLOBAL(char*, g_AdditionalSectionName, 0x008A877C);
static CSF_GLOBAL(char*, g_TextureSectionName, 0x008A8668);

static int __fastcall LoadGuiBuilderFromResource(
    void* self, void* /*edx*/, int source)
{
    std::uint32_t document[15];
    MenuExtras_InitializeFile(document, nullptr);
    MenuExtras_LoadFile(document, nullptr, source);

    std::uint8_t* root =
        reinterpret_cast<std::uint8_t*>(document[0]);

    if (root)
    {
        std::uint8_t* section =
            FindProperty(root, nullptr, g_ScalarSectionName);
        LoadGuiScalarProperties(self, nullptr, section);

        section = FindProperty(root, nullptr, g_SecondarySectionName);
        GuiBuilder_LoadScalarProperties(self, nullptr, section);

        section = FindProperty(root, nullptr, g_AdditionalSectionName);
        GuiBuilder_LoadColorProperties(self, nullptr, section);

        section = FindProperty(root, nullptr, g_TextureSectionName);
        if (section)
        {
            int textureQuad;
            if (CSF_AT(std::uint8_t, self, 0x08))
                textureQuad = CGuiBuilder_CreateTextureQuad(self, nullptr, section);
            else
                textureQuad = 0;

            CSF_AT(int, self, 0x84) = textureQuad;
        }
    }

    return MenuExtras_CloseFile(document, nullptr);
}

HOOK(0x0071F6E0, LoadGuiBuilderFromResource, "auto");
