#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// C_0082f9f8::TryAcquire forwarder: (self, owner, flag, timeout). Callee pops its stack args.
CSF_FUNC(void, __fastcall, C_0082f9f8_TryAcquire, 0x0048E5D0,
         (void* self, void* edx, void* owner, int flag, int timeoutMs));

// Spawn-point update, tail-jumped by the original.
CSF_FUNC(char, __fastcall, MpSpawnPoints_UpdateRelatedObject, 0x004E1E40,
         (void* self, void* edx));

// Acquires the lock object held at +0x0C (timeout 100), issues the 4-argument
// virtual call at slot +0x8C on that object, marks the state as 3 and then
// tail-calls the spawn-point update.
static char __fastcall C_0083e5e8_AcquireAndSetStateThenUpdate(C_0083e5e8* self, void* /*edx*/)
{
    void* lockObject = reinterpret_cast<void*>(self->field_000c);

    C_0082f9f8_TryAcquire(lockObject, nullptr, self, 0, 100);

    void* lockVtbl = CSF_AT(void**, lockObject, 0)[0];
    (void)lockVtbl;
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, lockObject, 0)[0x8C / 4])(lockObject, nullptr, 50, 0, -1, 0);

    CSF_AT(int, self, 0x18) = 3;

    return MpSpawnPoints_UpdateRelatedObject(self, nullptr);
}

HOOK(0x004E2070, C_0083e5e8_AcquireAndSetStateThenUpdate, "auto");
