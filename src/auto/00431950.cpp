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

// Resolves a HUD sub-object for the given request id via 0x42EBA0, then forwards it
// to the dispatcher 0x42F4F0 with a zero flag when the lookup succeeds.
CSF_FUNC(int, __fastcall, ResolveHudSubObject, 0x0042EBA0, (void* self, void* edx, int a2));
CSF_FUNC(void, __cdecl, DispatchHudSubObject, 0x0042F4F0, (int a1, int* a2, int a3));

static void __fastcall CHud_DispatchToResolvedControl(void* self, void* /*edx*/, int requestId)
{
    // The original passes the caller's edi through as the first (register) argument
    // of 0x42F4F0; that register value is not expressible in portable C++ here.
    const int incomingEdi = 0;

    int* resolvedSubObject = reinterpret_cast<int*>(
        static_cast<intptr_t>(static_cast<int32_t>(
            ResolveHudSubObject(self, nullptr, requestId))));

    if (resolvedSubObject) {
        DispatchHudSubObject(incomingEdi, resolvedSubObject, 0);
    }
}

HOOK(0x00431950, CHud_DispatchToResolvedControl, "auto");
