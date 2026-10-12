#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (C_00860630* self, void*));
CSF_FUNC(char, __fastcall, GfxFrame_InitRotationDegreesFromMatrix, 0x006BF380, (void* self, void* , int matrixBase));
CSF_FUNC(char, __fastcall, C_00860700_SetMatrixAndUpdateEulerDegrees, 0x006BF9C0, (C_00860700* self, void* , const void** a2));
CSF_FUNC(int, __fastcall, GfxObject_ConstructEmbeddedState, 0x006BFD00, (C_00860700* self, void*));
CSF_FUNC(int, __fastcall, GfxFrames_UpdateFrameRecordState, 0x006BFFC0, (void* self, void* , void* rec));
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));
CSF_FUNC(int, __fastcall, C_008670f0_SetEntry, 0x006C4B00, (C_008670f0* self, void* , int key, int value));
CSF_FUNC(void*, __fastcall, GfxObjectInstances_ConstructEntryPayload, 0x006E5210, (void* self, void* edx, void** a2, char a3));
CSF_FUNC(void, __fastcall, GfxObjectInstances_GetMatrix, 0x006E5EB0, (void* self, void* edx, void** arg0));

static int __fastcall GfxObjectInstances_SerializeInstance(
    void* self, void* /*edx*/, void* record)
{
    alignas(8) std::uint8_t stateBytes[0x20];
    alignas(8) std::uint8_t matrixRecordBytes[0x14];
    alignas(8) std::uint8_t referenceBytes[0x7];
    alignas(8) std::uint8_t flagBytes[0x1];

    CSF_AT(void*, matrixRecordBytes, 0xC) = self;

    bool failed = false;
    if (CSF_AT(std::uint8_t, record, 0) == 0)
    {
        void* stream = CSF_AT(void*, record, 0x4);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, stream, 0)[0x80 / 4])(
                stream, nullptr, 0, record);
    }

    const int savedState = CSF_AT(int, record, 0x1C);
    CSF_AT(std::uint8_t, record, 0) = failed;
    CSF_AT(int, matrixRecordBytes, 0x8) = savedState;
    CSF_AT(int, record, 0x1C) = 0;

    if (!failed)
    {
        void* stream = CSF_AT(void*, record, 0x4);
        CSF_AT(std::uint8_t, record, 0x15) = 0;
        CSF_AT(std::uint8_t, record, 0x16) = 0;
        CSF_AT(std::uint8_t, record, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, stream, 0)[0x44 / 4])(
                    stream, nullptr,
                    static_cast<std::uint8_t*>(self) + 0x11, record) == -1;
    }

    CSF_AT(std::uint32_t*, matrixRecordBytes, 0) = RwMatrixCreate();
    const bool readingMatrix = CSF_AT(std::uint8_t, record, 0x14) != 0;
    CSF_AT(std::uint8_t, matrixRecordBytes, 0x4) = 1;
    if (readingMatrix)
    {
        GfxObjectInstances_GetMatrix(
            static_cast<std::uint8_t*>(self) - 8, nullptr,
            reinterpret_cast<void**>(matrixRecordBytes));
    }

    {
        const bool ready = CSF_AT(std::uint8_t, record, 0) == 0;
        CSF_AT(int, record, 0x1C) = 0;
        if (ready)
        {
            CSF_AT(std::uint8_t, record, 0x15) = 0;
            CSF_AT(std::uint8_t, record, 0x16) = 0;
            GfxFrames_UpdateFrameRecordState(matrixRecordBytes, nullptr, record);
        }
    }

    void* entryOwner = CSF_AT(void*, self, 0x38);
    void* entries = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, entryOwner, 0)[0x1C / 4])(entryOwner, nullptr);
    CSF_AT(void*, matrixRecordBytes, 0x10) = entries;

    void** payloadEntry = reinterpret_cast<void**>(
        C_0085e7b8_FindEntryPayloadByKey(
            reinterpret_cast<C_0085e7b8*>(entries), nullptr, 5));
    std::uint8_t hasPayload = payloadEntry && *payloadEntry ? 1 : 0;

    {
        const bool ready = CSF_AT(std::uint8_t, record, 0) == 0;
        CSF_AT(int, record, 0x1C) = 0;
        if (ready)
        {
            void* stream = CSF_AT(void*, record, 0x4);
            CSF_AT(std::uint8_t, record, 0x15) = 0;
            CSF_AT(std::uint8_t, record, 0x16) = 0;
            CSF_AT(std::uint8_t, record, 0) =
                ((int(__fastcall*)(void*, void*, void*, void*))
                    CSF_AT(void**, stream, 0)[0x44 / 4])(
                        stream, nullptr, &hasPayload, record) == -1;
        }
    }

    if (hasPayload)
    {
        CSF_AT(bool, flagBytes, 0) = false;
        if (CSF_AT(std::uint8_t, record, 0x14) != 0)
        {
            void* instance = CSF_AT(void*, matrixRecordBytes, 0xC);
            void* flagOwner = CSF_AT(void*, instance, 0x8);
            void* flags = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, flagOwner, 0)[0x20 / 4])(
                    flagOwner, nullptr);
            CSF_AT(bool, flagBytes, 0) =
                (CSF_AT(std::uint32_t, flags, 0) & 0x800) == 2048;
        }

        {
            const bool ready = CSF_AT(std::uint8_t, record, 0) == 0;
            CSF_AT(int, record, 0x1C) = 0;
            if (ready)
            {
                void* stream = CSF_AT(void*, record, 0x4);
                CSF_AT(std::uint8_t, record, 0x15) = 0;
                CSF_AT(std::uint8_t, record, 0x16) = 0;
                CSF_AT(std::uint8_t, record, 0) =
                    ((int(__fastcall*)(void*, void*, void*, void*))
                        CSF_AT(void**, stream, 0)[0x44 / 4])(
                            stream, nullptr, flagBytes, record) == -1;
            }
        }

        void* payload = payloadEntry ? *payloadEntry : nullptr;
        const bool loading = CSF_AT(std::uint8_t, record, 0x14) == 0;
        CSF_AT(void*, referenceBytes, 0) = payload;
        if (loading && CSF_AT(std::uint8_t, record, 0) == 0 && !payload)
        {
            void* storage = Mem_Alloc(20);
            void* newPayload = nullptr;
            if (storage)
            {
                newPayload = GfxObjectInstances_ConstructEntryPayload(
                    storage, nullptr,
                    reinterpret_cast<void**>(
                        CSF_AT(void*, matrixRecordBytes, 0x10)),
                    0);
            }

            CSF_AT(void*, referenceBytes, 0) = newPayload;
            C_008670f0_SetEntry(
                reinterpret_cast<C_008670f0*>(
                    CSF_AT(void*, matrixRecordBytes, 0x10)),
                nullptr, 5, reinterpret_cast<int>(newPayload));
        }

        {
            const bool ready = CSF_AT(std::uint8_t, record, 0) == 0;
            CSF_AT(int, record, 0x1C) = 0;
            if (ready)
            {
                CSF_AT(std::uint8_t, record, 0x15) = 1;
                CSF_AT(std::uint8_t, record, 0x16) = 0;
                SceneLoader_SerializeReference(
                    referenceBytes, reinterpret_cast<bool*>(record));
            }
        }
    }

    if (CSF_AT(std::uint8_t, record, 0x14) == 0 &&
        CSF_AT(std::uint8_t, record, 0) == 0)
    {
        auto* state = reinterpret_cast<C_00860700*>(stateBytes);
        GfxObject_ConstructEmbeddedState(state, nullptr);

        void* instance = CSF_AT(void*, matrixRecordBytes, 0xC);
        void* frame = CSF_AT(void*, instance, 0x4);
        GfxFrame_InitRotationDegreesFromMatrix(
            stateBytes, nullptr, CSF_AT(int, frame, 0x4));
        C_00860700_SetMatrixAndUpdateEulerDegrees(
            state, nullptr, reinterpret_cast<const void**>(matrixRecordBytes));
        GfxObject_DestroyEmbeddedState(
            reinterpret_cast<C_00860630*>(stateBytes), nullptr);
    }

    if (CSF_AT(std::uint8_t, matrixRecordBytes, 0x4) != 0)
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixRecordBytes, 0));

    const bool ready = CSF_AT(std::uint8_t, record, 0) == 0;
    CSF_AT(int, record, 0x1C) = CSF_AT(int, matrixRecordBytes, 0x8);
    if (ready)
    {
        void* stream = CSF_AT(void*, record, 0x4);
        const bool result = ((bool(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, record);
        if (result)
        {
            CSF_AT(std::uint8_t, record, 0) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, record, 0) = 1;
    return 1;
}

HOOK(0x006E64D0, GfxObjectInstances_SerializeInstance, "auto");
