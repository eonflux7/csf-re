#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee bindings (one name and signature per address).
CSF_FUNC(int, __fastcall, sub_4AEB90, 0x004AEB90, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_4D7670, 0x004D7670, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

// Original: char __thiscall sub_4D8340(int* this, int a2); ret 4.
// Forwards a2 to the pending task object's virtual slot 0x90, then, if the
// owner character is idle, resolves the owner's state and starts movement.
static char __fastcall Autonomo_ProcessCharacterMovement(uint8_t* self, void* /*edx*/, int a2)
{
  int v5 = 0;

  // Pending task object at +0x18: notify it through vtable slot 0x90 / 4 = 36.
  uint32_t taskObjectAddr = CSF_AT(uint32_t, self, 0x18);
  if (taskObjectAddr != 0) {
    void* taskObject = reinterpret_cast<void*>(taskObjectAddr);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, taskObject, 0)[36])(taskObject, nullptr, a2);
  }

  // Owner character at +0x08.
  uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));

  if (sub_4D7670(owner, nullptr)) {
    v5 = sub_4AEB90(CSF_AT(void*, owner, 0x30C), nullptr);
    if (v5 != -1) {
      if (v5 == 0) {
        v5 = CSF_AT(uint8_t, owner, 0x33C);
      }
      if (v5 == 0 || CSF_AT(uint8_t, owner, 0x33C) == 0) {
        // Original leaves only the low byte of v5 updated here.
        v5 = static_cast<uint8_t>(CharTask_StartMovement(
            owner, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(owner + 0x164))));
      }
    }
  }

  return static_cast<char>(v5);
}

HOOK(0x004D8340, Autonomo_ProcessCharacterMovement, "auto");
