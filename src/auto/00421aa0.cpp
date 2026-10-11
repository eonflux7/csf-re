#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Enter-state helper for an AI task state: asks the owner's body object to run
// its sub-object hook (vtbl slot 14), resets the body's mode via vtbl slot 35
// (args 11, 0, -1, 0), then sets bit 0x10 in the body's flag word at +0x1F0.
static int* __fastcall AiTaskStates_EnterStateSetFlag10(void* self, void* /*edx*/)
{
    // First call: sub = owner->body->+0x48; sub->vtbl[14](sub, owner)
    void* owner = CSF_AT(void*, self, 0x8);
    void* body = CSF_AT(void*, owner, 0x14);
    void* sub = CSF_AT(void*, body, 0x48);
    ((void(__fastcall*)(void*, void*, void*))(CSF_AT(void**, sub, 0)[14]))(sub, nullptr, owner);

    // Second call: body->vtbl[35](body, 11, 0, -1, 0)
    owner = CSF_AT(void*, self, 0x8);
    body = CSF_AT(void*, owner, 0x14);
    ((void(__fastcall*)(void*, void*, int, int, int, int))(CSF_AT(void**, body, 0)[35]))(
        body, nullptr, 11, 0, -1, 0);

    // Third: body->flags(+0x1F0) |= 0x10; return address of the flag word
    owner = CSF_AT(void*, self, 0x8);
    body = CSF_AT(void*, owner, 0x14);
    int* flagWord = reinterpret_cast<int*>(static_cast<uint8_t*>(body) + 0x1F0);
    *flagWord = *flagWord | 0x10;
    return flagWord;
}

HOOK(0x421AA0, AiTaskStates_EnterStateSetFlag10, "auto");
