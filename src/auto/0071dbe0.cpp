#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_00692b80_GetChildCount, 0x00692B80, (S_00692b80* self, void*));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

CSF_GLOBAL(const char*, g_pszKey_TIPO, 0x008A86B4);

static int __fastcall CGuiBuilder_ParseControls(
    CGuiBuilder* self, void* /*edx*/, int parent, S_00692b80* definition)
{
    alignas(8) std::uint8_t typeTextBytes[0x104];
    char* typeText = reinterpret_cast<char*>(typeTextBytes);
    std::memset(typeText, 0, 256);

    // The packet labels this address as a vtable; the assembly calls it instead.
    (void)CSF_VTBL(0x00692B80);

    if (!definition)
        return 0;

    const int childCount = S_00692b80_GetChildCount(definition, nullptr);
    for (unsigned int index = 0; static_cast<int>(index) < childCount; ++index)
    {
        S_00692b80* component =
            S_00692b80_GetComponent(definition, nullptr, index);
        auto* componentBytes = reinterpret_cast<std::uint8_t*>(component);

        if (!FindProperty(componentBytes, nullptr, const_cast<char*>(g_pszKey_TIPO)))
            continue;

        MenuExtras_ReadText(componentBytes, nullptr,
                            const_cast<char*>(g_pszKey_TIPO), typeText, 256);

        void* control;
        if (!__stricmp(typeText, CSF_STR(0x00864B28, "BOTON")))
        {
            control = reinterpret_cast<void* (__fastcall*)(CGuiBuilder*, void*, S_00692b80*)>(
                self->vtbl[0x20 / 4])(self, nullptr, component);
        }
        else if (!__stricmp(typeText, CSF_STR(0x00864B1C, "BOTON_RADIO")))
        {
            control = reinterpret_cast<void* (__fastcall*)(CGuiBuilder*, void*, S_00692b80*)>(
                self->vtbl[0x24 / 4])(self, nullptr, component);
        }
        else if (!__stricmp(typeText, CSF_STR(0x00864B0C, "BOTON_CHOICE")))
        {
            control = reinterpret_cast<void* (__fastcall*)(CGuiBuilder*, void*, S_00692b80*)>(
                self->vtbl[0x28 / 4])(self, nullptr, component);
        }
        else if (!__stricmp(typeText, CSF_STR(0x00864AF8, "BOTON_CHOICE_NUM")))
        {
            control = reinterpret_cast<void* (__fastcall*)(CGuiBuilder*, void*, S_00692b80*)>(
                self->vtbl[0x2C / 4])(self, nullptr, component);
        }
        else if (!__stricmp(typeText, CSF_STR(0x00864AEC, "ESTATICO")))
        {
            control = reinterpret_cast<void* (__fastcall*)(CGuiBuilder*, void*, S_00692b80*)>(
                self->vtbl[0x30 / 4])(self, nullptr, component);
        }
        else if (!__stricmp(typeText, CSF_STR(0x00864AE4, "PANEL")))
        {
            control = reinterpret_cast<void* (__fastcall*)(CGuiBuilder*, void*, S_00692b80*)>(
                self->vtbl[0x18 / 4])(self, nullptr, component);
        }
        else if (!__stricmp(typeText, CSF_STR(0x00864AD8, "GROUP_RADIO")))
        {
            control = reinterpret_cast<void* (__fastcall*)(CGuiBuilder*, void*, S_00692b80*)>(
                self->vtbl[0x1C / 4])(self, nullptr, component);
        }
        else if (!__stricmp(typeText, CSF_STR(0x00864AD0, "SLIDER")))
        {
            control = reinterpret_cast<void* (__fastcall*)(CGuiBuilder*, void*, std::uint8_t*)>(
                self->vtbl[0x34 / 4])(self, nullptr, componentBytes);
        }
        else if (!__stricmp(typeText, CSF_STR(0x00864AC8, "LISTBOX")))
        {
            control = reinterpret_cast<void* (__fastcall*)(CGuiBuilder*, void*, std::uint8_t*)>(
                self->vtbl[0x38 / 4])(self, nullptr, componentBytes);
        }
        else if (!__stricmp(typeText, CSF_STR(0x00864AB4, "LISTBOX_TEXTURES")))
        {
            control = reinterpret_cast<void* (__fastcall*)(CGuiBuilder*, void*, std::uint8_t*)>(
                self->vtbl[0x3C / 4])(self, nullptr, componentBytes);
        }
        else if (!__stricmp(typeText, CSF_STR(0x00864AAC, "SWITCH")))
        {
            control = reinterpret_cast<void* (__fastcall*)(CGuiBuilder*, void*, S_00692b80*)>(
                self->vtbl[0x40 / 4])(self, nullptr, component);
        }
        else if (!__stricmp(typeText, CSF_STR(0x00864AA4, "EDIT")))
        {
            control = reinterpret_cast<void* (__fastcall*)(CGuiBuilder*, void*, S_00692b80*)>(
                self->vtbl[0x44 / 4])(self, nullptr, component);
        }
        else
        {
            continue;
        }

        if (control)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, control, 0)[0x108 / 4])(control, nullptr, parent);
        }
    }

    return childCount;
}

HOOK(0x0071DBE0, CGuiBuilder_ParseControls, "auto");
