#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, DynamicObjects_SerializeReference, 0x00442E70, (S_00691860* context, bool* state));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void**, __fastcall, SceneLoader_RegisterArchivedObject, 0x0069ACB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, AiAttackTasks_UpdateSlotFromManager, 0x006B32C0, (int owner, S_00691860* slot));
CSF_FUNC(char, __fastcall, AiAttackTasks_DestroyArchivedState, 0x006B5E90, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AiAttackTasks_PrepareArchivedState, 0x006B6160, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiAttackTasks_SerializeStateBlock, 0x006B6690, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));
CSF_FUNC(int, __fastcall, C_008670f0_SetEntry, 0x006C4B00, (C_008670f0* self, void* , int key, int value));
CSF_FUNC(void*, __fastcall, C_00861bd8_InitWithSkinFlags, 0x006E5210, (C_00861bd8* self, void* , int** a2, char a3));

static int __fastcall AiAttackTasks_SerializeArchivedState(
    void* self, void* /*edx*/, std::uint8_t* archive)
{
    bool failed = false;
    if (CSF_AT(std::uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, std::uint8_t*))
            CSF_AT(void**, stream, 0)[0x80 / 4])(stream, nullptr, 0, archive);
    }

    const std::uint32_t savedContext = CSF_AT(std::uint32_t, archive, 0x1C);
    CSF_AT(std::uint8_t, archive, 0x00) = failed;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        CSF_AT(std::uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, std::uint8_t*))
                CSF_AT(void**, stream, 0)[0x20 / 4])(
                    stream, nullptr, static_cast<std::uint8_t*>(self) + 0x5C,
                    archive) == -1;
    }

    bool ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        AiAttackTasks_UpdateSlotFromManager(
            reinterpret_cast<int>(static_cast<std::uint8_t*>(self) + 0x28),
            reinterpret_cast<S_00691860*>(archive));
    }

    if ((CSF_AT(std::uint32_t, self, 0x5C) & 0x40000) != 0)
    {
        std::uint32_t index = 0;
        if (CSF_AT(std::uint16_t, self, 0x14) != 0)
        {
            do
            {
                auto* references = CSF_AT(std::uint8_t*, self, 0x10);
                ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
                CSF_AT(std::uint32_t, archive, 0x1C) = 0;
                if (ready)
                {
                    CSF_AT(std::uint8_t, archive, 0x15) = 1;
                    CSF_AT(std::uint8_t, archive, 0x16) = 0;
                    DynamicObjects_SerializeReference(
                        reinterpret_cast<S_00691860*>(references + 4 * index),
                        reinterpret_cast<bool*>(archive));
                }
                const std::uint32_t count = CSF_AT(std::uint16_t, self, 0x14);
                ++index;
                if (index >= count)
                    break;
            } while (true);
        }
    }

    void* provider = CSF_AT(void*, self, 0x60);
    int** entries = ((int**(__fastcall*)(void*, void*))
        CSF_AT(void**, provider, 0)[0x1C / 4])(provider, nullptr);

    alignas(8) std::uint8_t referenceBytes[0x4];
    void*& reference = CSF_AT(void*, referenceBytes, 0);
    reference = reinterpret_cast<void*>(C_0085e7b8_FindEntryPayloadByKey(
        reinterpret_cast<C_0085e7b8*>(entries), nullptr, 5));

    std::uint8_t hasEntry =
        reference != nullptr && CSF_AT(std::uint32_t, reference, 0) != 0;

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        CSF_AT(std::uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, std::uint8_t*, std::uint8_t*))
                CSF_AT(void**, stream, 0)[0x44 / 4])(
                    stream, nullptr, &hasEntry, archive) == -1;
    }

    if (hasEntry)
    {
        reference = reference ? CSF_AT(void*, reference, 0) : nullptr;
        if (CSF_AT(std::uint8_t, archive, 0x14) == 0 &&
            CSF_AT(std::uint8_t, archive, 0x00) == 0 &&
            reference == nullptr)
        {
            auto* object = static_cast<C_00861bd8*>(Mem_Alloc(20));
            reference = object
                ? C_00861bd8_InitWithSkinFlags(
                    object, nullptr, entries,
                    (CSF_AT(std::uint32_t, self, 0x5C) & 0x800) != 2048)
                : nullptr;
            C_008670f0_SetEntry(
                reinterpret_cast<C_008670f0*>(entries), nullptr, 5,
                reinterpret_cast<int>(reference));
        }

        ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
        CSF_AT(std::uint32_t, archive, 0x1C) = 0;
        if (ready)
        {
            CSF_AT(std::uint8_t, archive, 0x15) = 1;
            CSF_AT(std::uint8_t, archive, 0x16) = 0;
            DynamicObjects_SerializeReference(
                reinterpret_cast<S_00691860*>(referenceBytes),
                reinterpret_cast<bool*>(archive));
        }
    }

    void* stateBlock = Mem_Alloc(64);
    if (stateBlock)
    {
        CSF_AT(std::uint32_t, stateBlock, 0x04) = 0;
        CSF_AT(std::uint32_t, stateBlock, 0x08) = 0;
        CSF_AT(std::uint32_t, stateBlock, 0x0C) = 0;
        CSF_AT(std::uint32_t, stateBlock, 0x14) = 0;
        CSF_AT(std::uint32_t, stateBlock, 0x18) = 0;
        CSF_AT(std::uint32_t, stateBlock, 0x1C) = 0;
        CSF_AT(std::uint32_t, stateBlock, 0x24) = 0;
        CSF_AT(std::uint32_t, stateBlock, 0x28) = 0;
        CSF_AT(std::uint32_t, stateBlock, 0x2C) = 0;
        CSF_AT(std::uint32_t, stateBlock, 0x34) = 0;
        CSF_AT(std::uint32_t, stateBlock, 0x38) = 0;
        CSF_AT(std::uint32_t, stateBlock, 0x3C) = 0;
    }
    CSF_AT(void*, self, 0x78) = stateBlock;

    if (CSF_AT(std::uint8_t, archive, 0x14) != 0)
        AiAttackTasks_PrepareArchivedState(
            static_cast<std::uint8_t*>(self) - 8, nullptr);

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    stateBlock = CSF_AT(void*, self, 0x78);
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        AiAttackTasks_SerializeStateBlock(
            stateBlock, nullptr, reinterpret_cast<int>(archive));
    }

    if (CSF_AT(std::uint8_t, archive, 0x14) != 0 ||
        CSF_AT(std::uint8_t, archive, 0x00) != 0)
    {
        stateBlock = CSF_AT(void*, self, 0x78);
        if (stateBlock)
        {
            AiAttackTasks_DestroyArchivedState(stateBlock, nullptr);
            Mem_Free(stateBlock);
            CSF_AT(void*, self, 0x78) = nullptr;
        }
    }
    else
    {
        SceneLoader_RegisterArchivedObject(
            CSF_AT(void*, archive, 0x10), nullptr,
            reinterpret_cast<std::uintptr_t>(self) != 8
                ? reinterpret_cast<int>(self) : 0);
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 1;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            static_cast<std::uint8_t*>(self) + 0x24,
            reinterpret_cast<bool*>(archive));
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = savedContext;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        const bool result = ((bool(__fastcall*)(void*, void*, std::uint8_t*))
            CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, archive);
        if (result)
        {
            CSF_AT(std::uint8_t, archive, 0x00) = 0;
            return result;
        }
    }
    CSF_AT(std::uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x006B67E0, AiAttackTasks_SerializeArchivedState, "auto");
