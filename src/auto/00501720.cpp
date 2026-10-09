#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, sub_4525E0, 0x004525E0, (void* self, void* edx, int a2));

// Walks a contiguous array of 88-byte records from `first` up to `last`, reassigning
// each record's name and copying a block of values from the source record at `source`.
// Returns `last`.
static C_0082d410* __cdecl SceneLoader_FillRecordRange(C_0082d410* first, C_0082d410* last, int source)
{
  C_0082d410* entry = first;
  if (first != last)
  {
    do
    {
      auto* dst = reinterpret_cast<std::uint8_t*>(entry);
      auto* src = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(source));

      AssignStringSubstring(dst, nullptr, src, 0, static_cast<std::uint32_t>(-1));

      CSF_AT(std::uint8_t, dst, 0x1C) = CSF_AT(std::uint8_t, src, 0x1C);
      CSF_AT(std::uint8_t, dst, 0x1D) = CSF_AT(std::uint8_t, src, 0x1D);
      CSF_AT(std::uint32_t, dst, 0x20) = CSF_AT(std::uint32_t, src, 0x20);
      CSF_AT(std::uint32_t, dst, 0x28) = CSF_AT(std::uint32_t, src, 0x28);
      CSF_AT(std::uint32_t, dst, 0x2C) = CSF_AT(std::uint32_t, src, 0x2C);
      CSF_AT(std::uint32_t, dst, 0x34) = CSF_AT(std::uint32_t, src, 0x34);
      CSF_AT(std::uint32_t, dst, 0x38) = CSF_AT(std::uint32_t, src, 0x38);
      CSF_AT(std::uint32_t, dst, 0x40) = CSF_AT(std::uint32_t, src, 0x40);
      CSF_AT(std::uint32_t, dst, 0x44) = CSF_AT(std::uint32_t, src, 0x44);

      sub_4525E0(dst + 0x48, nullptr, source + 72);

      entry = reinterpret_cast<C_0082d410*>(reinterpret_cast<std::uint8_t*>(entry) + 88);
    } while (entry != last);
  }
  return last;
}

HOOK(0x00501720, SceneLoader_FillRecordRange, "auto");
