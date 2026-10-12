#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int*, __cdecl, RpClumpStreamGetSize, 0x0058FD10, (int a1));
CSF_FUNC(int, __cdecl, RpClumpDestroy, 0x0058F740, (int a1));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorDestroy, 0x0061EE90, (int a1));

static int __fastcall C_00861068_ReleaseClumps(C_00861068* self, void* /*edx*/, uint32_t* a2) {
  uint32_t clumpIndex = 0;
  for (; clumpIndex < CSF_AT(uint32_t, a2, 0x28); ++clumpIndex) {
    auto* clumpArray = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, a2, 0x20)));
    uint32_t clump = clumpArray[clumpIndex];
    if (clump) {
      if (clumpIndex == 0) {
        CSF_AT(int32_t, self, 0x18) -= 1;
        int32_t streamSize = static_cast<int32_t>(
            reinterpret_cast<intptr_t>(RpClumpStreamGetSize(static_cast<int>(clump))));
        CSF_AT(int32_t, self, 0x14) -= streamSize;
      }
      RpClumpDestroy(static_cast<int>(clump));
    }
  }

  int result = CSF_AT(int, a2, 0x30);
  CSF_AT(int, a2, 0x1C) = 0;
  CSF_AT(int, a2, 0x28) = 0;
  if (result) {
    result = RtAnimInterpolatorDestroy(result);
    CSF_AT(int, a2, 0x30) = 0;
  }
  return result;
}

HOOK(0x6D41C0, C_00861068_ReleaseClumps, "auto");
