#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, S_005080e0_FindColor, 0x00507B20, (void* self, void* edx, float* a2, char* a3));
CSF_FUNC(int, __fastcall, S_005080e0_FindFont, 0x00507C10, (void* self, void* edx, char* a2));
CSF_FUNC(char, __fastcall, S_005080e0_LoadColors, 0x00508490, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, S_005080e0_LoadFonts, 0x005085F0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, S_005080e0_FinalizeConfiguration, 0x005088C0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_005080e0_LoadText, 0x005089D0, (void* self, void* edx, int arg0));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static char __fastcall S_005080e0_LoadConfiguration(
    S_005080e0* self, void* /*edx*/, int pathAddress)
{
    alignas(8) std::uint8_t colorBytes[0x10];
    alignas(8) std::uint8_t fileBytes[0x3C];
    // The text copy spans both original adjacent character buffers.
    alignas(8) std::uint8_t textAndBufferBytes[0x3C];

    auto& file = *reinterpret_cast<S_00692b10*>(fileBytes);
    char* text = reinterpret_cast<char*>(textAndBufferBytes);
    char* buffer = reinterpret_cast<char*>(textAndBufferBytes + 0x14);
    char success = 0;

    (void)CSF_VTBL(0x00692B10);
    MenuExtras_InitializeFile(&file, nullptr);

    if (MenuExtras_LoadFile(
            &file, nullptr, reinterpret_cast<const char*>(pathAddress)))
    {
        auto* root = reinterpret_cast<std::uint8_t*>(file.field_0000);
        if (root)
        {
            double speed = ReadFloatProperty(
                root, nullptr, const_cast<char*>(CSF_STR(0x0083F7DC, ".SPEED")));
            CSF_AT(float, self, 0x0C) = static_cast<float>(speed);

            if (speed != g_fZero)
            {
                auto* colors = FindProperty(
                    root, nullptr, const_cast<char*>(CSF_STR(0x0083F7D4, ".COLORS")));
                if (colors)
                {
                    success = S_005080e0_LoadColors(self, nullptr, colors);
                    if (success)
                    {
                        auto* defaultColor = FindProperty(
                            root, nullptr,
                            const_cast<char*>(CSF_STR(0x0083F7C4, ".DEFAULT_COLOR")));
                        if (defaultColor)
                        {
                            CopyAttributeText(defaultColor, nullptr, buffer, 40);
                            float* color = S_005080e0_FindColor(
                                self, nullptr,
                                reinterpret_cast<float*>(colorBytes), buffer);
                            CSF_AT(float, self, 0x20) = color[0];
                            CSF_AT(float, self, 0x24) = color[1];
                            CSF_AT(float, self, 0x28) = color[2];
                            CSF_AT(float, self, 0x2C) = color[3];
                        }

                        auto* fonts = FindProperty(
                            root, nullptr,
                            const_cast<char*>(CSF_STR(0x0083F7BC, ".FONTS")));
                        if (fonts)
                            success = static_cast<char>(
                                S_005080e0_LoadFonts(self, nullptr, fonts));

                        if (success)
                        {
                            auto* defaultFont = FindProperty(
                                root, nullptr,
                                const_cast<char*>(CSF_STR(0x0083F7AC, ".DEFAULT_FONT")));
                            if (defaultFont)
                            {
                                CopyAttributeText(defaultFont, nullptr, buffer, 40);
                                CSF_AT(int, self, 0x1C) =
                                    S_005080e0_FindFont(self, nullptr, buffer);
                            }
                        }
                    }
                }
            }

            S_005080e0_FinalizeConfiguration(self, nullptr);

            if (success)
            {
                auto* textProperty = FindProperty(
                    root, nullptr, const_cast<char*>(CSF_STR(0x0083F7A4, ".TEXT")));
                if (textProperty)
                {
                    CopyAttributeText(textProperty, nullptr, text, 60);
                    success = S_005080e0_LoadText(
                        self, nullptr, reinterpret_cast<int>(text));
                }
            }
        }
    }

    MenuExtras_CloseFile(&file, nullptr);
    return success;
}

HOOK(0x00508AF0, S_005080e0_LoadConfiguration, "auto");
