#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callees bound by address.
// 0x501870: __fastcall with two register args (ecx, edx), no stack args, ret 0.
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (void* buffer, void* edx));
// 0x50D190: __cdecl, no args, ret 0.
CSF_FUNC(int, __cdecl, Game_GetGlobalState, 0x0050D190, ());
// 0x50F790: __thiscall with four stack args, ret 16 (__fastcall with ecx/edx + 4 stack args).
CSF_FUNC(unsigned int, __fastcall, Game_DispatchMenuRequest, 0x0050F790,
         (void* self, void* edx, int a2, int a3, int a4, void* a5));
// 0x4FF3C0: __thiscall, no stack args, ret 0.
CSF_FUNC(unsigned int, __fastcall, MenuBuffer_Destroy, 0x004FF3C0, (void* self, void* edx));

static unsigned int __fastcall CMenuPause_OpenSubmenuRequestGen8(CMenuPause* self, void* edx)
{
    self->field_0014 = 0;

    uint32_t requestBuffer[45];
    // The original does not set edx before this call, so the caller's edx is forwarded.
    MenuBuffer_Init(requestBuffer, edx);
    requestBuffer[2] = 2;

    int globalState = Game_GetGlobalState();
    Game_DispatchMenuRequest(reinterpret_cast<void*>(globalState), nullptr,
                             0x67656E38, 1, 3, requestBuffer);

    return MenuBuffer_Destroy(requestBuffer, nullptr);
}

HOOK(0x0053A610, CMenuPause_OpenSubmenuRequestGen8, "auto");
