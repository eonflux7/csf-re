#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00483360 (__thiscall, ret 0) — BOOL __thiscall sub_483360(_DWORD *this)
CSF_FUNC(int, __fastcall, IsFieldAt10Equal36, 0x483360, (void* self, void* edx));

// 0x00495550: reads the object pointer stored at self+0x44 and tail-jumps into
// sub_483360 with it as `this`, forwarding its BOOL result to the caller.
static int __fastcall C_495550_CheckInnerObject(void* self, void* /*edx*/)
{
    void* innerObject = CSF_AT(void*, self, 0x44);
    return IsFieldAt10Equal36(innerObject, nullptr);
}

HOOK(0x495550, C_495550_CheckInnerObject, "auto");
