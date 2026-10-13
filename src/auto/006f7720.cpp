#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(void, __fastcall, C_008670f0_RemoveMatchingEntry, 0x006C4630, (C_008670f0* self, void* , int key));
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));

static void __cdecl GfxObjectProps_ReleaseAttachedMatrixPayload(int objectIndex)
{
    const int attachmentAddress = GfxObjectProps_GetObjectAttachment(objectIndex);
    void* object = nullptr;
    if (attachmentAddress)
        object = CSF_AT(void*, reinterpret_cast<void*>(attachmentAddress), 8);

    if (!object)
        return;

    // The getter takes no stack arguments. The preceding push of 7
    // belongs to FindEntryPayloadByKey, which consumes it.
    auto* table = static_cast<C_0085e7b8*>(
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr));

    const int payloadAddress =
        C_0085e7b8_FindEntryPayloadByKey(table, nullptr, 7);
    if (!payloadAddress)
        return;

    void* block = CSF_AT(void*, reinterpret_cast<void*>(payloadAddress), 0);
    if (block)
    {
        if (CSF_AT(std::uint8_t, block, 8))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, block, 4));
        CoreMemory_Free(block);
    }

    auto* removalTable = static_cast<C_008670f0*>(
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr));
    C_008670f0_RemoveMatchingEntry(removalTable, nullptr, 7);
}

HOOK(0x006F7720, GfxObjectProps_ReleaseAttachedMatrixPayload, "auto");
