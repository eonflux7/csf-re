#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot at byte offset 0xA4 (slot index 0xA4 / 4 = 41).
// Shuts down the optional child at +0x39C, notifies the object at +0x22C,
// then tail-jumps into the object at +0x16C's slot 0xA4 with the caller's frame.
static hook::Tail __fastcall CPlayerCharacter_ShutdownSubobjectsAndForward(void* self, const uint32_t* /*args*/)
{
    CPlayerCharacter* player = reinterpret_cast<CPlayerCharacter*>(self);

    void* optionalObj = reinterpret_cast<void*>(player->field_039c);
    if (player->field_039c != 0)
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, optionalObj, 0)[0xA4 / 4])(optionalObj, nullptr);

    void* notifyObj = reinterpret_cast<void*>(player->field_022c);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, notifyObj, 0)[0xA4 / 4])(notifyObj, nullptr);

    void* forwardObj = reinterpret_cast<void*>(player->field_016c);
    return hook::tail_jump(forwardObj, (*reinterpret_cast<void***>(forwardObj))[0xA4 / 4]);
}

HOOK_TAIL(0x495450, CPlayerCharacter_ShutdownSubobjectsAndForward, "auto");
