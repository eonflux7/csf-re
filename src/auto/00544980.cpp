#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original callee: cdecl, one stack argument, result tested via al.
CSF_FUNC(int, __cdecl, Sub_7A8ED0, 0x007A8ED0, (int condition));

// Global: pointer value stored at 0x899AF4, passed to the menu object's string setter.
CSF_GLOBAL(const char*, g_pszMenu_P_MeJueg, 0x00899AF4);

// CMenuSaveGame::ProcessPendingState (__thiscall, 2 stack args, ret 8).
// Field offsets are relative to the sub-object at self-4 (the original's "v5").
static int __fastcall MenuSaveGame_ProcessPendingState(void* self, void* /*edx*/, int conditionArg, int /*unusedArg*/)
{
    int result = Sub_7A8ED0(conditionArg);
    if (static_cast<uint8_t>(result) != 0)
    {
        result = CSF_AT(int32_t, self, 0x4C);
        // v5 = this - 1 (dword), i.e. this - 4 bytes
        void* v5 = static_cast<uint8_t*>(self) - 4;
        if (result == -1)
        {
            void* obj = CSF_AT(void*, v5, 0x40);
            CSF_AT(int32_t, v5, 0x50) = 6;
            // vtbl slot 0x60 / 4 = 24 : (obj, v5, 0)
            void** vt = CSF_AT(void**, obj, 0);
            ((void(__fastcall*)(void*, void*, void*, int))vt[24])(obj, nullptr, v5, 0);

            obj = CSF_AT(void*, v5, 0x40);
            // vtbl slot 0x58 / 4 = 22 : (obj, g_pszMenu_P_MeJueg)
            vt = CSF_AT(void**, obj, 0);
            return ((int(__fastcall*)(void*, void*, const char*))vt[22])(obj, nullptr, g_pszMenu_P_MeJueg);
        }
    }
    return result;
}

HOOK(0x00544980, MenuSaveGame_ProcessPendingState, "auto");
