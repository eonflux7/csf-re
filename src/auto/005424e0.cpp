#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Vtable of CGuiCharacterPreview (also the first dword written into the object).
CSF_FUNC(int, __fastcall, CGuiCharacterPreview_ReleaseResources, 0x541ED0, (void* self, void*));
CSF_FUNC(int, __fastcall, CGuiControl_Destroy, 0x719B10, (void* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* p));

// Scalar deleting destructor: restores the CGuiCharacterPreview vtable, releases the
// preview resources, destroys the CGuiControl base and frees the object when the
// low bit of the flag argument is set.
static int* __fastcall CGuiCharacterPreview_DestroyAndFreeIfFlag(void* self, void* /*edx*/, char a2)
{
    *reinterpret_cast<void**>(self) = reinterpret_cast<void*>(CSF_VTBL(0x8432B0));

    CGuiCharacterPreview_ReleaseResources(self, nullptr);
    CGuiControl_Destroy(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return static_cast<int*>(self);
}

HOOK(0x5424E0, CGuiCharacterPreview_DestroyAndFreeIfFlag, "auto");
