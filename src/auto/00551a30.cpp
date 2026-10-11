#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

static void* AsGameObject(int value) {
  return reinterpret_cast<void*>(static_cast<uintptr_t>(value));
}

// Advances the multiplayer spawn scene-load state machine.
// Returns 1 when the state transition completed, 0 otherwise.
static int __fastcall MpSpawn_UpdateSceneLoadPhase(uint8_t* self, void* /*edx*/) {
  int result = GetGlobalState_6A6D70();
  if (result == 0) {
    return result;
  }

  void* obj = AsGameObject(GetGlobalState_6A6D70());
  bool ready = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[38])(obj, nullptr);
  if (!ready) {
    return 0;
  }

  if (CSF_AT(uint8_t, self, 0x14A4) != 0) {
    void* obj2 = AsGameObject(GetGlobalState_6A6D70());
    bool ok = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, obj2, 0)[42])(obj2, nullptr);
    if (ok) {
      SceneLoader_NoOp();
      CSF_AT(uint8_t, self, 0x14A4) = 0;
    }
    return ok ? 1 : 0;
  }

  void* state = AsGameObject(GetGlobalState_50D190());
  void* sub = CSF_AT(void*, state, 0x10);
  int handle = ((int(__fastcall*)(void*, void*))CSF_AT(void**, sub, 0)[25])(sub, nullptr);

  void* obj3 = AsGameObject(GetGlobalState_6A6D70());
  bool loaded = ((bool(__fastcall*)(void*, void*, int))CSF_AT(void**, obj3, 0)[39])(obj3, nullptr, handle);
  if (!loaded) {
    return 0;
  }

  SceneLoader_NoOp();
  void* obj4 = AsGameObject(GetGlobalState_6A6D70());
  ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj4, 0)[41])(obj4, nullptr, handle);
  CSF_AT(uint8_t, self, 0x14A4) = 1;
  return 1;
}

HOOK(0x00551A30, MpSpawn_UpdateSceneLoadPhase, "auto");
