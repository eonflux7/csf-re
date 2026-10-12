#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Fills a 18-byte minimap icon entry: two ints, a 2-dword position/size pair
// copied from a4, and two byte flags. Returns the entry pointer (eax = this).
static uint32_t* __fastcall Minimap_InitIconEntry(void* self, void* /*edx*/,
                                                  int firstValue, int secondValue,
                                                  uint32_t* pairSource,
                                                  char firstFlag, char secondFlag)
{
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(int, self, 0x00) = firstValue;
    CSF_AT(int, self, 0x04) = secondValue;
    CSF_AT(uint32_t, self, 0x08) = pairSource[0];
    CSF_AT(uint32_t, self, 0x0C) = pairSource[1];
    CSF_AT(uint8_t, self, 0x10) = static_cast<uint8_t>(firstFlag);
    CSF_AT(uint8_t, self, 0x11) = static_cast<uint8_t>(secondFlag);
    return static_cast<uint32_t*>(self);
}

HOOK(0x43AB60, Minimap_InitIconEntry, "auto");
