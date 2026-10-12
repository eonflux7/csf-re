#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));
CSF_FUNC(void, __fastcall, GfxObject_UpdateAttachment, 0x006E50C0, (void* self, void* edx, float a2));

static void __fastcall GfxObject_UpdateChild(void* self, void* /*edx*/, float a2)
{
  void* owner = CSF_AT(void*, self, 0x40);

  // Vtable slot 7 (+0x1C) on the owner: argumentless getter returning C_0085e7b8*.
  using ChildTableGetterFn = void* (__fastcall*)(void* self, void* edx);
  auto* childTable = reinterpret_cast<C_0085e7b8*>(
      reinterpret_cast<ChildTableGetterFn>(CSF_AT(void**, owner, 0)[7])(owner, nullptr));

  int entryAddress = C_0085e7b8_FindEntryPayloadByKey(childTable, nullptr, 5);
  if (entryAddress == 0)
    return;

  void** entry = reinterpret_cast<void**>(static_cast<uintptr_t>(entryAddress));
  GfxObject_UpdateAttachment(*entry, nullptr, a2);
}

HOOK(0x006E5720, GfxObject_UpdateChild, "auto");
