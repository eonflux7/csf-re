#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x7726C0 takes (a1, a2, a3) per its packet bind line. The original pushes
// three arguments plus an unwritten [ebp-4] slot; the bind line is kept exactly
// as given, so only the three caller-supplied arguments are forwarded.
CSF_FUNC(int, __cdecl, AnimDefs_ProcessEntry, 0x007726C0, (void* a1, void* a2, int a3));

static int __cdecl ForwardAnimDefsArgsToProcessEntry(int a1, int a2, int a3)
{
  return AnimDefs_ProcessEntry(reinterpret_cast<void*>(a1), reinterpret_cast<void*>(a2), a3);
}

HOOK(0x00772830, ForwardAnimDefsArgsToProcessEntry, "auto");
