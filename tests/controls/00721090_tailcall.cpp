#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwards the call to the virtual method at vtable offset 0x64 of the object
// stored at self+0x134 (the original is a tail jump: mov ecx,[ecx+0x134];
// mov eax,[ecx]; jmp [eax+0x64]). The callee is a __thiscall taking no stack
// arguments, so this thunk pops nothing, exactly like the original.
static int __fastcall C_00864e18_CallEmbeddedObjectVtbl64(C_00864e18* self, void* /*edx*/)
{
  void* embeddedObject = *CSF_AT(void**, self, 0x134);

  int (__fastcall* method)(void* self, void* edx) = reinterpret_cast<int(__fastcall*)(void*, void*)>(
      CSF_AT(void**, embeddedObject, 0)[0x64 / 4]);

  return method(embeddedObject, nullptr);
}

HOOK(0x00721090, C_00864e18_CallEmbeddedObjectVtbl64, "auto");
