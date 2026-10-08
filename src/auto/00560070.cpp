#include <cstdint>
#include "hook.h"
#include "globals.h"

// ---------------------------------------------------------------------------
// External functions called by target 0x00560070 (module menu_mp_sessions)
// ---------------------------------------------------------------------------

// 0x0073C020 : int __cdecl sub_73C020()
// Returns a global dword (a singleton / manager object pointer).
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x73C020, ());

// 0x0073D310 : void __thiscall sub_73D310(std::uint32_t* this)   (ret 0)
// Modeled as __fastcall so the object pointer lands in ecx (edx unused).
CSF_FUNC(void, __fastcall, GlobalObject_ResetState, 0x73D310, (void* self, void* /*edx*/));

// 0x0073C7F0 : char __stdcall sub_73C7F0(char a1)   (callee pops 4 bytes)
CSF_FUNC(char, __stdcall, GlobalObject_ApplyMode, 0x73C7F0, (char a1));

// ---------------------------------------------------------------------------
// Data referenced by the target
// ---------------------------------------------------------------------------

// 0x008B0903 : single module flag byte, cleared at the end.
CSF_GLOBAL(uint8_t, byte_8B0903, 0x8B0903);

// ---------------------------------------------------------------------------
// 0x00560070 : void __cdecl StartAddress()   (size 34, `ret` -> cdecl)
// ---------------------------------------------------------------------------
static void __cdecl MenuMpSessions_StartAddress()
{
    // Reset the current global manager object (ecx = returned pointer, edx unused).
    GlobalObject_ResetState(reinterpret_cast<void*>(GetGlobalDword_73C020()), nullptr);

    // Re-fetch the global object (result discarded by the __stdcall callee),
    // apply mode 1, then clear the module flag byte.
    GetGlobalDword_73C020();
    GlobalObject_ApplyMode(1);
    byte_8B0903 = 0;
}

HOOK(0x560070, MenuMpSessions_StartAddress, "auto");
