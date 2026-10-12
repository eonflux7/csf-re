#include <cstdint>
#include "hook.h"
#include "globals.h"

// Hex-Rays lists 0x43F5C0 with its real 4-slot stack layout: the caller pushes the
// 3 arguments plus an unwritten [ebp-4] local as the 4th, and cleans up itself (__cdecl).
CSF_FUNC(int, __cdecl, CampaignBriefing_RunSceneStep, 0x0043F5C0, (int a1, int a2, int a3, int a4));

static int __cdecl CampaignBriefing_ForwardToSceneStep(int a1, int a2, int a3)
{
  // The original reserves the [ebp-4] slot with `push ecx` and forwards it as the 4th
  // argument without ever writing it. Per the uninitialized-value rule, pass 0.
  int unwrittenLocalArg = 0;
  return CampaignBriefing_RunSceneStep(a1, a2, a3, unwrittenLocalArg);
}

HOOK(0x0043F9E0, CampaignBriefing_ForwardToSceneStep, "auto");
