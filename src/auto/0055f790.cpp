#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static __declspec(naked) int __fastcall MenuMpOnline_ForwardMatchTransition(
    void* /*self*/, void* /*onlineFlag*/)
{
    __asm
    {
        mov eax, edx
        test eax, eax
        je Offline
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x4c]
    Offline:
        mov edx, dword ptr [ecx]
        mov dword ptr [ecx + 0x18], 2
        jmp dword ptr [edx + 0x4c]
    }
}

static int __fastcall MenuMpOnline_TransitionForNetworkMode(
    void* self, void* /*edx*/)
{
    int online;
    if (GetGlobal8B0688())
    {
        GetGlobal8B0688();
        online = g_OnlineMatchFlag;
    }
    else
    {
        online = g_MpNetworkType == 3;
    }

    [[clang::musttail]]
    return MenuMpOnline_ForwardMatchTransition(
        self, reinterpret_cast<void*>(static_cast<std::uintptr_t>(online)));
}

HOOK(0x0055F790, MenuMpOnline_TransitionForNetworkMode, "auto");
