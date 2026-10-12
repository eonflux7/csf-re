#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxFrames_UpdateTraversalMatrix, 0x006BFFC0,
         (void* self, void* edx, int a2));

static int __fastcall GfxFrames_ProcessTransformTraversal(
    void* self, void* /*edx*/, void* traversal)
{
    bool blocked = false;
    if (CSF_AT(std::uint8_t, traversal, 0) == 0)
    {
        void* visitor = CSF_AT(void*, traversal, 0x4);
        blocked = !((bool(__fastcall*)(void*, void*, int, void*))
                        CSF_AT(void**, visitor, 0)[0x80 / 4])(
            visitor, nullptr, 0, traversal);
    }

    const std::uint32_t savedState =
        CSF_AT(std::uint32_t, traversal, 0x1C);
    CSF_AT(std::uint8_t, traversal, 0) = blocked;

    alignas(8) std::uint8_t matrixBytes[0xC];
    CSF_AT(std::uint32_t*, matrixBytes, 0) = RwMatrixCreate();
    const std::uint8_t useCurrentTransform =
        CSF_AT(std::uint8_t, traversal, 0x14);
    CSF_AT(std::uint8_t, matrixBytes, 0x4) = 1;

    if (useCurrentTransform)
    {
        alignas(8) std::uint8_t currentMatrixBytes[0x8];
        void* frame = static_cast<std::uint8_t*>(self) - 8;
        void* result =
            ((void*(__fastcall*)(void*, void*, void*))
                 CSF_AT(void**, frame, 0)[0x90 / 4])(
                frame, nullptr, currentMatrixBytes);

        std::memcpy(CSF_AT(void*, matrixBytes, 0),
                    CSF_AT(const void*, result, 0), 0x40);
        if (CSF_AT(std::uint8_t, currentMatrixBytes, 0x4))
            RwMatrixDestroy(
                CSF_AT(std::uint32_t*, currentMatrixBytes, 0));
    }

    const bool shouldTraverse =
        CSF_AT(std::uint8_t, traversal, 0) == 0;
    CSF_AT(std::uint32_t, traversal, 0x1C) = 0;
    if (shouldTraverse)
    {
        CSF_AT(std::uint8_t, traversal, 0x15) = 0;
        CSF_AT(std::uint8_t, traversal, 0x16) = 0;
        GfxFrames_UpdateTraversalMatrix(
            matrixBytes, nullptr,
            static_cast<int>(reinterpret_cast<std::intptr_t>(traversal)));
    }

    if (!CSF_AT(std::uint8_t, traversal, 0x14) &&
        !CSF_AT(std::uint8_t, traversal, 0))
    {
        void* frame = static_cast<std::uint8_t*>(self) - 8;
        ((void(__fastcall*)(void*, void*, void*))
             CSF_AT(void**, frame, 0)[0x98 / 4])(
            frame, nullptr, matrixBytes);
        ((void(__fastcall*)(void*, void*))
             CSF_AT(void**, frame, 0)[0x48 / 4])(frame, nullptr);
    }

    if (CSF_AT(std::uint8_t, matrixBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));

    const bool shouldFinish =
        CSF_AT(std::uint8_t, traversal, 0) == 0;
    CSF_AT(std::uint32_t, traversal, 0x1C) = savedState;
    if (shouldFinish)
    {
        void* visitor = CSF_AT(void*, traversal, 0x4);
        if (((bool(__fastcall*)(void*, void*, void*))
                 CSF_AT(void**, visitor, 0)[0x7C / 4])(
                visitor, nullptr, traversal))
        {
            CSF_AT(std::uint8_t, traversal, 0) = 0;
            return 0;
        }
    }

    CSF_AT(std::uint8_t, traversal, 0) = 1;
    return 1;
}

HOOK(0x006C0050, GfxFrames_ProcessTransformTraversal, "auto");
