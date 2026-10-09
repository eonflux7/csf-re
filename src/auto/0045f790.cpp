#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall SceneTarget_ApplyFromMessage(uint8_t* self, void* /*edx*/, [[maybe_unused]] int a2, [[maybe_unused]] int a3, uint32_t* a4)
{
  uint8_t* msg = reinterpret_cast<uint8_t*>(a4[0]);
  uint8_t* v4 = *reinterpret_cast<uint8_t**>(msg + 4);
  int result = *reinterpret_cast<int*>(v4 + 12);
  uint8_t* v6 = v4 + 4;

  if (result >= 0) {
    result = CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<int>(v6));
    if (result) {
      uint8_t* resolved = reinterpret_cast<uint8_t*>(result);
      uint8_t* v8 = CSF_AT(uint8_t*, resolved, 0x34);
      if (v8) {
        uint8_t* payloadSrc = reinterpret_cast<uint8_t*>(a4[1]);
        uint8_t* payloadSub = *reinterpret_cast<uint8_t**>(payloadSrc + 4);
        int v11 = *reinterpret_cast<int*>(payloadSub + 4);
        int args[4] = {25, v11, 0, -1};

        // slot 168 / 4 = 42
        reinterpret_cast<void(__fastcall*)(void*, void*, int*)>(CSF_AT(void**, v8, 0)[42])(v8, nullptr, args);

        result = *reinterpret_cast<int*>(v6 + 4);
        CSF_AT(int, self, 0x60) = result;
        CSF_AT(int, self, 0x64) = *reinterpret_cast<int*>(v6 + 8);
        CSF_AT(uint8_t, self, 0x82) = 0;
      }
    }
  }
  return result;
}

HOOK(0x45F790, SceneTarget_ApplyFromMessage, "auto");
