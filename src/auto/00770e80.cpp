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

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, SoundAreas_InitSubBlock, 0x00770D50, (void* self, void* edx, int a2));
CSF_FUNC(S_006b0320*, __fastcall, S_006b0320_Init, 0x006B0470, (S_006b0320* self, void* , int a2));

// Copies one sound-area record (source dwords at a2) into this object:
// identity dword, an embedded string at +4 (reset to empty, then assigned
// from the record's string), a sub-block at +0x20, a S_006b0320 member at
// +0x30, and six trailing dwords at +0x50..+0x64.
static char* __fastcall SoundAreas_InitFromRecord(char* self, void* /*edx*/, std::uint32_t* a2)
{
  CSF_AT(std::uint32_t, self, 0x00) = a2[0];

  // Embedded string object at self+4: length 0, capacity 15, first byte 0.
  CSF_AT(std::uint8_t, self, 0x08) = 0;
  CSF_AT(std::uint32_t, self, 0x18) = 0;
  CSF_AT(std::uint32_t, self, 0x1C) = 15;
  AssignStringSubstring(self + 4, nullptr, a2 + 1, 0, static_cast<std::uint32_t>(-1));

  SoundAreas_InitSubBlock(self + 32, nullptr, static_cast<int>(reinterpret_cast<std::uintptr_t>(a2 + 8)));
  S_006b0320_Init(reinterpret_cast<S_006b0320*>(self + 48), nullptr, static_cast<int>(reinterpret_cast<std::uintptr_t>(a2 + 12)));

  CSF_AT(std::uint32_t, self, 0x50) = CSF_AT(std::uint32_t, a2, 0x50);
  CSF_AT(std::uint32_t, self, 0x54) = CSF_AT(std::uint32_t, a2, 0x54);
  CSF_AT(std::uint32_t, self, 0x58) = CSF_AT(std::uint32_t, a2, 0x58);
  CSF_AT(std::uint32_t, self, 0x5C) = CSF_AT(std::uint32_t, a2, 0x5C);
  CSF_AT(std::uint32_t, self, 0x60) = CSF_AT(std::uint32_t, a2, 0x60);
  CSF_AT(std::uint32_t, self, 0x64) = CSF_AT(std::uint32_t, a2, 0x64);

  return self;
}

HOOK(0x00770E80, SoundAreas_InitFromRecord, "auto");
