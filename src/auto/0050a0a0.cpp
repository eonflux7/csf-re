#include <cstdint>
#include "globals.h"
#include "hook.h"

// Calls the object's virtual slot at +0x20 (index 8), then stores the
// argument at +0x04 and returns the slot's result.
static int __fastcall GameBddLoader_AttachMenuManager(void* self, void* /*edx*/, int a2)
{
    int result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x20 / 4])(self, nullptr);
    CSF_AT(int, self, 0x4) = a2;
    return result;
}

HOOK(0x0050A0A0, GameBddLoader_AttachMenuManager, "auto");
