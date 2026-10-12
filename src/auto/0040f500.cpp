#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, StateNode_UpdateBase, 0x0041E140, (C_0082e520* self, void* /*edx*/, int a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

// Serializes a state node through an archive object. When the archive's first byte is clear,
// the archive's embedded object (+4) is probed through its vtable to compute the initial flag.
// The base state is serialized, and if the result is still clear, a secondary object reference
// is serialized with the node's field at +0x1C preserved around the call. Returns 1 on failure, 0 on success.
static int __fastcall C_0082ddf8_SerializeState(C_0082ddf8* self, void* /*edx*/, void* archive)
{
  uint8_t v3 = 0;
  if (CSF_AT(uint8_t, archive, 0) == 0)
  {
    void* obj = CSF_AT(void*, archive, 4);
    uint8_t probeResult = ((uint8_t(__fastcall*)(void*, void*, int, void*))CSF_AT(void**, obj, 0)[32])(obj, nullptr, 0, archive);
    v3 = (probeResult == 0) ? 1 : 0;
  }

  int savedField1C = CSF_AT(int, archive, 0x1C);
  CSF_AT(uint8_t, archive, 0) = v3;

  StateNode_UpdateBase(reinterpret_cast<C_0082e520*>(self), nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(archive)));

  CSF_AT(int, archive, 0x1C) = 0;
  if (CSF_AT(uint8_t, archive, 0) == 0)
  {
    CSF_AT(uint8_t, archive, 0x15) = 1;
    CSF_AT(uint8_t, archive, 0x16) = 0;
    Archive_SerializeObjectRef(reinterpret_cast<void*>(&self->field_001c), reinterpret_cast<bool*>(archive));
  }

  CSF_AT(int, archive, 0x1C) = savedField1C;

  if (CSF_AT(uint8_t, archive, 0) == 0)
  {
    void* obj = CSF_AT(void*, archive, 4);
    uint8_t shouldFail = ((uint8_t(__fastcall*)(void*, void*, void*))CSF_AT(void**, obj, 0)[31])(obj, nullptr, archive);
    if (shouldFail)
    {
      CSF_AT(uint8_t, archive, 0) = 0;
      return 0;
    }
  }

  CSF_AT(uint8_t, archive, 0) = 1;
  return 1;
}

HOOK(0x0040F500, C_0082ddf8_SerializeState, "auto");
