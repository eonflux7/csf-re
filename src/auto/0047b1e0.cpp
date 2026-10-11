#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// int __thiscall CharTask_StartMovement(this, this + 0x24)
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

static int __fastcall C_0082f9f8_PrepareAndStartMovement(C_0082f9f8* self, void* /*edx*/)
{
    // The object referenced by this->field_0014 has no declared class here; its
    // sub-object at +0x44 is driven through its vtable.
    void* owner = reinterpret_cast<void*>(self->field_0014);
    void* controller = CSF_AT(void*, owner, 0x44);

    // Slot 0x90 / 4 = 36: readiness query (returns a byte flag).
    using IsReadyFn = bool(__fastcall*)(void* self, void* edx);
    auto isReady = reinterpret_cast<IsReadyFn>(CSF_AT(void**, controller, 0)[0x90 / 4]);
    if (!isReady(controller, nullptr))
    {
        // Slot 0x8C / 4 = 35: preparation step, called only when not ready.
        using PrepareFn = void(__fastcall*)(void* self, void* edx);
        auto prepare = reinterpret_cast<PrepareFn>(CSF_AT(void**, controller, 0)[0x8C / 4]);
        prepare(controller, nullptr);
    }

    // Start movement using the address of the embedded sub-object at +0x24.
    return CharTask_StartMovement(self, nullptr, reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x24));
}

HOOK(0x47B1E0, C_0082f9f8_PrepareAndStartMovement, "auto");
