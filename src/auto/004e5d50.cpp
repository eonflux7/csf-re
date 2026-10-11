#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor body for C_0083e9d0 (base C_0082d410).
// Frees the element array at field_0020 (count header at -4, 20-byte elements whose
// vtable dword at +8 is reset to C_0082d410), then releases the owned object at field_000c.
static void __fastcall C_0083e9d0_Destroy(C_0083e9d0* self, void* /*edx*/) {
  self->vtbl = CSF_VTBL(0x0083E9D0);
  self->field_0004 = CSF_ADDR_DATA(0x0083E9B0);

  if (self->field_0020) {
    uint8_t* arrayBase = reinterpret_cast<uint8_t*>(self->field_0020);
    int elementCount = CSF_AT(int, arrayBase, -4);
    for (int i = elementCount - 1; i >= 0; --i) {
      CSF_AT(void*, arrayBase, 20 * i + 8) = CSF_VTBL(0x0082D410);
    }
    Mem_Free(arrayBase - 4);
    self->field_0020 = 0;
  }

  void** owned = reinterpret_cast<void**>(self->field_000c);
  if (owned) {
    if (CSF_AT(int, owned, -4)) {
      using VirtualDtorFn = void(__fastcall*)(void*, void*, int);
      VirtualDtorFn dtor = reinterpret_cast<VirtualDtorFn>(CSF_AT(void**, owned, 0)[0]);
      dtor(owned, nullptr, 3);
    } else {
      Mem_Free(reinterpret_cast<uint8_t*>(owned) - 4);
    }
    self->field_000c = 0;
  }

  self->field_0004 = CSF_ADDR_DATA(0x0082D410);
  self->vtbl = CSF_VTBL(0x0082D410);
}

HOOK(0x004E5D50, C_0083e9d0_Destroy, "auto");
