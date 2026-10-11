#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Reads the signed value at +8 of the referenced record. A non-negative value
// is resolved through the current scene's handle table; if that yields an entry,
// the entry's field at +0x50 is passed to the object's virtual slot 0xB0.
// Otherwise the non-negative flag itself is returned.
static char __fastcall C_00831ac0_ResolveHandleAndDispatch(C_00831ac0* self, void* /*edx*/, void* handleRef)
{
    const int handleValue = CSF_AT(int, handleRef, 0x08);
    const char isNonNegative = (handleValue >= 0) ? 1 : 0;

    if (isNonNegative) {
        const int resolved = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(handleRef)));
        if (resolved != 0) {
            uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(resolved));
            const int entryParam = CSF_AT(int, entry, 0x50);
            return ((char(__fastcall*)(void*, void*, int))self->vtbl[0xB0 / 4])(self, nullptr, entryParam);
        }
    }

    return isNonNegative;
}

HOOK(0x0047AE20, C_00831ac0_ResolveHandleAndDispatch, "auto");
