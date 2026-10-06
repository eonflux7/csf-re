#include <cstdint>
#include "hook.h"

// 0x004E1240 — null subroutine (no-op). __stdcall with four stack arguments;
// the original is a bare `ret 0x10`, so the callee pops 16 bytes.
static void __stdcall NullSub_6(int /*a1*/, int /*a2*/, int /*a3*/, int /*a4*/)
{
}

HOOK(0x4E1240, NullSub_6, "auto");
