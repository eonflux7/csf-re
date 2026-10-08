#include <cstdint>
#include "hook.h"
#include "globals.h"

// 0x007A8ED0 - query/predicate invoked with the caller-supplied argument.
// Returns its value in eax; only the low byte is tested by the target.
// Caller cleans the stack (add esp, 4) => __cdecl, one argument.
CSF_FUNC(int, __cdecl, Sub_7A8ED0, 0x7A8ED0, (int a));

// 0x005604D0 - menu_mp_sessions / CMenuMpSessions.
// Runs a query on the given argument; if it succeeds and the cached session
// pointer (offset +0x10 of the passed sub-object) is null, forwards to the
// owning object's virtual method at vtable slot 19 (0x4C/4) and returns that.
static int __fastcall CMenuMpSessions_ResolveSession(void* self, void* /*edx*/, int a2, int a3)
{
  int result = Sub_7A8ED0(a2);
  if ((uint8_t)result)
  {
    // Re-read the cached session pointer (0x10) after the query call.
    result = *reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x10);
    if (!result)
    {
      // The owning object's vtable lives 4 bytes before this sub-object.
      void** vtbl = *reinterpret_cast<void***>(reinterpret_cast<uint8_t*>(self) - 4);
      typedef int(__fastcall* ResolveFn)(void* self, void* /*edx*/);
      return reinterpret_cast<ResolveFn>(vtbl[0x4C / 4])(
          reinterpret_cast<uint8_t*>(self) - 4, nullptr);
    }
  }
  return result;
}

HOOK(0x5604D0, CMenuMpSessions_ResolveSession, "auto");
