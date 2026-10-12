#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x772710 takes 4 stack args (cdecl). This wrapper forwards the three
// caller-supplied arguments and passes the 4th slot, which the original reads
// from an unwritten [ebp-4] local, as 0.
CSF_FUNC(void*, __cdecl, sub_772710, 0x00772710, (void* a1, void* a2, void* a3, int a4));

static void* __cdecl AnimDefs_Forward3ArgsTo772710(void* a1, void* a2, void* a3)
{
  return sub_772710(a1, a2, a3, 0);
}

HOOK(0x00772890, AnimDefs_Forward3ArgsTo772710, "auto");
