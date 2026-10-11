#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original 0x0050E1C0: reads the active-object id via the owner at +0x14,
// then tail-jumps into the returned object's vtable slot 0x20.
CSF_GLOBAL(const char*, g_pszMenu_P_MsgBox, 0x899B28);

static hook::Tail __fastcall GlobalStateSlot_ReadActiveId(void* self, const uint32_t* /*args*/) {
    // mov ecx, [ecx + 0x14]: owner pointer stored at +0x14
    void* owner = CSF_AT(void*, self, 0x14);

    // Virtual call at owner's vtable slot 0x5c / 4, thiscall with one argument (the string)
    void* activeObj = ((void* (__fastcall*)(void*, void*, const char*))(CSF_AT(void**, owner, 0)[0x5c / 4]))(
        owner, nullptr, g_pszMenu_P_MsgBox);

    if (activeObj != nullptr) {
        // mov ecx, eax; jmp [vtable + 0x20]
        return hook::tail_jump(activeObj, (*reinterpret_cast<void***>(activeObj))[0x20 / 4]);
    }
    // xor eax, eax; ret
    return hook::tail_ret(0);
}

HOOK_TAIL(0x0050E1C0, GlobalStateSlot_ReadActiveId, "game_bdd_loader");
