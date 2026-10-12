#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, StateNode_UpdateBase, 0x0041E140, (C_0082e520* self, void* /*edx*/, int a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

// Serializes a state node through an archive object. When the archive's first byte is clear,
// the archive's embedded object (+4) is probed through its vtable (slot 32) to compute the initial flag.
// The base state is serialized; if the result is still clear, an object reference at this+0x10 is
// serialized, with the archive's field at +0x1C cleared during that call and restored afterwards.
// A final vtable probe (slot 31) may force failure. Returns 1 on failure, 0 on success.
static int __fastcall C_0082db10_SerializeState(C_0082db10* self, void* /*edx*/, void* archive)
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

  StateNode_UpdateBase(reinterpret_cast<C_0082e520*>(self), nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(archive)));

  CSF_AT(int, archive, 0x1C) = 0;
  if (CSF_AT(uint8_t, archive, 0) == 0)
  {
    CSF_AT(uint8_t, archive, 0x15) = 1;
    CSF_AT(uint8_t, archive, 0x16) = 0;
    Archive_SerializeObjectRef(reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x10), reinterpret_cast<bool*>(archive));
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

HOOK(0x0040C770, C_0082db10_SerializeState, "auto");
