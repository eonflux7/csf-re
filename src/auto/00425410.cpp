#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, SceneList_AppendIdIfFound, 0x0041ECE0, (unsigned int a2));
CSF_FUNC(int, __fastcall, C_0086c12c_ReleaseOwnedBuffer, 0x0075A190, (C_0086c12c* self, void*));
CSF_FUNC(int, __fastcall, C_0082e598_InstallVtablesAndInitBase, 0x0041E210, (C_0082e598* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Tears down a C_0082fcd8 object: drops the scene-list registration, releases the
// owned buffer, reinstalls base vtables, and optionally frees the block (bit 0 of a2).
static int* __fastcall C_0082fcd8_Destroy(C_0082fcd8* self, void* /*edx*/, char a2)
{
    uint32_t sceneListId = CSF_AT(uint32_t, self, 0x44);

    self->vtbl = CSF_VTBL(0x0082FCD8);
    self->field_0004 = (uint32_t)CSF_VTBL(0x0082FCB8);

    if (sceneListId) {
        SceneList_AppendIdIfFound(sceneListId);
        CSF_AT(uint32_t, self, 0x44) = 0;
    }

    C_0086c12c_ReleaseOwnedBuffer(
        reinterpret_cast<C_0086c12c*>(reinterpret_cast<uint8_t*>(self) + 0x24), nullptr);
    C_0082e598_InstallVtablesAndInitBase(reinterpret_cast<C_0082e598*>(self), nullptr);

    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }
    return reinterpret_cast<int*>(self);
}

HOOK(0x425410, C_0082fcd8_Destroy, "auto");
