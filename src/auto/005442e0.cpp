#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0053BF50  int __thiscall sub_53BF50(int this)
CSF_FUNC(int, __fastcall, CMenuProfiles_SyncButtonsToProfileCount, 0x53BF50, (void* self, void* edx));

// 0x005442E0
// Clears the currently selected entry of the save-game menu:
// deselects the linked control, refreshes internal state, resets the
// secondary control to index 0 and invalidates the stored selection index.
static int __fastcall CMenuSaveGame_ResetSelection(void* self, void* /*edx*/)
{
  void* list = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x28));
  void* item = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x30));

  // (*list)->vtbl[2](list, item)
  ((void (__fastcall*)(void*, void*, void*))CSF_AT(void**, list, 0)[2])(list, nullptr, item);

  CMenuProfiles_SyncButtonsToProfileCount(self, nullptr);

  void* control = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x24));

  // (*control)->vtbl[0x2A](control, 0)
  int result = ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, control, 0)[0x2A])(control, nullptr, 0);

  CSF_AT(uint32_t, self, 0x38) = 0xFFFFFFFFu;

  return result;
}

HOOK(0x5442E0, CMenuSaveGame_ResetSelection, "auto");
