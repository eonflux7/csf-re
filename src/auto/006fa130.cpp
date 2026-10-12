#include <cstdint>
#include "hook.h"
#include "globals.h"

// 0x458490 takes 4 stack slots under __cdecl: the 3 forwarded arguments plus a 4th
// value that the caller never writes (the [ebp-4] slot reserved by `push ecx`).
CSF_FUNC(uint32_t*, __cdecl, sub_458490, 0x00458490, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));

static uint32_t* __cdecl ScreenEffects_ForwardTo458490(uint32_t* a1, uint32_t* a2, uint32_t* a3)
{
  // The original forwards an uninitialized stack slot as the 4th argument; that value is
  // garbage with no defined meaning, so 0 is passed instead.
  return sub_458490(a1, a2, a3, 0);
}

HOOK(0x6FA130, ScreenEffects_ForwardTo458490, "auto");
