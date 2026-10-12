#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Degrees-to-radians factor (0.017453292f) stored in the original's data section.
CSF_GLOBAL(float, g_DegreesToRadians, 0x00860620);

// RwFrame update: takes the frame's object pointer and refreshes the objects attached to it.
CSF_FUNC(int, __cdecl, RwFrameUpdateObjects, 0x006236A0, (int a1));

// RwMatrix destroy: releases the matrix at the given address.
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// Copies a 3-dword translation into the matrix and updates it; self is the 8-byte transform object.
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void*, const uint32_t* src));

// Sets the rotation (radians) on the transform object.
CSF_FUNC(int, __fastcall, S_006ada10_SetRotation, 0x006AE2F0, (S_006ada10* self, void*, float angleX, float angleY, float angleZ));

// Applies the frame's Euler rotation (degrees in fields 0x14/0x18/0x1C), copies the frame's
// translation, pushes both into the frame matrix, updates the frame objects, and destroys
// the matrix if the transform object's flag byte was set by the rotation call.
static char __fastcall C_00860700_ApplyRotationAndUpdate(C_00860700* self, void* /*edx*/)
{
    // Transform object lives in an 8-byte stack buffer: +0 = matrix pointer, +4 = flag byte.
    alignas(8) uint8_t transformBytes[8];
    std::memset(transformBytes, 0, sizeof(transformBytes));

    // Matrix base pointer is the frame's field_000c + 0x10.
    const uint32_t frameMatrixAddr = self->field_000c + 16;
    CSF_AT(uint32_t, transformBytes, 0) = frameMatrixAddr;

    // Translation copy (three dwords taken from the matrix at +0x40 relative to field_000c).
    uint8_t* frameMatrix = reinterpret_cast<uint8_t*>(frameMatrixAddr);
    uint32_t translation[3];
    translation[0] = CSF_AT(uint32_t, frameMatrix, 0x30);
    translation[1] = CSF_AT(uint32_t, frameMatrix, 0x34);
    translation[2] = CSF_AT(uint32_t, frameMatrix, 0x38);

    // Angles converted from degrees to radians.
    const float angleX = self->field_0014 * g_DegreesToRadians;
    const float angleY = self->field_0018 * g_DegreesToRadians;
    const float angleZ = self->field_001c * g_DegreesToRadians;

    S_006ada10_SetRotation(reinterpret_cast<S_006ada10*>(transformBytes), nullptr, angleX, angleY, angleZ);
    GfxMatrix_CopyTranslationAndUpdate(transformBytes, nullptr, translation);
    RwFrameUpdateObjects(static_cast<int>(self->field_000c));

    // Flag byte is re-read after the calls; it may have been changed by the callees.
    const char flagResult = CSF_AT(char, transformBytes, 4);
    if (flagResult)
    {
        return static_cast<char>(RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(CSF_AT(uint32_t, transformBytes, 0))));
    }
    return flagResult;
}

HOOK(0x6BFC70, C_00860700_ApplyRotationAndUpdate, "auto");
