#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00861d48_Serialize, 0x006E7BB0, (void* self, void* edx, int a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

// Serializes a C_008632c0 node through an archive object. When the archive's first byte is clear,
// the archive's embedded object (+4) is probed through vtable slot 32 to compute the initial flag.
// The base class is serialized; then the archive's field at +0x1C is cleared, and if the result is
// still clear an object reference at this+0x4C is serialized. The field at +0x1C is restored afterwards.
// A final vtable probe (slot 31) on the embedded object may force failure. Returns 1 on failure, 0 on success.
static int __fastcall C_008632c0_SerializeState(C_008632c0* self, void* /*edx*/, void* archive)
{
  uint8_t initialFlag = 0;
  if (CSF_AT(uint8_t, archive, 0) == 0)
  {
    void* probeObj = CSF_AT(void*, archive, 4);
    uint8_t probeResult = ((uint8_t(__fastcall*)(void*, void*, int, void*))CSF_AT(void**, probeObj, 0)[32])(probeObj, nullptr, 0, archive);
    initialFlag = (probeResult == 0) ? 1 : 0;
  }

  int savedField1C = CSF_AT(int, archive, 0x1C);
  CSF_AT(uint8_t, archive, 0) = initialFlag;

  C_00861d48_Serialize(reinterpret_cast<void*>(self), nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(archive)));

  CSF_AT(int, archive, 0x1C) = 0;
  if (CSF_AT(uint8_t, archive, 0) == 0)
  {
    CSF_AT(uint8_t, archive, 0x15) = 0;
    CSF_AT(uint8_t, archive, 0x16) = 0;
    Archive_SerializeObjectRef(reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x4C), reinterpret_cast<bool*>(archive));
  }

  CSF_AT(int, archive, 0x1C) = savedField1C;

  if (CSF_AT(uint8_t, archive, 0) == 0)
  {
    void* finalObj = CSF_AT(void*, archive, 4);
    uint8_t shouldFail = ((uint8_t(__fastcall*)(void*, void*, void*))CSF_AT(void**, finalObj, 0)[31])(finalObj, nullptr, archive);
    if (shouldFail)
    {
      CSF_AT(uint8_t, archive, 0) = 0;
      return 0;
    }
  }

  CSF_AT(uint8_t, archive, 0) = 1;
  return 1;
}

HOOK(0x00707190, C_008632c0_SerializeState, "auto");
