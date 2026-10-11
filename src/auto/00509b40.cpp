#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, C_008407a8_AppendNamedEntryNoAlternate, 0x0050DE30, (C_008407a8* self, void* , int a1, int a2));

CSF_GLOBAL(const char*, g_pszMenu_P_MeJuMP, 0x00899AF8);
CSF_GLOBAL(const char*, g_pszMenu_P_CharMP, 0x00899AFC);
CSF_GLOBAL(const char*, g_pszMenu_P_EstaMP, 0x00899B18);
CSF_GLOBAL(const char*, g_pszMenu_P_VictMP, 0x00899B1C);
CSF_GLOBAL(const char*, g_pszMenu_P_TmPart, 0x00899B20);
CSF_GLOBAL(const char*, g_pszMenu_P_LbSlCu, 0x00899B30);
CSF_GLOBAL(const char*, g_pszMenu_P_LbCuLa, 0x00899B34);
CSF_GLOBAL(const char*, g_pszMenu_P_LbCuCr, 0x00899B38);
CSF_GLOBAL(const char*, g_pszMenu_P_LbCuMo, 0x00899B3C);
CSF_GLOBAL(const char*, g_pszMenu_P_LbMePr, 0x00899B40);
CSF_GLOBAL(const char*, g_pszMenu_P_LbQMch, 0x00899B48);
CSF_GLOBAL(const char*, g_pszMenu_P_LbTpQM, 0x00899B4C);
CSF_GLOBAL(const char*, g_pszMenu_P_LbCMch, 0x00899B50);
CSF_GLOBAL(const char*, g_pszMenu_P_LbOMch, 0x00899B54);
CSF_GLOBAL(const char*, g_pszMenu_P_LbFLIn, 0x00899B64);
CSF_GLOBAL(const char*, g_pszMenu_P_LbFIOp, 0x00899B68);
CSF_GLOBAL(const char*, g_pszMenu_P_LbLaWa, 0x00899B74);
CSF_GLOBAL(const char*, g_pszMenu_P_MeCaPa, 0x00899B88);
CSF_GLOBAL(const char*, g_pszMenu_P_AuUnPa, 0x00899B90);
CSF_GLOBAL(const char*, g_pszMenu_P_EEULA, 0x00899B94);

static int __fastcall C_00840754_RegisterMenuEntries(
    C_00840754* self, void* /*edx*/)
{
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_006c), reinterpret_cast<int>(g_pszMenu_P_MeJuMP));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_0070), reinterpret_cast<int>(g_pszMenu_P_CharMP));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_0064), reinterpret_cast<int>(g_pszMenu_P_MeCaPa));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_0068), reinterpret_cast<int>(g_pszMenu_P_AuUnPa));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_0008), reinterpret_cast<int>(g_pszMenu_P_LbSlCu));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_000c), reinterpret_cast<int>(g_pszMenu_P_LbCuLa));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_0010), reinterpret_cast<int>(g_pszMenu_P_LbCuCr));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_0014), reinterpret_cast<int>(g_pszMenu_P_LbCuMo));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_0018), reinterpret_cast<int>(g_pszMenu_P_LbMePr));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_001c), reinterpret_cast<int>(g_pszMenu_P_LbCMch));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_0020), reinterpret_cast<int>(g_pszMenu_P_LbQMch));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_003c), reinterpret_cast<int>(g_pszMenu_P_LbTpQM));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_0024), reinterpret_cast<int>(g_pszMenu_P_LbOMch));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_0044), reinterpret_cast<int>(g_pszMenu_P_LbLaWa));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_004c), reinterpret_cast<int>(g_pszMenu_P_EstaMP));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_0050), reinterpret_cast<int>(g_pszMenu_P_VictMP));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_0048), reinterpret_cast<int>(g_pszMenu_P_TmPart));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_0030), reinterpret_cast<int>(g_pszMenu_P_LbFLIn));
    C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_0034), reinterpret_cast<int>(g_pszMenu_P_LbFIOp));
    return static_cast<int>(C_008407a8_AppendNamedEntryNoAlternate(
        reinterpret_cast<C_008407a8*>(self->field_0004), nullptr,
        static_cast<int>(self->field_0074), reinterpret_cast<int>(g_pszMenu_P_EEULA)));
}

HOOK(0x00509B40, C_00840754_RegisterMenuEntries, "auto");
