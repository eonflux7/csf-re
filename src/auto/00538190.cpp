#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __fastcall, S_00537cd0_SubmitGamePanels, 0x00537CD0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MenuCharSelect_TickPreviewLoop, 0x00542560, ());
CSF_FUNC(void, __fastcall, MenuMpCharacterSelect_ReleaseServerInfoChild, 0x005426C0, (void* self, void*));
CSF_FUNC(void*, __cdecl, CoreFiles_SetFileOption, 0x00698600, (char locked));

static int __fastcall S_00537cd0_ActivateGameInterface(
    S_00537cd0* self, void* /*edx*/)
{
    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* activeObject = CSF_AT(void*, globalState, 0x74);
    if (activeObject)
    {
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, activeObject, 0)[0x0C / 4])(
                activeObject, nullptr);
    }

    void* interfaceObject = reinterpret_cast<void*>(self->field_0018);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, interfaceObject, 0)[0xAC / 4])(
            interfaceObject, nullptr, 1);

    globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (CSF_AT(std::uint8_t, globalState, 0xD0))
        MenuCharSelect_TickPreviewLoop();

    void* settings = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    if (CSF_AT(int, settings, 0x5C) == 1)
    {
        settings = reinterpret_cast<void*>(GetGlobalValue_8B0568());
        char* text;
        if (CSF_AT(std::uint32_t, settings, 0x94) < 0x10)
            text = reinterpret_cast<char*>(settings) + 0x7C + 4;
        else
            text = CSF_AT(char*, settings, 0x80);

        AssignStringRange_429B60(
            reinterpret_cast<std::uint8_t*>(self) + 0x160,
            nullptr, text, static_cast<unsigned int>(std::strlen(text)));
    }

    MenuMpCharacterSelect_ReleaseServerInfoChild(
        reinterpret_cast<void*>(self->field_014c), nullptr);

    // The packet's vtable entry identifies this direct callee address.
    (void)CSF_VTBL(0x00537CD0);
    S_00537cd0_SubmitGamePanels(self, nullptr);

    interfaceObject = reinterpret_cast<void*>(self->field_0018);
    ((void(__fastcall*)(void*, void*, S_00537cd0*, int))
        CSF_AT(void**, interfaceObject, 0)[0x60 / 4])(
            interfaceObject, nullptr, self, 1);

    return static_cast<int>(
        reinterpret_cast<std::intptr_t>(CoreFiles_SetFileOption(0)));
}

HOOK(0x00538190, S_00537cd0_ActivateGameInterface, "auto");
