#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(int, __fastcall, WeaponFirearms_Serialize, 0x00477B40, (void* self, void* , S_00691860* archive));

// Serializes the character action state. Returns the value of the state object's
// finish/continue virtual check when the second check succeeds, otherwise 1.
static int __fastcall CharActionFsm_SerializeStateData(void* self, void* /*edx*/, S_00691860* a2)
{
  // First check: skip-serialization query on the state object (vtable slot 0x80 / 4 = 32).
  bool v4 = false;
  if (a2->pad_0000[0] == 0)
  {
    void* stateObj = reinterpret_cast<void*>(a2->field_0004);
    using ShouldSkipFn = bool(__fastcall*)(void*, void*, int, S_00691860*);
    ShouldSkipFn shouldSkip = reinterpret_cast<ShouldSkipFn>(CSF_AT(void**, stateObj, 0)[32]);
    v4 = !shouldSkip(stateObj, nullptr, 0, a2);
  }

  uint32_t savedField = CSF_AT(uint32_t, a2, 0x1C);
  a2->pad_0000[0] = v4;
  CSF_AT(uint32_t, a2, 0x1C) = 0;

  if (!v4)
  {
    CSF_AT(uint8_t, a2, 0x15) = 0;
    CSF_AT(uint8_t, a2, 0x16) = 0;
    Archive_SerializeString(reinterpret_cast<int>(self), a2);
  }

  bool v6 = a2->pad_0000[0] == 0;
  CSF_AT(uint32_t, a2, 0x1C) = 0;
  if (v6)
  {
    CSF_AT(uint8_t, a2, 0x15) = 0;
    CSF_AT(uint8_t, a2, 0x16) = 0;
    WeaponFirearms_Serialize(reinterpret_cast<void*>(static_cast<uint8_t*>(self) + 0xC), nullptr, a2);
  }

  v6 = a2->pad_0000[0] == 0;
  CSF_AT(uint32_t, a2, 0x1C) = savedField;

  // Second check: virtual call at vtable slot 0x7C / 4 = 31 on the state object.
  int result = 0;
  bool finished = false;
  if (v6)
  {
    void* stateObj = reinterpret_cast<void*>(a2->field_0004);
    using FinishFn = int(__fastcall*)(void*, void*, S_00691860*);
    FinishFn finish = reinterpret_cast<FinishFn>(CSF_AT(void**, stateObj, 0)[31]);
    result = finish(stateObj, nullptr, a2);
    finished = static_cast<uint8_t>(result) != 0;
  }

  if (finished)
  {
    a2->pad_0000[0] = 0;
    return result;
  }

  a2->pad_0000[0] = 1;
  return 1;
}

HOOK(0x0048B5C0, CharActionFsm_SerializeStateData, "auto");
