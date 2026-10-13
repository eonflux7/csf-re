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

// Singleton slot whose address is passed to the lookup routine.
CSF_GLOBAL(uint32_t, dword_8BC188, 0x008BC188);

CSF_FUNC(int, __cdecl, GetNetSingletonInstance, 0x007A8660, (uint32_t* a1));
CSF_FUNC(char, __fastcall, NetSingleton_SetFlag, 0x007A69A0, (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, NetSingleton_SetRtcErrors, 0x007A68D0, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, AiMountTasks_GetRtcNumErrors, 0x004A7DA0, ());

// Looks up the net singleton; if present, clears its flag, stores the RTC error count
// into it, then tail-jumps through the object's vtable slot 0x40 (slot 16).
static hook::Tail __fastcall NetCallbacks_ForwardRtcErrorsToSingleton(void* self, const uint32_t* args)
{
    (void)self;
    (void)args;

    C_00858180* singleton = reinterpret_cast<C_00858180*>(GetNetSingletonInstance(&dword_8BC188));
    if (singleton == nullptr)
    {
        return hook::tail_ret(0);
    }

    NetSingleton_SetFlag(singleton, nullptr, 0);
    int rtcErrors = AiMountTasks_GetRtcNumErrors();
    NetSingleton_SetRtcErrors(singleton, nullptr, static_cast<char>(rtcErrors));

    void* obj = singleton;
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x40 / 4]);
}

HOOK_TAIL(0x007437E0, NetCallbacks_ForwardRtcErrorsToSingleton, "net_callbacks");
