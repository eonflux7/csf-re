#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, (unsigned int fa, unsigned int f8, unsigned int f9));
CSF_FUNC(unsigned int, __fastcall, sub_67D110, 0x0067D110, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, CreateGatheredNetCogibleDdl, 0x00742C80, (int a2));
CSF_FUNC(int, __cdecl, CreateGatheredNetPlayerDdl, 0x00745AF0, (int classId));
CSF_FUNC(int, __cdecl, sub_747BF0, 0x00747BF0, (int a2));
CSF_FUNC(int, __cdecl, sub_74B9D0, 0x0074B9D0, (int a2));
CSF_FUNC(int, __cdecl, NetGameDdl_CreateGatheredRecord, 0x0074D140, (int a2));
CSF_FUNC(int, __cdecl, sub_74EC60, 0x0074EC60, (int a2));
CSF_FUNC(int, __cdecl, sub_74F5E0, 0x0074F5E0, (int a2));
CSF_FUNC(int, __cdecl, sub_74FEF0, 0x0074FEF0, (int a2));
CSF_FUNC(int, __cdecl, sub_751020, 0x00751020, (int a2));

CSF_GLOBAL(std::uint32_t, g_NextNetDdlClassId, 0x008A65E4);
CSF_GLOBAL(std::uint8_t, g_NetDdlOptionF8, 0x008A65F8);
CSF_GLOBAL(std::uint8_t, g_NetDdlOptionF9, 0x008A65F9);
CSF_GLOBAL(std::uint8_t, g_NetDdlOptionFA, 0x008A65FA);
CSF_GLOBAL(std::uint32_t, g_NetGameClassId, 0x008BC178);
CSF_GLOBAL(std::uint32_t, g_NetControlClassId, 0x008BC17C);
CSF_GLOBAL(std::uint32_t, g_DdlObjectClassId, 0x008BC1CC);
CSF_GLOBAL(std::uint32_t, g_RegisteredNetClassId, 0x008BC27C);

static int __cdecl NetChatDdl_RegisterGatheredRecords()
{
    // 00750AD2/AD9/AE1 zero-extend F9/F8/FA; pushes are F9, F8, FA.
    const unsigned int f9 = g_NetDdlOptionF9;
    const unsigned int f8 = g_NetDdlOptionF8;
    const unsigned int fa = g_NetDdlOptionFA;
    SceneLoader_NoOp(fa, f8, f9);

    sub_751020(static_cast<int>(g_NextNetDdlClassId + 10));
    CreateGatheredNetCogibleDdl(static_cast<int>(g_NextNetDdlClassId + 7));
    sub_74EC60(static_cast<int>(g_NextNetDdlClassId + 12));
    NetGameDdl_CreateGatheredRecord(static_cast<int>(g_NextNetDdlClassId + 11));
    sub_74B9D0(static_cast<int>(g_NextNetDdlClassId + 13));
    CreateGatheredNetPlayerDdl(static_cast<int>(g_NextNetDdlClassId + 8));
    sub_74F5E0(static_cast<int>(g_NextNetDdlClassId + 14));
    sub_74FEF0(static_cast<int>(g_NextNetDdlClassId + 9));
    sub_747BF0(static_cast<int>(g_NextNetDdlClassId + 6));

    const std::uint32_t nextClassId = g_NextNetDdlClassId + 15;
    const int registeredClassId = static_cast<int>(g_RegisteredNetClassId);
    g_NextNetDdlClassId = nextClassId;

    sub_67D110(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008BC188)),
              nullptr, registeredClassId);
    sub_67D110(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008BC194)),
              nullptr, static_cast<int>(g_DdlObjectClassId));
    sub_67D110(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008BC1A0)),
              nullptr, static_cast<int>(g_NetGameClassId));
    return static_cast<int>(
        sub_67D110(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008BC1AC)),
                  nullptr, static_cast<int>(g_NetControlClassId)));
}

HOOK(0x00750AD0, NetChatDdl_RegisterGatheredRecords, "auto");
