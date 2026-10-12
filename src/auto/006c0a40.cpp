#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S00402350_InitTransform, 0x00402350, (S_00402350* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, S_006abb80_InitializeBounds, 0x006ABB80, (S_006abb80* self, void* , float xFirst, float xSecond, float yFirst, float ySecond, float zFirst, float zSecond));
CSF_FUNC(std::uint32_t*, __fastcall, GfxMatrix_TransformBasisAndPosition, 0x006ABF10, (std::uint32_t* self, void* , std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, GetBit2OfField4, 0x006B8170, (void* self, void*));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(void, __fastcall, GfxFrames_DestroyEmbeddedState, 0x006BF180, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxFrames_SetEmbeddedFrame, 0x006BF380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GfxFrames_GetEmbeddedMatrix, 0x006BF970, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxFrames_ConstructEmbeddedState, 0x006BFD00, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, SceneObject_GetWorldBounds, 0x006C0570, (uint32_t* a1, int arg4, int a7));
CSF_FUNC(char, __fastcall, GfxFrames_GrowBoundsArray, 0x006C08E0, (void* self, void* edx, int a2));

static int __cdecl GfxFrames_AppendObjectBounds(int object, int collectionAddress)
{
    void* attachment =
        reinterpret_cast<void*>(GfxObjectProps_GetObjectAttachment(object));

    if (attachment &&
        static_cast<std::uint8_t>(GetBit2OfField4(attachment, nullptr)) != 0)
        return object;

    void* collection = reinterpret_cast<void*>(collectionAddress);
    alignas(8) std::uint8_t transformBytes[0x3C];
    alignas(8) std::uint8_t stateBytes[0x20];
    alignas(8) std::uint8_t boundsBytes[0x18];
    alignas(8) std::uint8_t matrixResultBytes[0x8];

    SceneObject_GetWorldBounds(
        reinterpret_cast<uint32_t*>(boundsBytes),
        object,
        CSF_AT(std::uint8_t, collection, 0x10));

    // These identities belong to the two callee-owned initialization steps.
    (void)CSF_VTBL(0x00402350);
    S00402350_InitTransform(
        reinterpret_cast<S_00402350*>(transformBytes), nullptr);

    (void)CSF_VTBL(0x006ABB80);
    S_006abb80_InitializeBounds(
        reinterpret_cast<S_006abb80*>(transformBytes), nullptr,
        CSF_AT(float, boundsBytes, 0x00),
        CSF_AT(float, boundsBytes, 0x0C),
        CSF_AT(float, boundsBytes, 0x04),
        CSF_AT(float, boundsBytes, 0x10),
        CSF_AT(float, boundsBytes, 0x08),
        CSF_AT(float, boundsBytes, 0x14));

    GfxFrames_ConstructEmbeddedState(stateBytes, nullptr);
    GfxFrames_SetEmbeddedFrame(
        stateBytes, nullptr,
        CSF_AT(int, reinterpret_cast<void*>(object), 0x04));

    void* matrix = GfxFrames_GetEmbeddedMatrix(
        stateBytes, nullptr, reinterpret_cast<int>(matrixResultBytes));
    GfxMatrix_TransformBasisAndPosition(
        reinterpret_cast<std::uint32_t*>(transformBytes), nullptr,
        reinterpret_cast<std::uint32_t*>(matrix));

    if (CSF_AT(std::uint8_t, matrixResultBytes, 0x04))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixResultBytes, 0x00));

    if (CSF_AT(std::uint32_t, collection, 0x08) >=
        CSF_AT(std::uint32_t, collection, 0x04))
    {
        GfxFrames_GrowBoundsArray(
            collection, nullptr, CSF_AT(int, collection, 0x0C));
    }

    const std::uint32_t index = CSF_AT(std::uint32_t, collection, 0x08);
    std::uint8_t* destination =
        CSF_AT(std::uint8_t*, collection, 0x00) + 60 * index;
    CSF_AT(std::uint32_t, collection, 0x08) = index + 1;
    std::memcpy(destination, transformBytes, 0x3C);

    GfxFrames_DestroyEmbeddedState(stateBytes, nullptr);
    return object;
}

HOOK(0x006C0A40, GfxFrames_AppendObjectBounds, "auto");
