#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));

// Builds a translated copy of this object's matrix into the caller's 8-byte
// output record { matrix pointer, owned flag }. Returns the output record.
static int __fastcall C_008622e8_CaptureTranslatedMatrix(C_008622e8* self, void* /*edx*/, int outRecord)
{
    // 8-byte temporary on the stack: { matrix pointer, flag byte }
    alignas(8) std::uint8_t tempBytes[8];

    std::uint32_t* sourceMatrix = RwMatrixCreate();
    CSF_AT(std::uint32_t*, tempBytes, 0) = sourceMatrix;
    CSF_AT(std::uint8_t, tempBytes, 4) = 1;

    const uint32_t* translationSrc =
        reinterpret_cast<const uint32_t*>(reinterpret_cast<std::uint8_t*>(self) + 0x28);
    GfxMatrix_CopyTranslationAndUpdate(tempBytes, nullptr, translationSrc);

    std::uint8_t* out = reinterpret_cast<std::uint8_t*>(outRecord);

    std::uint32_t* copiedMatrix = RwMatrixCreate();
    CSF_AT(std::uint32_t*, out, 0) = copiedMatrix;
    std::memcpy(copiedMatrix, CSF_AT(std::uint32_t*, tempBytes, 0), 0x40);

    CSF_AT(std::uint8_t, out, 4) = 1;

    if (CSF_AT(std::uint8_t, tempBytes, 4) != 0) {
        RwMatrixDestroy(CSF_AT(std::uint32_t*, tempBytes, 0));
    }

    return outRecord;
}

HOOK(0x006F0750, C_008622e8_CaptureTranslatedMatrix, "auto");
