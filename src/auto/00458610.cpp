#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x458430 takes 4 stack args (cdecl). This wrapper forwards the three
// caller-supplied arguments and passes the 4th slot, which the original reads
// from an unwritten [ebp-4] local, as 0.
CSF_FUNC(char*, __cdecl, sub_458430, 0x00458430, (char* a1, char* a2, char* a3, int a4));

static char* __cdecl HudStatusIcons_ForwardToIconLayout(char* a1, char* a2, char* a3)
{
  return sub_458430(a1, a2, a3, 0);
}

HOOK(0x00458610, HudStatusIcons_ForwardToIconLayout, "auto");
