#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(uint32_t, g_PauseCallbackPrimary, 0x008BC148);
CSF_GLOBAL(uint32_t, g_PauseCallbackSecondary, 0x008BC144);
CSF_GLOBAL(int, g_PauseCallbackPending, 0x008BC158);

static hook::Tail __fastcall MenuMpPause_PrepareInitialization(void* /*self*/, const uint32_t* /*args*/)
{
  g_PauseCallbackPending = 0;

  if (g_PauseCallbackPrimary != 0) {
    void* primary = reinterpret_cast<void*>(g_PauseCallbackPrimary);
    bool primaryReady = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, primary, 0)[3])(primary, nullptr);
    if (primaryReady) {
      primary = reinterpret_cast<void*>(g_PauseCallbackPrimary);
      ((void(__fastcall*)(void*, void*))CSF_AT(void**, primary, 0)[2])(primary, nullptr);
    }
  }

  if (g_PauseCallbackSecondary != 0) {
    void* secondary = reinterpret_cast<void*>(g_PauseCallbackSecondary);
    bool secondaryReady = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, secondary, 0)[3])(secondary, nullptr);
    if (secondaryReady) {
      void* obj = reinterpret_cast<void*>(g_PauseCallbackSecondary);
      return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x08 / 4]);
    }
  }

  return hook::tail_ret(0);
}

HOOK_TAIL(0x007438A0, MenuMpPause_PrepareInitialization, "net_callbacks");
