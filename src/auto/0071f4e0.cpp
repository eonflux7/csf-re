#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0,
         (std::size_t size));
CSF_FUNC(void*, __fastcall, CGuiSlider_Construct, 0x0072D8D0,
         (void* self, void* edx, std::uint32_t owner, void* member));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0,
         (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0,
         (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, CGuiBuilder_ReadControlProps, 0x0071EBD0,
         (CGuiBuilder* self, void* edx, void* control, std::uint8_t* properties));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41,
         (const char* left, const char* right));

CSF_GLOBAL(char*, g_SliderOrientationKeyword, 0x008A86A0);
CSF_GLOBAL(char*, g_SliderFirstControlKeyword, 0x008A86A4);
CSF_GLOBAL(char*, g_SliderSecondControlKeyword, 0x008A86A8);
CSF_GLOBAL(char*, g_SliderThirdControlKeyword, 0x008A86AC);
CSF_GLOBAL(char*, g_SliderConsoleKeyword, 0x008A873C);

static int __fastcall CGuiBuilder_CreateSlider(
    CGuiBuilder* self, void* /*edx*/, std::uint8_t* properties)
{
    char orientation[256] = {};

    void* slider = Mem_Alloc(0x16C);
    if (slider)
    {
        slider = CGuiSlider_Construct(
            slider, nullptr, self->field_0004,
            reinterpret_cast<std::uint8_t*>(self) + 0x0C);
    }

    CGuiBuilder_ReadControlProps(self, nullptr, slider, properties);

    if (FindProperty(properties, nullptr, g_SliderOrientationKeyword))
    {
        MenuExtras_ReadText(
            properties, nullptr, g_SliderOrientationKeyword, orientation, 256);

        if (__stricmp(
                orientation, CSF_STR(0x00864B8C, "HORIZONTAL")) == 0)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, slider, 0)[0x258 / 4])(
                    slider, nullptr, 1);
        }
    }

    std::uint8_t* property =
        FindProperty(properties, nullptr, g_SliderFirstControlKeyword);
    if (property)
    {
        void** builderVtable = CSF_AT(void**, self, 0);
        void** sliderVtable = CSF_AT(void**, slider, 0);
        int control =
            reinterpret_cast<int(__fastcall*)(CGuiBuilder*, void*, std::uint8_t*)>(
                builderVtable[0x20 / 4])(self, nullptr, property);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            sliderVtable[0x240 / 4])(slider, nullptr, control);
    }

    property =
        FindProperty(properties, nullptr, g_SliderSecondControlKeyword);
    if (property)
    {
        void** builderVtable = CSF_AT(void**, self, 0);
        void** sliderVtable = CSF_AT(void**, slider, 0);
        int control =
            reinterpret_cast<int(__fastcall*)(CGuiBuilder*, void*, std::uint8_t*)>(
                builderVtable[0x20 / 4])(self, nullptr, property);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            sliderVtable[0x248 / 4])(slider, nullptr, control);
    }

    property =
        FindProperty(properties, nullptr, g_SliderThirdControlKeyword);
    if (property)
    {
        void** sliderVtable = CSF_AT(void**, slider, 0);
        void** builderVtable = CSF_AT(void**, self, 0);
        int control =
            reinterpret_cast<int(__fastcall*)(CGuiBuilder*, void*, std::uint8_t*)>(
                builderVtable[0x20 / 4])(self, nullptr, property);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            sliderVtable[0x250 / 4])(slider, nullptr, control);
    }

    if (FindProperty(properties, nullptr, g_SliderConsoleKeyword))
    {
        char* keyword = g_SliderConsoleKeyword;
        char consoleText[256] = {};
        MenuExtras_ReadText(
            properties, nullptr, keyword, consoleText, 256);

        const auto* consoleConstant = reinterpret_cast<const std::uint8_t*>(
            CSF_STR(0x00864B84, "CONSOLE"));

        // The original compares eight bytes, then replaces only AL with SETE.
        // A negative comparison therefore passes 0xFFFFFF00, not zero.
        int consoleArgument = 1;
        for (int index = 0; index < 8; ++index)
        {
            const std::uint8_t expected = consoleConstant[index];
            const std::uint8_t actual =
                static_cast<std::uint8_t>(consoleText[index]);
            if (expected != actual)
            {
                consoleArgument = expected < actual ? -256 : 0;
                break;
            }
        }

        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, slider, 0)[0x27C / 4])(
                slider, nullptr, consoleArgument);
    }

    return reinterpret_cast<int>(slider);
}

HOOK(0x0071F4E0, CGuiBuilder_CreateSlider, "auto");
