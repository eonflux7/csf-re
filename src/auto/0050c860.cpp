#include <cstdint>
#include "core/memory.h"
#include "hook.h"
#include "globals.h"

// Zeroes out the small menu-related structure and sets the byte at +0x10 to 1.
static int __fastcall MenuManager_InitEntry(void* self, void* /*edx*/)
{
  CSF_AT(int,     self, 0x00) = 0;
  CSF_AT(int,     self, 0x04) = 0;
  CSF_AT(uint8_t, self, 0x08) = 0;
  CSF_AT(uint8_t, self, 0x09) = 0;
  CSF_AT(int,     self, 0x0C) = 0;
  CSF_AT(uint8_t, self, 0x10) = 1;
  CSF_AT(uint8_t, self, 0x11) = 0;
  CSF_AT(uint8_t, self, 0x12) = 0;
  return 0;
}

HOOK(0x50C860, MenuManager_InitEntry, "auto");
