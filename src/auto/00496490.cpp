#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, EulerAnglesToDirection, 0x006AE640, (void* self, void* edx, float* a2));

// Temporary matrix handle returned by the "get matrix" virtual (slot 0x8C):
// a matrix pointer plus a flag telling the caller it must destroy it.
struct S_MatrixRef
{
    std::uint32_t* matrix;
    char owned;
};

// The original keeps these temporaries overlapped on its stack: the direction
// result (12 bytes starting at dirOut) overlaps the 8-byte scratch after it.
struct S_OrientationScratch
{
    std::uint8_t angles[12];   // ebp-0x18
    std::uint32_t dirOut;      // ebp-0x0C (direction vector starts here)
    std::uint32_t scratch[2];  // ebp-0x08
};

// Returns the position and view direction used for aiming: from the active camera
// object when flag 0x2000 is set (and not overridden), otherwise from the entity itself.
static char __fastcall C_00833ee8_GetViewPositionAndDirection(C_00833ee8* self, void* /*edx*/,
                                                              std::uint32_t* outPosition,
                                                              std::uint32_t* outDirection)
{
    if ((self->field_02e0 & 0x2000) == 0 || self->field_0228 != 0)
    {
        S_OrientationScratch tmp;
        std::memset(&tmp, 0, sizeof(tmp));

        std::uint32_t* position =
            ((std::uint32_t*(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);
        outPosition[0] = position[0];
        outPosition[1] = position[1];
        outPosition[2] = position[2];

        void* orientation =
            ((void*(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, self, 0)[0xAC / 4])(
                self, nullptr, tmp.scratch, tmp.angles);
        std::uint32_t* direction = reinterpret_cast<std::uint32_t*>(
            EulerAnglesToDirection(orientation, nullptr, reinterpret_cast<float*>(&tmp.dirOut)));
        outDirection[0] = direction[0];
        outDirection[1] = direction[1];
        std::uint32_t lastComponent = direction[2];
        outDirection[2] = lastComponent;
        return static_cast<char>(lastComponent);
    }

    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
    void* referenced = GetReferencedObject(provider, nullptr);

    void* viewObject =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, referenced, 0)[0x10 / 4])(referenced, nullptr);

    std::uint32_t* position =
        ((std::uint32_t*(__fastcall*)(void*, void*))CSF_AT(void**, viewObject, 0)[0x10 / 4])(viewObject, nullptr);
    outPosition[0] = position[0];
    outPosition[1] = position[1];
    outPosition[2] = position[2];

    S_MatrixRef matrixRef;
    matrixRef.matrix = nullptr;
    matrixRef.owned = 0;

    S_MatrixRef* result =
        ((S_MatrixRef*(__fastcall*)(void*, void*, S_MatrixRef*))CSF_AT(void**, viewObject, 0)[0x8C / 4])(
            viewObject, nullptr, &matrixRef);

    std::uint8_t* matrixBytes = reinterpret_cast<std::uint8_t*>(result->matrix);
    outDirection[0] = CSF_AT(std::uint32_t, matrixBytes, 0x20);
    outDirection[1] = CSF_AT(std::uint32_t, matrixBytes, 0x24);
    char owned = matrixRef.owned;
    outDirection[2] = CSF_AT(std::uint32_t, matrixBytes, 0x28);

    if (owned != 0)
        return static_cast<char>(RwMatrixDestroy(matrixRef.matrix));
    return owned;
}

HOOK(0x00496490, C_00833ee8_GetViewPositionAndDirection, "auto");
