#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxObject_ConstructEmbeddedState, 0x006BFD00, (C_00860700* self, void*));
CSF_FUNC(char, __fastcall, GfxFrame_InitRotationDegreesFromMatrix, 0x006BF380, (void* self, void* , int matrixBase));
CSF_FUNC(int, __fastcall, C_00860700_FillRangeHeader, 0x006BF950, (C_00860700* self, void* , int out));
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (C_00860630* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// Copies the 0x40-byte atomic matrix block of this instance's embedded frame into *arg0.
static void __fastcall GfxObjectInstance_CopyAtomicMatrixOut(std::uint32_t* self, void* /*edx*/, void** arg0)
{
    // Embedded state object lives in the 0x20-byte stack region below the out buffer.
    alignas(8) std::uint8_t embeddedStateBytes[0x20];
    C_00860700& embeddedState = *reinterpret_cast<C_00860700*>(embeddedStateBytes);

    // 8-byte out buffer: [0] = matrix handle (a2), [4] = ownership flag (v5).
    alignas(8) std::uint8_t outBytes[8];

    GfxObject_ConstructEmbeddedState(&embeddedState, nullptr);

    // Owner pointer at self+0xC; its dword at +4 is the matrix base.
    std::uint32_t owner = CSF_AT(std::uint32_t, self, 0x0C);
    std::uint32_t matrixBase = CSF_AT(std::uint32_t, reinterpret_cast<void*>(owner), 4);
    GfxFrame_InitRotationDegreesFromMatrix(&embeddedState, nullptr, static_cast<int>(matrixBase));

    int headerPtr = C_00860700_FillRangeHeader(&embeddedState, nullptr, static_cast<int>(reinterpret_cast<std::uintptr_t>(outBytes)));
    const void* source = *reinterpret_cast<const void* const*>(static_cast<std::uintptr_t>(headerPtr));
    void* destination = *arg0;
    std::memcpy(destination, source, 0x40);

    std::uint8_t ownsMatrix = outBytes[4];
    if (ownsMatrix) {
        std::uint32_t matrixHandle = CSF_AT(std::uint32_t, outBytes, 0);
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixHandle));
    }

    GfxObject_DestroyEmbeddedState(reinterpret_cast<C_00860630*>(&embeddedState), nullptr);
}

HOOK(0x006E5EB0, GfxObjectInstance_CopyAtomicMatrixOut, "auto");
