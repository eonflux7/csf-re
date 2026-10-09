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

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));

// Inlined strlen loop from the original (no CRT call in the original).
static size_t Hud_MeasureCString(const char* text) {
  const char* scan = text;
  while (*scan != '\0') {
    ++scan;
  }
  return static_cast<size_t>(scan - text);
}

// Returns 1 when the widget's stored text (field at +0x14) equals the given string, 0 otherwise.
static int __cdecl Hud_WidgetTextEquals(int a1, const char* a2) {
  size_t textLength = Hud_MeasureCString(a2);
  uint8_t* widget = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a1));
  uint32_t widgetText = CSF_AT(uint32_t, widget, 0x14);
  int compareResult = CompareStringSubstring(reinterpret_cast<void*>(a1), nullptr, 0,
                                             static_cast<unsigned int>(widgetText), a2,
                                             static_cast<unsigned int>(textLength));
  return compareResult == 0 ? 1 : 0;
}

HOOK(0x00431CB0, Hud_WidgetTextEquals, "auto");
