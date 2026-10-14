#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, AutonomySetPendingState, 0x00492C10, (void* self, void* edx, int a1, int a2));

static int __fastcall Autonomo_BeginActionWithFlag(void* self, void* /*edx*/, int a2, int a3, char a4, int a5)
{
    // self->owner (self + 8); owner->ctx (owner + 0x14); ctx->sub (ctx + 0x30)
    uint8_t* owner = CSF_AT(uint8_t*, self, 0x08);
    uint8_t* ctx = CSF_AT(uint8_t*, owner, 0x14);
    uint8_t* sub = CSF_AT(uint8_t*, ctx, 0x30);
    CSF_AT(uint8_t, sub, 0x230) = 1;

    AutonomySetPendingState(ctx, nullptr, 1, a5);

    // owner->flags |= 1 (re-read owner after the call)
    owner = CSF_AT(uint8_t*, self, 0x08);
    CSF_AT(uint32_t, owner, 0x18) |= 1u;

    // Reload the chain after the call, as the original does
    ctx = CSF_AT(uint8_t*, CSF_AT(uint8_t*, self, 0x08), 0x14);
    sub = CSF_AT(uint8_t*, ctx, 0x30);
    void** vtbl = CSF_AT(void**, sub, 0);
    CSF_AT(int, sub, 0x1c) = (a4 != 0);

    // vtable slot 0xAC / 4 on sub: the original leaves sub in ecx (mov ecx, [eax+0x30]; mov eax, [ecx]) and pushes
    // only a2, a3, so it is a __thiscall with this = sub that pops its own 8 bytes; result is returned
    return reinterpret_cast<int(__thiscall*)(void*, int, int)>(vtbl[0xAC / 4])(sub, a2, a3);
}

HOOK(0x004D7E10, Autonomo_BeginActionWithFlag, "auto");
