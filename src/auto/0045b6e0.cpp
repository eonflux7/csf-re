#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(void*, __fastcall, GetGlobalSubObject_4FC0A0, 0x004FC0A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ProcessSubObjectRecord, 0x00443000, (void* self, void* edx, int a2));

static int ReadDwordAt(uint32_t base, uint32_t off) {
  return *reinterpret_cast<int*>(base + off);
}

static float ReadFloatAt(uint32_t base, uint32_t off) {
  return *reinterpret_cast<float*>(base + off);
}

// Builds the 6-dword record {3, objectId, 0, x, y, z} from four pointer-chained
// values and submits it to the global sub-object.
static int __stdcall BuildAndSubmitVec3Record(int a1, int a2, int* a3) {
  (void)a1;
  (void)a2;

  int record[6];
  record[0] = 3;
  record[2] = 0;

  // First value: integer stored at (a3[0]->+4)->+4.
  record[1] = ReadDwordAt(ReadDwordAt(static_cast<uint32_t>(a3[0]), 4), 4);

  // Remaining three values: float stored at (a3[i]->+4)->+4, truncated to int.
  record[3] = static_cast<int>(ReadFloatAt(ReadDwordAt(static_cast<uint32_t>(a3[1]), 4), 4));
  record[4] = static_cast<int>(ReadFloatAt(ReadDwordAt(static_cast<uint32_t>(a3[2]), 4), 4));
  record[5] = static_cast<int>(ReadFloatAt(ReadDwordAt(static_cast<uint32_t>(a3[3]), 4), 4));

  void* globalValue = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_4FBDE0()));
  void* subObject = GetGlobalSubObject_4FC0A0(globalValue, nullptr);
  return ProcessSubObjectRecord(subObject, nullptr,
                                static_cast<int>(reinterpret_cast<uintptr_t>(record)));
}

HOOK(0x0045B6E0, BuildAndSubmitVec3Record, "auto");
