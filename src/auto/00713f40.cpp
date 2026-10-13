#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Destroys a C_00863f60 object: resets its vtable, releases the child object
// held at +0x008 (via its deleting-destructor slot with flag 1), and frees the
// text buffer at +0x014 when the small-string capacity at +0x028 is >= 16.
static int __fastcall C_00863f60_Destroy(C_00863f60* self, void* /*edx*/)
{
  int result = 0;

  *reinterpret_cast<void***>(self) = CSF_VTBL(0x00863F60);

  void* child = reinterpret_cast<void*>(self->field_0008);
  if (child) {
    result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[0])(child, nullptr, 1);
    self->field_0008 = 0;
  }

  if (self->field_0028 >= 0x10) {
    result = static_cast<int>(self->field_0014);
    if (result) {
      result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0014));
    }
  }

  self->field_0028 = 15;
  self->field_0024 = 0;
  self->field_0014 &= 0xFFFFFF00u;
  return result;
}

HOOK(0x00713F40, C_00863f60_Destroy, "auto");
