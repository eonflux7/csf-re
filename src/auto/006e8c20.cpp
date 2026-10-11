#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, GfxHAnim_ResolveNodeIndex, 0x006E8120, (int a1, int a2));
CSF_FUNC(int, __cdecl, GfxHAnim_ResolveRelatedNodeIndex, 0x006E82F0, (int a1, int a2));
CSF_FUNC(char, __fastcall, GfxHAnim_UpdateRelativeTransform, 0x00707BC0, (void* self, void* edx, void** a3, void** a4));

static char __fastcall C_00861e98_UpdateNodeRelativeTransform(
    C_00861e98* self, void* /*edx*/, int nodeId)
{
    if (!self->field_001c)
        return 0;

    const int nodeIndex =
        GfxHAnim_ResolveNodeIndex(static_cast<int>(self->field_0014), nodeId);
    if (nodeIndex < 0)
        return static_cast<char>(nodeIndex);

    const int relatedIndex =
        GfxHAnim_ResolveRelatedNodeIndex(static_cast<int>(self->field_0014), nodeIndex);
    if (relatedIndex < 0)
        return static_cast<char>(relatedIndex);

    void* node = reinterpret_cast<void*>(self->vSlot6C(nodeIndex));
    void* relatedNode = reinterpret_cast<void*>(self->vSlot6C(relatedIndex));

    alignas(8) std::uint8_t outputMatrixBytes[0x8];
    alignas(8) std::uint8_t inputMatrixBytes[0x8];

    CSF_AT(std::uint32_t*, inputMatrixBytes, 0) =
        reinterpret_cast<std::uint32_t**>(self->field_0050)[relatedIndex];
    CSF_AT(std::uint8_t, inputMatrixBytes, 4) = 0;

    // The saved input pointer is an argument to the following ret-8 call,
    // not a second argument to this virtual getter.
    void** matrix = reinterpret_cast<void**(__fastcall*)(void*, void*, void**)>(
        CSF_AT(void**, relatedNode, 0)[0x1c / 4])(
            relatedNode, nullptr, reinterpret_cast<void**>(outputMatrixBytes));

    GfxHAnim_UpdateRelativeTransform(
        node, nullptr, matrix, reinterpret_cast<void**>(inputMatrixBytes));

    if (CSF_AT(std::uint8_t, outputMatrixBytes, 4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, outputMatrixBytes, 0));

    const char ownsInput = static_cast<char>(
        CSF_AT(std::uint8_t, inputMatrixBytes, 4));
    if (ownsInput)
        return static_cast<char>(
            RwMatrixDestroy(CSF_AT(std::uint32_t*, inputMatrixBytes, 0)));

    return ownsInput;
}

HOOK(0x006E8C20, C_00861e98_UpdateNodeRelativeTransform, "auto");
