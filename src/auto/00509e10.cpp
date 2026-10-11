#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_pszMenu_P_LbSlCu, 0x00899B30);
CSF_GLOBAL(const char*, g_pszMenu_P_LbCuLa, 0x00899B34);
CSF_GLOBAL(const char*, g_pszMenu_P_LbCuCr, 0x00899B38);
CSF_GLOBAL(const char*, g_pszMenu_P_LbCuMo, 0x00899B3C);
CSF_GLOBAL(const char*, g_pszMenu_P_LbMePr, 0x00899B40);
CSF_GLOBAL(const char*, g_pszMenu_P_LbQMch, 0x00899B48);
CSF_GLOBAL(const char*, g_pszMenu_P_LbTpQM, 0x00899B4C);
CSF_GLOBAL(const char*, g_pszMenu_P_LbCMch, 0x00899B50);
CSF_GLOBAL(const char*, g_pszMenu_P_LbOMch, 0x00899B54);
CSF_GLOBAL(const char*, g_pszMenu_P_LbLaWa, 0x00899B74);

static int AppMain_SetMenuLabel(void* menu, C_008407a8* label, const char* text)
{
    return reinterpret_cast<int(__fastcall*)(void*, void*, C_008407a8*, const char*)>(
        CSF_AT(void**, menu, 0)[0x2C / 4])(menu, nullptr, label, text);
}

static int __fastcall C_00840754_UpdateMenuLabels(C_00840754* self, void* /*edx*/)
{
    AppMain_SetMenuLabel(
        reinterpret_cast<void*>(self->field_0004),
        reinterpret_cast<C_008407a8*>(self->field_0008),
        g_pszMenu_P_LbSlCu);
    AppMain_SetMenuLabel(
        reinterpret_cast<void*>(self->field_0004),
        reinterpret_cast<C_008407a8*>(self->field_000c),
        g_pszMenu_P_LbCuLa);
    AppMain_SetMenuLabel(
        reinterpret_cast<void*>(self->field_0004),
        reinterpret_cast<C_008407a8*>(self->field_0010),
        g_pszMenu_P_LbCuCr);
    AppMain_SetMenuLabel(
        reinterpret_cast<void*>(self->field_0004),
        reinterpret_cast<C_008407a8*>(self->field_0014),
        g_pszMenu_P_LbCuMo);
    AppMain_SetMenuLabel(
        reinterpret_cast<void*>(self->field_0004),
        reinterpret_cast<C_008407a8*>(self->field_0018),
        g_pszMenu_P_LbMePr);
    AppMain_SetMenuLabel(
        reinterpret_cast<void*>(self->field_0004),
        reinterpret_cast<C_008407a8*>(self->field_001c),
        g_pszMenu_P_LbCMch);
    AppMain_SetMenuLabel(
        reinterpret_cast<void*>(self->field_0004),
        reinterpret_cast<C_008407a8*>(self->field_0020),
        g_pszMenu_P_LbQMch);
    AppMain_SetMenuLabel(
        reinterpret_cast<void*>(self->field_0004),
        reinterpret_cast<C_008407a8*>(self->field_003c),
        g_pszMenu_P_LbTpQM);
    AppMain_SetMenuLabel(
        reinterpret_cast<void*>(self->field_0004),
        reinterpret_cast<C_008407a8*>(self->field_0024),
        g_pszMenu_P_LbOMch);
    return AppMain_SetMenuLabel(
        reinterpret_cast<void*>(self->field_0004),
        reinterpret_cast<C_008407a8*>(self->field_0044),
        g_pszMenu_P_LbLaWa);
}

HOOK(0x00509E10, C_00840754_UpdateMenuLabels, "auto");
