#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Binds to the original callees by address (signatures copied from the packet).
CSF_FUNC(int, __fastcall, GfxObject_ConstructEmbeddedState, 0x006BFD00, (C_00860700* self, void*));
CSF_FUNC(char, __fastcall, GfxFrame_InitRotationDegreesFromMatrix, 0x006BF380, (void* self, void*, int matrixBase));
CSF_FUNC(char, __fastcall, C_00860700_UpdateFrameMatrixAndDestroyIfFlagged, 0x006BF400, (C_00860700* self, void*, uint32_t* a2));
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (C_00860630* self, void*));

// Copies the 3-dword source transform into this+0x28..+0x30, then refreshes the
// frame through a temporary embedded state when field_0024 is set; otherwise
// forwards the source to the child object's vtable slot 14 (+0x38).
static void __fastcall C_008622e8_CopyPositionAndRefreshFrame(C_008622e8* self, void* /*edx*/, uint32_t* a2)
{
    CSF_AT(uint32_t, self, 0x28) = a2[0];
    CSF_AT(uint32_t, self, 0x2C) = a2[1];
    CSF_AT(uint32_t, self, 0x30) = a2[2];

    if (self->field_0024 != 0)
    {
        // field_0024 points at an object whose dword at +4 is the matrix base.
        uint32_t matrixBase = CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0024), 0x4);

        alignas(8) uint8_t embeddedStateBytes[0x20];
        C_00860700& embeddedState = *reinterpret_cast<C_00860700*>(embeddedStateBytes);

        GfxObject_ConstructEmbeddedState(&embeddedState, nullptr);
        GfxFrame_InitRotationDegreesFromMatrix(&embeddedState, nullptr, static_cast<int>(matrixBase));
        C_00860700_UpdateFrameMatrixAndDestroyIfFlagged(&embeddedState, nullptr, a2);
        GfxObject_DestroyEmbeddedState(reinterpret_cast<C_00860630*>(&embeddedState), nullptr);
    }
    else
    {
        void* child = reinterpret_cast<void*>(self->field_0020);
        ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, child, 0)[0x38 / 4])(child, nullptr, a2);
    }
}

HOOK(0x006F04D0, C_008622e8_CopyPositionAndRefreshFrame, "auto");
