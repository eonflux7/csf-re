#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x00555790 - runs the character-select finalize/update step and returns its result.
CSF_FUNC(int, __cdecl, MenuMpCharacterSelect_Finalize, 0x555790, ());

// 0x008B0684 - pointer to the currently live multiplayer character-select object
// (holds nullptr once it has been released).
CSF_GLOBAL(void*, g_MpCharacterSelect, 0x8B0684);

// 0x00542720
// Runs the finalize step, then, if the character-select object is still alive,
// invokes its virtual slot 0 with a "delete" flag of 1 and clears the global pointer.
// The original tail-jumps (0x0054ECC0) into this cleanup logic, so eax from
// MenuMpCharacterSelect_Finalize is the returned value when the global is null.
static int __cdecl MenuMpCharacterSelect_Exit()
{
    int result = MenuMpCharacterSelect_Finalize();

    void* obj = g_MpCharacterSelect;
    if (obj != nullptr)
    {
        typedef int (__fastcall *VtblSlot0)(void* self, void* edx, int flag);
        result = ((VtblSlot0)(*(void***)obj)[0])(obj, nullptr, 1);
        g_MpCharacterSelect = nullptr;
    }

    return result;
}

HOOK(0x542720, MenuMpCharacterSelect_Exit, "auto");
