#include <cstddef>
#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(char*, g_ListBoxTextureKeyword, 0x008A8648);

CSF_FUNC(int, __cdecl, Mem_Alloc, 0x006910F0,
         (std::size_t size));
CSF_FUNC(int, __fastcall, CGuiTextureListBox_Construct, 0x007313C0,
         (void* self, void* edx, std::uint32_t manager, void* builderMember));
CSF_FUNC(void, __fastcall, ReadTextControlProps, 0x00720420,
         (CGuiBuilder* self, void* edx, std::uint8_t* properties, int control));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0,
         (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, SetListBoxTexture, 0x00714050,
         (void* self, void* edx, int texture));

static int __fastcall CGuiBuilder_CreateTextureListBox(
    CGuiBuilder* self, void* /*edx*/, std::uint8_t* properties)
{
    int storage = Mem_Alloc(0x20C);
    int listBox = 0;
    if (storage)
    {
        listBox = CGuiTextureListBox_Construct(
            reinterpret_cast<void*>(storage), nullptr, self->field_0004,
            reinterpret_cast<std::uint8_t*>(self) + 0x0C);
    }

    void* control = reinterpret_cast<void*>(listBox);
    std::uint32_t setting = self->field_0020;
    reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(
        CSF_AT(void**, control, 0)[0x170 / 4])(control, nullptr, setting);

    ReadTextControlProps(self, nullptr, properties, listBox);

    std::uint8_t* textureProperties =
        FindProperty(properties, nullptr, g_ListBoxTextureKeyword);
    if (textureProperties)
    {
        int texture =
            reinterpret_cast<int(__fastcall*)(CGuiBuilder*, void*, std::uint8_t*)>(
                CSF_AT(void**, self, 0)[0x34 / 4])(
                    self, nullptr, textureProperties);
        SetListBoxTexture(control, nullptr, texture);
    }

    return listBox;
}

HOOK(0x007208F0, CGuiBuilder_CreateTextureListBox, "auto");
