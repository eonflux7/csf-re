#include <cstddef>
#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Keyword string looked up in the control's property bag (same one the
// texture-listbox builder uses).
CSF_GLOBAL(char*, g_ListBoxTextureKeyword, 0x008A8648);

// Game heap allocation used for the raw control storage.
CSF_FUNC(int, __cdecl, AllocateControlStorage, 0x006910F0,
         (std::size_t size));

// Builds a CGuiListBox in the supplied storage.
CSF_FUNC(int, __fastcall, CGuiListBox_Construct, 0x00724A30,
         (void* self, void* edx, std::uint32_t manager, void* builderMember));

// Applies the .fbs property bag to a freshly created control.
CSF_FUNC(void, __fastcall, ConfigureControl, 0x007207E0,
         (CGuiBuilder* self, void* edx, std::uint8_t* properties, int control));

// Looks up a keyword-based property inside the bag (returns nullptr if absent).
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0,
         (std::uint8_t* self, void* edx, char* keyword));

// Assigns a texture handle to a listbox control.
CSF_FUNC(int, __fastcall, SetListBoxTexture, 0x00714050,
         (void* self, void* edx, int texture));

static int __fastcall CGuiBuilder_CreateListBox(
    CGuiBuilder* self, void* /*edx*/, std::uint8_t* properties)
{
    int storage = AllocateControlStorage(0x230);
    int listBox = 0;
    if (storage)
    {
        listBox = CGuiListBox_Construct(
            reinterpret_cast<void*>(storage), nullptr, self->field_0004,
            reinterpret_cast<std::uint8_t*>(self) + 0x0C);
    }

    void* control = reinterpret_cast<void*>(listBox);
    std::uint32_t setting = self->field_0020;
    reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(
        CSF_AT(void**, control, 0)[0x170 / 4])(control, nullptr, setting);

    ConfigureControl(self, nullptr, properties, listBox);

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

HOOK(0x00720970, CGuiBuilder_CreateListBox, "auto");
