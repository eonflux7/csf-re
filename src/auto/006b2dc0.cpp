#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Resolves a sub-object either at a fixed offset (index -1) or through the
// virtual index chain (vSlot98 -> vSlot8C -> child vtbl[6]), then calls
// a2Object's vtbl[6] with (a4Flag == 1) and passes the returned token to the
// sub-object's vtbl[33] (+0x84).
static int __fastcall C_00860288_ApplyFlagToSubObject(C_00860288* self, void* /*edx*/,
                                                      void* a2Object, int a3Index, int a4Flag) {
  int result = a3Index;
  void* subObject = nullptr;

  if (a3Index == -1) {
    subObject = reinterpret_cast<uint8_t*>(self) + 0x30;
  } else {
    result = self->vSlot98(a3Index);
    if (result == -1) {
      return result;
    }
    void* childObject = reinterpret_cast<void*>(
        static_cast<uintptr_t>(self->vSlot8C(static_cast<unsigned int>(result))));
    result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, childObject, 0)[6])(childObject, nullptr);
    subObject = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
  }

  if (subObject == nullptr) {
    return result;
  }

  int flagValue = (a4Flag == 1);
  int token = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, a2Object, 0)[6])(
      a2Object, nullptr, flagValue);

  return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, subObject, 0)[33])(
      subObject, nullptr, token);
}

HOOK(0x006B2DC0, C_00860288_ApplyFlagToSubObject, "auto");
