#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Initializes a menu_mp_character_select entry with the given character/slot id:
//   +0x00 : stored id (returned)
//   +0x04 : zeroed dword
//   +0x08 : zeroed byte flag
//   +0x09 : byte flag set to 1
static int __fastcall MenuMPCharSel_InitEntry(void* self, void* /*edx*/, int id)
{
  CSF_AT(uint32_t, self, 0x04) = 0;
  CSF_AT(uint8_t,  self, 0x08) = 0;
  CSF_AT(uint8_t,  self, 0x09) = 1;
  CSF_AT(uint32_t, self, 0x00) = static_cast<uint32_t>(id);
  return id;
}

HOOK(0x542700, MenuMPCharSel_InitEntry, "auto");
