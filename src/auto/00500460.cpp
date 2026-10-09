#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200,
         (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(uint32_t*, __fastcall, InitGlobalRequest_405A00, 0x00405A00, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, ApplyGlobalValueRequest, 0x00523940, (void* self, void* edx, void* a2, float a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, CallInnerObjectAt90_SecondVirtual, 0x0050DDD0, (void* self, void* edx));
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Checks the string at self+0xAC against g_byte_82E65C; if it matches nothing and the
// value at v2+0x14 is set (!= -1), queues a global-value request with it, then
// dispatches through the global state object.
static int __fastcall SceneLoader_ProcessPendingGlobalRequest(void* self, void* /*edx*/)
{
    uint8_t* v2 = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetValue8B0554())) + 0x54;

    if (CompareStringSubstring(reinterpret_cast<uint8_t*>(self) + 0x98, nullptr, 0,
                               CSF_AT(uint32_t, self, 0xAC),
                               reinterpret_cast<const char*>(&g_byte_82E65C), 0) == 0
        && CSF_AT(int, v2, 0x14) != -1)
    {
        uint8_t request[0x38];
        InitGlobalRequest_405A00(request, nullptr);
        CSF_AT(int, request, 0x08) = CSF_AT(int, v2, 0x14);
        CSF_AT(int, request, 0x00) = 3;
        CSF_AT(int, request, 0x10) = 0;

        // GetGlobalValue_521BB0 takes no arguments; the two pushed values above are
        // popped by ApplyGlobalValueRequest (ret 8), so they are passed as its parameters.
        int globalValueObject = GetGlobalValue_521BB0();
        ApplyGlobalValueRequest(reinterpret_cast<void*>(static_cast<uintptr_t>(globalValueObject)),
                                nullptr, request, -1.0f);
    }

    int globalState = GetGlobalState_50D190();
    return CallInnerObjectAt90_SecondVirtual(reinterpret_cast<void*>(static_cast<uintptr_t>(globalState)), nullptr);
}

HOOK(0x00500460, SceneLoader_ProcessPendingGlobalRequest, "auto");
