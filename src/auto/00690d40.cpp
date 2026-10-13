#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int32_t, g_Value8BAD60, 0x008BAD60);
CSF_GLOBAL(uint8_t, g_MemDebugAllocationStatus, 0x008BAD90);
CSF_GLOBAL(uint8_t, g_MemDebugRecoveryBusy, 0x008BAEBC);

using VoidVirtualMethodFn = void(__fastcall*)(void* self, void* edx);

// Tries to run the debug allocator's recovery callback (vtable slot 3) when the
// allocator is idle and not already busy; otherwise invokes slot 1 on the object.
static char CoreMemDebug_TryRecoverAllocation() {
  if (g_Value8BAD60 != 0) {
    void* debugAllocator = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value8BAD60));

    if (g_MemDebugAllocationStatus == 0 && g_MemDebugRecoveryBusy == 0) {
      g_MemDebugRecoveryBusy = 1;
      g_MemDebugAllocationStatus = 1;
      reinterpret_cast<VoidVirtualMethodFn>(CSF_AT(void**, debugAllocator, 0)[3])(debugAllocator, nullptr);
      g_MemDebugRecoveryBusy = 0;
      return 1;
    }

    reinterpret_cast<VoidVirtualMethodFn>(CSF_AT(void**, debugAllocator, 0)[1])(debugAllocator, nullptr);
  }
  return 0;
}

HOOK(0x690D40, CoreMemDebug_TryRecoverAllocation, "auto");
