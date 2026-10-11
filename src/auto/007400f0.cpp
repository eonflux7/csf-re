#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_742100, 0x00742100, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_74B200_SetFlagAndReturnTrue, 0x0074B200, (uint8_t* self, void*));

// Copies the signed byte at +0x72 into the float at +0xB8, sets bit 1 of the byte
// at +0x6F, then refreshes through Sub_64B250 and, if that succeeds, sub_742100.
static char __fastcall CMenuMpStats_SetScoreFieldAndRefresh(CMenuMpStats* self, void* /*edx*/)
{
  int scoreValue = CSF_AT(int8_t, self, 0x72);
  CSF_AT(uint8_t, self, 0x6F) |= 2;
  CSF_AT(float, self, 0xB8) = static_cast<float>(scoreValue);
  C_74B200_SetFlagAndReturnTrue(reinterpret_cast<uint8_t*>(self) + 0x6F, nullptr);
  char result = static_cast<char>(Sub_64B250(self, nullptr));
  if (result)
    return sub_742100(self, nullptr);
  return result;
}

HOOK(0x7400F0, CMenuMpStats_SetScoreFieldAndRefresh, "auto");
