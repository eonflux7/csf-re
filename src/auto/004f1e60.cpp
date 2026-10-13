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

CSF_FUNC(std::uint32_t*, __fastcall, S_004f21d0_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));

// Stateful text-serialization step: when the state block's dirty flag (+0x14) is set, it
// asks the attached object (+4, vtable slot 27) for the current text and clears/sets the
// flag; otherwise it asks the object (vtable slot 29) for text into a 256-entry stack buffer
// and, on failure, assigns that buffer into the owner object.
static int __stdcall ProfilesSavegames_SerializeBase(S_004f21d0* a1, void* arg4)
{
  if (CSF_AT(std::uint8_t, arg4, 0x14))
  {
    // Choose the text storage: inline buffer when the length is below 8, otherwise the heap pointer.
    std::uint16_t* textPtr;
    if (a1->field_0018 < 8)
      textPtr = reinterpret_cast<std::uint16_t*>(&a1->field_0004);
    else
      textPtr = *reinterpret_cast<std::uint16_t**>(&a1->field_0004);

    if (CSF_AT(std::uint8_t, arg4, 0) == 0)
    {
      void* obj = CSF_AT(void*, arg4, 4);
      bool failed = ((bool(__fastcall*)(void*, void*, std::uint16_t*, void*))CSF_AT(void**, obj, 0)[27])(obj, nullptr, textPtr, arg4);
      if (!failed)
      {
        CSF_AT(std::uint8_t, arg4, 0) = 1;
        return 1;
      }
    }
    CSF_AT(std::uint8_t, arg4, 0) = 0;
    return 0;
  }

  std::int16_t textBuffer[256];
  int result = 0;
  if (CSF_AT(std::uint8_t, arg4, 0) == 0)
  {
    void* obj = CSF_AT(void*, arg4, 4);
    bool failed = ((bool(__fastcall*)(void*, void*, std::int16_t*, int, void*))CSF_AT(void**, obj, 0)[29])(obj, nullptr, textBuffer, 256, arg4);
    result = failed ? 0 : 1;
  }

  CSF_AT(std::uint8_t, arg4, 0) = static_cast<std::uint8_t>(result);
  if (result == 0)
  {
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(
        S_004f21d0_AssignText(reinterpret_cast<std::uint32_t*>(a1), nullptr, textBuffer)));
  }
  return result;
}

HOOK(0x004F1E60, ProfilesSavegames_SerializeBase, "auto");
