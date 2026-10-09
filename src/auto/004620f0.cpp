#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(char, __fastcall, IsGlobalValueActive, 0x00521C00, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ApplyGlobalValueRequest, 0x00523940, (void* self, void* edx, void* a2, float a3));
CSF_GLOBAL(float, g_MessageBoxSizeScale, 0x00831430);

static int ReadDwordAt(int baseAddress, int offset)
{
  return CSF_AT(int, reinterpret_cast<void*>(baseAddress), offset);
}

static float ReadFloatAt(int baseAddress, int offset)
{
  return CSF_AT(float, reinterpret_cast<void*>(baseAddress), offset);
}

static char __stdcall DispatchGlobalValueRequest(int /*unusedA1*/, int /*unusedArg2*/, int* paramNodes)
{
  // Request block the original builds on its stack (a2 in the pseudocode, 0x38 bytes).
  uint8_t requestBlock[0x38];
  CSF_AT(int, requestBlock, 0x00) = 1;
  CSF_AT(int, requestBlock, 0x04) = 1;
  CSF_AT(uint8_t, requestBlock, 0x34) = 0;
  CSF_AT(uint8_t, requestBlock, 0x35) = 1;
  CSF_AT(uint8_t, requestBlock, 0x36) = 1;
  CSF_AT(int, requestBlock, 0x30) = 0;
  CSF_AT(int, requestBlock, 0x28) = -1;
  CSF_AT(int, requestBlock, 0x2C) = -1;
  CSF_AT(int, requestBlock, 0x08) = -1;
  CSF_AT(int, requestBlock, 0x0C) = -1;
  CSF_AT(int, requestBlock, 0x10) = 0;
  CSF_AT(int, requestBlock, 0x14) = 0;
  CSF_AT(int, requestBlock, 0x18) = 0;
  CSF_AT(int, requestBlock, 0x1C) = 0;
  CSF_AT(int, requestBlock, 0x20) = 0;
  CSF_AT(void*, requestBlock, 0x24) = CSF_VTBL(0x0082D448);

  int firstKey = ReadDwordAt(ReadDwordAt(paramNodes[0], 4), 4);
  if (firstKey == -1) {
    return static_cast<char>(firstKey);
  }

  int secondKey = ReadDwordAt(ReadDwordAt(paramNodes[1], 4), 4);
  CSF_AT(int, requestBlock, 0x08) = firstKey;
  CSF_AT(int, requestBlock, 0x10) = secondKey;
  CSF_AT(int, requestBlock, 0x04) = 4;

  float requestValue = ReadFloatAt(ReadDwordAt(paramNodes[2], 4), 4);
  if (requestValue == g_MessageBoxSizeScale) {
    CSF_AT(int, requestBlock, 0x00) = 5;
  }

  void* globalValueObject = reinterpret_cast<void*>(GetGlobalValue_521BB0());
  char isActive = IsGlobalValueActive(globalValueObject, nullptr);
  char resultByte = isActive;
  if (isActive) {
    void* applyTarget = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    resultByte = static_cast<char>(ApplyGlobalValueRequest(applyTarget, nullptr, requestBlock, requestValue));
  }
  return resultByte;
}

HOOK(0x004620F0, DispatchGlobalValueRequest, "auto");
