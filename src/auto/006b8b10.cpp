#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"

// S_00402350 is a 0x3C-byte record. The original builds it on its own stack frame
// and passes its address as `this`, so it is reserved as raw bytes here.
CSF_FUNC(uint32_t*, __fastcall, InitS00402350Object, 0x00402350, (void* self, void* edx));

static void* __fastcall FxParticleManager_GetAttachmentTransform(void* self, void* /*edx*/, void* a2)
{
    // The S_00402350 vtable is referenced by the original's construction path.
    // Code-address annotation: function value, not a vtable.
    const void* s00402350Vtable = reinterpret_cast<void**>(CSF_ADDR_FN(0x00402350));
    (void)s00402350Vtable;

    // Stack object for S_00402350: room 0x3C bytes up to the frame end.
    alignas(8) uint8_t transformStorage[0x3C];

    uint32_t* transform = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x14));
    if (!transform)
        transform = InitS00402350Object(transformStorage, nullptr);

    memcpy(a2, transform, 0x3C);
    return a2;
}

HOOK(0x006B8B10, FxParticleManager_GetAttachmentTransform, "auto");
