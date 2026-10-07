#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee at 0x64B250 (lib region): a __thiscall method receiving the adjusted
// this-pointer in ecx and popping no stack bytes (ret 0).
CSF_FUNC(int, __fastcall, Sub_64B250, 0x64B250, (void* self, void* edx));

// 0x753420: compiler-generated thunk. It shifts ecx by 0xFFFFFEAC (-340) to
// point at an embedded sub-object / secondary base located at +340 inside the
// callee's object, then tail-jumps to 0x64B250 with that adjusted this-pointer.
static int __fastcall AdjustThisAndCall64B250(void* self, void* /*edx*/)
{
    return Sub_64B250(static_cast<uint8_t*>(self) - 340, nullptr);
}

HOOK(0x753420, AdjustThisAndCall64B250, "auto");
