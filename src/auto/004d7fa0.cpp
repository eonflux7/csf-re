#include <cstdint>
#include "globals.h"
#include "hook.h"

// C_0082f9f8::TryAcquire (0x48E5D0): __fastcall, three stack args, callee pops (ret 0Ch).
CSF_FUNC(void, __fastcall, C_0082f9f8_TryAcquire, 0x48E5D0, (void* self, void* edx, void* owner, int flagA, int flagB));

// Sets bit 0 in the owner's state word, acquires the owner's sub-resource, then
// forwards (arg0, a3, -1, 0) to virtual slot 36 (0x90) of that sub-resource.
static int __fastcall AutonomoMarkBusyAndDispatch(void* self, void* /*edx*/, int arg0, int a3)
{
    // *(*(self + 8) + 0x18) |= 1
    CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 8), 0x18) |= 1u;

    // C_0082f9f8_TryAcquire(*(self+8)+0x14, self, 0, 1)
    void* resource = CSF_AT(void*, CSF_AT(uint8_t*, self, 8), 0x14);
    C_0082f9f8_TryAcquire(resource, nullptr, self, 0, 1);

    // Re-read the resource pointer after the call, as the original does.
    void* resourceAfter = CSF_AT(void*, CSF_AT(uint8_t*, self, 8), 0x14);
    return ((int(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, resourceAfter, 0)[0x90 / 4])(
        resourceAfter, nullptr, arg0, a3, -1, 0);
}

HOOK(0x4D7FA0, AutonomoMarkBusyAndDispatch, "auto");
