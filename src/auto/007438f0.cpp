#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Global net object pointers (dword_8BC144 / dword_8BC148).
CSF_GLOBAL(void*, g_NetCallbackObjectA, 0x008BC144);
CSF_GLOBAL(void*, g_NetCallbackObjectB, 0x008BC148);

// Returns true when the object exists and its virtual slot 3 (offset +0x0C) reports ready.
static bool IsNetCallbackObjectReady(void* object)
{
    if (object == nullptr)
        return false;
    using ReadyFn = uint8_t(__fastcall*)(void* self, void* edx);
    ReadyFn isReady = reinterpret_cast<ReadyFn>(CSF_AT(void**, object, 0)[3]);
    return isReady(object, nullptr) != 0;
}

static int __cdecl NetCallbacks_AreBothCallbacksReady()
{
    if (!IsNetCallbackObjectReady(g_NetCallbackObjectA))
        return 0;
    if (!IsNetCallbackObjectReady(g_NetCallbackObjectB))
        return 0;
    return 1;
}

HOOK(0x007438F0, NetCallbacks_AreBothCallbacksReady, "auto");
