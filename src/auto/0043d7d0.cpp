#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

// Looks up the menu item named by the string object a3 (text at +4 or inline-pointer at +4,
// length at +0x18) and forwards the found handle to the owner object held at this+0x1C.
static int __fastcall C_00830e5c_SelectNamedMenuItem(C_00830e5c* self, void* /*edx*/, void* a3)
{
  int result = static_cast<int>(self->field_001c);
  if (result != 0)
  {
    result = CompareStringSubstring(a3, nullptr, 0u, CSF_AT(uint32_t, a3, 0x14),
                                    reinterpret_cast<const char*>(CSF_ADDR_DATA(0x0082E65C)), 0u);
    if (result)
    {
      void* menuObj = MenuControls_ResolveObject(
          reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt())), nullptr);

      const char* nameText;
      if (CSF_AT(uint32_t, a3, 0x18) < 0x10u)
        nameText = reinterpret_cast<const char*>(reinterpret_cast<uint8_t*>(a3) + 4);
      else
        nameText = reinterpret_cast<const char*>(static_cast<uintptr_t>(CSF_AT(uint32_t, a3, 0x04)));

      void** menuVtbl = CSF_AT(void**, menuObj, 0);
      int childHandle = reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(menuVtbl[31])(
          menuObj, nullptr, nameText);

      result = childHandle;
      if (childHandle != 0)
      {
        void** ownerVtbl = CSF_AT(void**, reinterpret_cast<void*>(self->field_001c), 0);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(ownerVtbl[19])(
            reinterpret_cast<void*>(self->field_001c), nullptr, childHandle);

        void* menuObj2 = MenuControls_ResolveObject(
            reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt())), nullptr);
        void** menu2Vtbl = CSF_AT(void**, menuObj2, 0);
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(menu2Vtbl[34])(
            menuObj2, nullptr, childHandle);
      }
    }
  }
  return result;
}

HOOK(0x0043D7D0, C_00830e5c_SelectNamedMenuItem, "auto");
