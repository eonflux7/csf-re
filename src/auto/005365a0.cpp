#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(void, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0, (void* self, void* edx, int value));

// Resolves a menu entry for key 0x4D503031, forwards it to the child stored at +0x138,
// then tail-jumps to the object's vtable slot 0xAC with the caller's stack arguments intact.
static hook::Tail __fastcall InstancingSceneForwardThenTailJump(void* self, const uint32_t* /*args*/)
{
    int resolvedEntry = MenuMessageBox_ResolveEntryByKey(0x4D503031, 2);

    void* childObject = CSF_AT(void*, self, 0x138);
    C_00720DF0_ForwardCallToChildVtblSlot16(childObject, nullptr, resolvedEntry);

    return hook::tail_jump(self, (*reinterpret_cast<void***>(self))[0xAC / 4]);
}

HOOK_TAIL(0x005365A0, InstancingSceneForwardThenTailJump, "interface");
