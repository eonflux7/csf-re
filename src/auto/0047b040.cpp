#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

// Serializes a task's state through an archive object. The archive's first byte is a
// flag; when it is clear, the archive's embedded object (+4) is consulted through its
// vtable to decide the result. Returns 1 on failure, 0 on success.
static int __fastcall AiAttackTasks_SerializeTaskState(S_0041f0a0* self, void* /*edx*/, S_0041f0a0* a2)
{
  // v3 = !flag && !obj->vtbl[32](obj, 0, a2)
  uint8_t v3 = 0;
  if (CSF_AT(uint8_t, a2, 0) == 0)
  {
    void* obj = CSF_AT(void*, a2, 4);
    uint8_t probeResult = ((uint8_t(__fastcall*)(void*, void*, int, S_0041f0a0*))CSF_AT(void**, obj, 0)[32])(obj, nullptr, 0, a2);
    v3 = (probeResult == 0) ? 1 : 0;
  }

  int savedField1C = CSF_AT(int, a2, 0x1C);
  CSF_AT(uint8_t, a2, 0) = v3;

  Archive_SerializeBaseFields(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(a2)));

  bool needsStringSerialize = (CSF_AT(uint8_t, a2, 0) == 0);
  CSF_AT(int, a2, 0x1C) = 0;
  if (needsStringSerialize)
  {
    CSF_AT(uint8_t, a2, 0x15) = 0;
    CSF_AT(uint8_t, a2, 0x16) = 0;
    Archive_SerializeString(
        static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x14)),
        reinterpret_cast<S_00691860*>(a2));
  }

  CSF_AT(int, a2, 0x1C) = savedField1C;

  if (CSF_AT(uint8_t, a2, 0) == 0)
  {
    void* obj = CSF_AT(void*, a2, 4);
    uint8_t shouldFail = ((uint8_t(__fastcall*)(void*, void*, S_0041f0a0*))CSF_AT(void**, obj, 0)[31])(obj, nullptr, a2);
    if (shouldFail)
    {
      CSF_AT(uint8_t, a2, 0) = 0;
      return 0;
    }
  }

  CSF_AT(uint8_t, a2, 0) = 1;
  return 1;
}

HOOK(0x0047B040, AiAttackTasks_SerializeTaskState, "auto");
