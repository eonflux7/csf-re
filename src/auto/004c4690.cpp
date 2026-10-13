#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GoToCover_StartEntryMove, 0x004C45C0, (void* self, void* edx, int a2, int a3));

CSF_GLOBAL(float, g_LevelScaleFactor, 0x0082E048);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Walks the array of 40-byte go-to-cover entries referenced by a2[0] (count in a2[2]).
// For each entry with a non-negative int at +0x10 whose float at +0x24 (+ g_LevelScaleFactor)
// is greater than the scene's float at ((scene+0xB0)+8), it starts a move via GoToCover_StartEntryMove.
static void __fastcall GoToCover_ProcessPendingEntries(S_004bd310* self, void* /*edx*/, uint32_t* a2)
{
  if (a2[2] == 0)
    return;

  uint32_t remaining = a2[2];
  uint32_t offset = 0;
  do
  {
    uint8_t* entry = reinterpret_cast<uint8_t*>(a2[0]) + offset;
    if (CSF_AT(int, entry, 0x10) >= 0)
    {
      uint8_t* sceneSub = CSF_AT(uint8_t*, g_pCurrentScene, 0xB0);
      if (CSF_AT(float, entry, 0x24) + g_LevelScaleFactor > CSF_AT(float, sceneSub, 8))
      {
        GoToCover_StartEntryMove(
            self,
            nullptr,
            CSF_AT(int, entry, 4),
            static_cast<int>(reinterpret_cast<uintptr_t>(entry + 8)));
      }
    }
    offset += 40;
    --remaining;
  } while (remaining != 0);
}

HOOK(0x004C4690, GoToCover_ProcessPendingEntries, "auto");
