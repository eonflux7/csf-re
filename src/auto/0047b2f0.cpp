#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Runs the attack step only when the controller reports it is ready. Then it notifies
// the sub-object at y+4, looks up the task offset, and forwards the owner pointer to
// the controller's slot-15 method, returning that method's result.
static int __fastcall C_00834fa8_TryExecuteReadyAttack(C_00834fa8* self, void* /*edx*/, int /*a2*/) {
    // this->field_0008 -> x; x+0x14 -> y; y+0x44 -> controller
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
    uint8_t* ownerLink = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x14));
    void* controller = reinterpret_cast<void*>(CSF_AT(uint32_t, ownerLink, 0x44));

    // controller->vtbl[0x90 / 4]() : IsReady check, returns bool (al)
    bool ready = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, controller, 0)[0x90 / 4])(controller, nullptr);
    if (!ready) {
        return 0;
    }

    // Notify the embedded sub-object at ownerLink + 4 : vtbl[0x14 / 4]()
    uint8_t* subObject = ownerLink + 4;
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, subObject, 0)[0x14 / 4])(subObject, nullptr);

    // CharTask_LookupByOffset(this, this->field_000c + 36)
    CharTask_LookupByOffset(self, nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0x0c) + 36));

    // Re-read this->field_0008 chain after the call (callees may change state).
    uint32_t ownerValue = CSF_AT(uint32_t, self, 0x08);
    uint8_t* ownerAgain = reinterpret_cast<uint8_t*>(ownerValue);
    uint8_t* linkAgain = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, ownerAgain, 0x14));
    void* target = reinterpret_cast<void*>(CSF_AT(uint32_t, linkAgain, 0x48));

    // target->vtbl[0x3C / 4](this->field_0008)
    return ((int(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, target, 0)[0x3C / 4])(
        target, nullptr, ownerValue);
}

HOOK(0x0047B2F0, C_00834fa8_TryExecuteReadyAttack, "auto");
