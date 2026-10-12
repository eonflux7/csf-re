#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_pszMenu_P_Juego, 0x00899AF0);
CSF_GLOBAL(const char*, g_pszMenu_P_MsgBox, 0x00899B28);
CSF_GLOBAL(const char*, g_pszMenu_P_Tactic, 0x00899B14);
CSF_GLOBAL(const char*, g_pszMenu_P_EstaMP, 0x00899B18);

CSF_FUNC(unsigned int, __fastcall, AppendFourByteElement_50CDF0, 0x0050CDF0, (void* self, void* , std::uint32_t* source));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static void* __fastcall S_00537cd0_SubmitGamePanels(
    S_00537cd0* self, void* /*edx*/)
{
    void* interfaceObject = reinterpret_cast<void*>(self->field_0018);
    std::uint32_t gamePanel =
        ((int(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, interfaceObject, 0)[0x5C / 4])(
                interfaceObject, nullptr, g_pszMenu_P_Juego);

    interfaceObject = reinterpret_cast<void*>(self->field_0018);
    std::uint32_t messagePanel =
        ((int(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, interfaceObject, 0)[0x5C / 4])(
                interfaceObject, nullptr, g_pszMenu_P_MsgBox);

    interfaceObject = reinterpret_cast<void*>(self->field_0018);
    std::uint32_t tacticsPanel =
        ((int(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, interfaceObject, 0)[0x5C / 4])(
                interfaceObject, nullptr, g_pszMenu_P_Tactic);

    interfaceObject = reinterpret_cast<void*>(self->field_0018);
    std::uint32_t multiplayerStatusPanel =
        ((int(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, interfaceObject, 0)[0x5C / 4])(
                interfaceObject, nullptr, g_pszMenu_P_EstaMP);

    alignas(8) std::uint8_t panelListBytes[0x10];
    S_0050c9f0& panelList =
        *reinterpret_cast<S_0050c9f0*>(panelListBytes);
    std::memset(&panelList.field_0004, 0, 12);

    AppendFourByteElement_50CDF0(&panelList, nullptr, &gamePanel);
    AppendFourByteElement_50CDF0(&panelList, nullptr, &messagePanel);
    AppendFourByteElement_50CDF0(&panelList, nullptr, &tacticsPanel);
    AppendFourByteElement_50CDF0(&panelList, nullptr, &multiplayerStatusPanel);

    interfaceObject = reinterpret_cast<void*>(self->field_0018);
    ((void(__fastcall*)(void*, void*, S_0050c9f0*, int))
        CSF_AT(void**, interfaceObject, 0)[0x64 / 4])(
            interfaceObject, nullptr, &panelList, 1);

    void* storage = reinterpret_cast<void*>(panelList.field_0004);
    if (storage)
        return reinterpret_cast<void*>(Mem_FreeDebug(storage));
    return storage;
}

HOOK(0x00537CD0, S_00537cd0_SubmitGamePanels, "auto");
