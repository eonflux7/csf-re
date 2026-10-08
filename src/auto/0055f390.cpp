#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x50D190, ());
CSF_FUNC(void*, __fastcall, SchedulerBase_GetSchedulerProxy, 0x734A10, (void* self, void* edx));

// 0x0055F390  (__thiscall, one ignored int stack argument, ret 4)
// Fetches the global scheduler, asks it for its scheduler proxy, and, when a
// proxy exists, dispatches to the object's virtual method at vtable offset 0x4C
// returning that method's result; otherwise returns the null proxy.
static void* __fastcall CMenuMpOnline_InvokeSchedulerProxyCallback(void* self, void* /*edx*/, int /*unused*/)
{
  void* schedulerBase = GetGlobalState_50D190();
  void* schedulerProxy = SchedulerBase_GetSchedulerProxy(schedulerBase, nullptr);
  if (schedulerProxy)
  {
    void** vtbl = *reinterpret_cast<void***>(self);
    return reinterpret_cast<void* (__fastcall*)(void*, void*)>(vtbl[0x4C / 4])(self, nullptr);
  }
  return schedulerProxy;
}

HOOK(0x55F390, CMenuMpOnline_InvokeSchedulerProxyCallback, "auto");
