#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, DynamicObjects_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (C_00860630* self, void*));
CSF_FUNC(char, __fastcall, GfxFrame_InitRotationDegreesFromMatrix, 0x006BF380, (void* self, void* , int matrixBase));
CSF_FUNC(char, __fastcall, C_00860700_SetMatrixAndUpdateEulerDegrees, 0x006BF9C0, (C_00860700* self, void* , const void** a2));
CSF_FUNC(int, __fastcall, GfxObject_ConstructEmbeddedState, 0x006BFD00, (C_00860700* self, void*));
CSF_FUNC(int, __fastcall, GfxFrames_UpdateFrameRecordState, 0x006BFFC0, (void* self, void* , void* rec));
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));
CSF_FUNC(int, __fastcall, C_008670f0_SetEntry, 0x006C4B00, (C_008670f0* self, void* , int key, int value));
CSF_FUNC(void*, __fastcall, C_00861bd8_InitWithSkinFlags, 0x006E5210, (C_00861bd8* self, void* , int** a2, char a3));
CSF_FUNC(void, __fastcall, GfxObjectInstance_CopyAtomicMatrixOut, 0x006E5EB0, (std::uint32_t* self, void* , void** arg0));

static int __fastcall GfxObjectInstances_SerializeInstance(
    void* self, void* /*edx*/, void* stream)
{
    bool beginFailed = false;
    if (!CSF_AT(bool, stream, 0))
    {
        void* serializer = CSF_AT(void*, stream, 0x04);
        beginFailed =
            !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
                CSF_AT(void**, serializer, 0)[0x80 / 4])(
                    serializer, nullptr, 0, stream);
    }

    const std::uint32_t savedState = CSF_AT(std::uint32_t, stream, 0x1C);
    CSF_AT(bool, stream, 0) = beginFailed;
    CSF_AT(std::uint32_t, stream, 0x1C) = 0;
    if (!beginFailed)
    {
        void* serializer = CSF_AT(void*, stream, 0x04);
        CSF_AT(std::uint8_t, stream, 0x15) = 0;
        CSF_AT(std::uint8_t, stream, 0x16) = 0;
        CSF_AT(bool, stream, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x44 / 4])(
                    serializer, nullptr,
                    static_cast<std::uint8_t*>(self) + 0x11, stream) == -1;
    }

    alignas(8) std::uint8_t matrixRecordBytes[0x14];
    CSF_AT(void*, matrixRecordBytes, 0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, matrixRecordBytes, 0x04) = 1;

    if (CSF_AT(std::uint8_t, stream, 0x14))
    {
        GfxObjectInstance_CopyAtomicMatrixOut(
            reinterpret_cast<std::uint32_t*>(
                static_cast<std::uint8_t*>(self) - 8),
            nullptr, reinterpret_cast<void**>(matrixRecordBytes));
    }

    bool canSerialize = !CSF_AT(bool, stream, 0);
    CSF_AT(std::uint32_t, stream, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(std::uint8_t, stream, 0x15) = 0;
        CSF_AT(std::uint8_t, stream, 0x16) = 0;
        GfxFrames_UpdateFrameRecordState(matrixRecordBytes, nullptr, stream);
    }

    void* entryOwner = CSF_AT(void*, self, 0x38);
    int** entries =
        reinterpret_cast<int**(__fastcall*)(void*, void*)>(
            CSF_AT(void**, entryOwner, 0)[0x1C / 4])(
                entryOwner, nullptr);
    void** payloadEntry = reinterpret_cast<void**>(
        C_0085e7b8_FindEntryPayloadByKey(
            reinterpret_cast<C_0085e7b8*>(entries), nullptr, 5));
    std::uint8_t hasPayload = payloadEntry && *payloadEntry ? 1 : 0;

    canSerialize = !CSF_AT(bool, stream, 0);
    CSF_AT(std::uint32_t, stream, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, stream, 0x04);
        CSF_AT(std::uint8_t, stream, 0x15) = 0;
        CSF_AT(std::uint8_t, stream, 0x16) = 0;
        CSF_AT(bool, stream, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x44 / 4])(
                    serializer, nullptr, &hasPayload, stream) == -1;
    }

    if (hasPayload)
    {
        alignas(8) std::uint8_t skinFlagBytes[0x01] = {};
        if (CSF_AT(std::uint8_t, stream, 0x14))
        {
            void* atomic = CSF_AT(void*, self, 0x08);
            const std::uint32_t* flags =
                reinterpret_cast<const std::uint32_t*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, atomic, 0)[0x20 / 4])(atomic, nullptr);
            skinFlagBytes[0] = (*flags & 0x800) == 0x800;
        }

        canSerialize = !CSF_AT(bool, stream, 0);
        CSF_AT(std::uint32_t, stream, 0x1C) = 0;
        if (canSerialize)
        {
            void* serializer = CSF_AT(void*, stream, 0x04);
            CSF_AT(std::uint8_t, stream, 0x15) = 0;
            CSF_AT(std::uint8_t, stream, 0x16) = 0;
            CSF_AT(bool, stream, 0) =
                reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                    CSF_AT(void**, serializer, 0)[0x44 / 4])(
                        serializer, nullptr, skinFlagBytes, stream) == -1;
        }

        void* payload = payloadEntry ? *payloadEntry : nullptr;
        alignas(8) std::uint8_t referenceBytes[0x07];
        CSF_AT(void*, referenceBytes, 0) = payload;

        if (!CSF_AT(std::uint8_t, stream, 0x14) &&
            !CSF_AT(bool, stream, 0) && !payload)
        {
            auto* allocation = reinterpret_cast<C_00861bd8*>(
                CoreMemory_AllocUntagged(20));
            void* newPayload = allocation
                ? C_00861bd8_InitWithSkinFlags(allocation, nullptr, entries, 0)
                : nullptr;
            CSF_AT(void*, referenceBytes, 0) = newPayload;
            C_008670f0_SetEntry(
                reinterpret_cast<C_008670f0*>(entries), nullptr, 5,
                reinterpret_cast<int>(newPayload));
        }

        canSerialize = !CSF_AT(bool, stream, 0);
        CSF_AT(std::uint32_t, stream, 0x1C) = 0;
        if (canSerialize)
        {
            CSF_AT(std::uint8_t, stream, 0x15) = 1;
            CSF_AT(std::uint8_t, stream, 0x16) = 0;
            DynamicObjects_SerializeReference(
                referenceBytes, reinterpret_cast<bool*>(stream));
        }
    }

    if (!CSF_AT(std::uint8_t, stream, 0x14) && !CSF_AT(bool, stream, 0))
    {
        alignas(8) std::uint8_t embeddedStateBytes[0x20];
        auto& embeddedState =
            *reinterpret_cast<C_00860700*>(embeddedStateBytes);
        GfxObject_ConstructEmbeddedState(&embeddedState, nullptr);
        void* frame = CSF_AT(void*, self, 0x04);
        GfxFrame_InitRotationDegreesFromMatrix(
            &embeddedState, nullptr, CSF_AT(int, frame, 0x04));
        C_00860700_SetMatrixAndUpdateEulerDegrees(
            &embeddedState, nullptr,
            reinterpret_cast<const void**>(matrixRecordBytes));
        GfxObject_DestroyEmbeddedState(
            reinterpret_cast<C_00860630*>(embeddedStateBytes), nullptr);
    }

    if (CSF_AT(std::uint8_t, matrixRecordBytes, 0x04))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixRecordBytes, 0));

    const bool canFinish = !CSF_AT(bool, stream, 0);
    CSF_AT(std::uint32_t, stream, 0x1C) = savedState;
    if (canFinish)
    {
        void* serializer = CSF_AT(void*, stream, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x7C / 4])(
                    serializer, nullptr, stream);
        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(bool, stream, 0) = false;
            return result;
        }
    }

    CSF_AT(bool, stream, 0) = true;
    return 1;
}

HOOK(0x006E64D0, GfxObjectInstances_SerializeInstance, "auto");
