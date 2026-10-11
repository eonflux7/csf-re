#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Notifies the owner's sub-object (+0x30C, vtable slot 0x90) and the context's
// child (ctx+0x3C, slot 0x94), then forwards to the context's next node (ctx+0x40)
// through a tail jump on vtable slot 0x80. If there is no next node, returns ctx.
static hook::Tail __fastcall Autonomo_RefreshAndForwardToNode(void* self, const uint32_t* /*args*/)
{
    // owner = this->+0x08
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));

    // owner->+0x30C: refresh via vtable slot 0x90
    void* subObject = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x30C));
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, subObject, 0)[0x90 / 4])(subObject, nullptr);

    // ctx = owner->+0x14; child = ctx->+0x3C; if child: child->vtbl[0x94]()
    uint8_t* ctx = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x14));
    void* child = reinterpret_cast<void*>(CSF_AT(uint32_t, ctx, 0x3C));
    if (child) {
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x94 / 4])(child, nullptr);
    }

    // Re-read owner->+0x14 after the calls (the callees may have changed it).
    ctx = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x14));
    void* next = reinterpret_cast<void*>(CSF_AT(uint32_t, ctx, 0x40));
    if (!next) {
        return hook::tail_ret(static_cast<uint32_t>(reinterpret_cast<uintptr_t>(ctx)));
    }

    // Tail-jump to next->vtbl[0x80] with next as this.
    return hook::tail_jump(next, (*reinterpret_cast<void***>(next))[0x80 / 4]);
}

HOOK_TAIL(0x4D7D40, Autonomo_RefreshAndForwardToNode, "ai_autonomous");
