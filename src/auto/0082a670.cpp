#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// PE .text thunk: b9 e8 02 8b 00 e9 66 6d c4 ff, followed by CC padding.
static uintptr_t const blockList = CSF_ADDR_DATA(0x008B02E8);
static uintptr_t const cleanup = CSF_ADDR_FN(0x004713E0);
__declspec(naked) static void ReleaseBlockList() {
    __asm {
        mov ecx, dword ptr [blockList]
        jmp dword ptr [cleanup]
    }
}
HOOK(0x0082A670, ReleaseBlockList, "auto");
