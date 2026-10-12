#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// SceneLoader_CreateLoadTimer: __cdecl, returns a new load-timer object (eax).
CSF_FUNC(uint8_t*, __cdecl, SceneLoader_CreateLoadTimer, 0x0069CDC0, ());

// C_0085e7d0 constructor: stores its vtable, creates a load timer, keeps it in
// field_0014, then calls the timer's vtable slot 2 (+0x8). Returns this.
static uint32_t* __fastcall C_0085e7d0_Construct(C_0085e7d0* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x0085E7D0);

    uint8_t* loadTimer = SceneLoader_CreateLoadTimer();
    self->field_0014 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(loadTimer));

    void** timerVtbl = CSF_AT(void**, loadTimer, 0);
    ((void(__fastcall*)(void*, void*))timerVtbl[2])(loadTimer, nullptr);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0069AE70, C_0085e7d0_Construct, "auto");
