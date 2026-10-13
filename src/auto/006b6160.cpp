#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CopyDwordArrayWithContext_403790, 0x00403790, (std::uint32_t* destination, std::uint32_t count, const std::uint32_t* source, void* , void*));
CSF_FUNC(unsigned int, __fastcall, InsertDwordElements_403EE0, 0x00403EE0, (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int*, __cdecl, GfxObject_CopyChildCacheRecords, 0x006B5960, (int* a1, int a2, int a3, int a4, int a5));
CSF_FUNC(char, __fastcall, GfxObject_InsertChildCacheRecords, 0x006B5A90, (void* self, void* edx, void* a1, unsigned int a3, int a2));
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (C_00860630* self, void*));
CSF_FUNC(int, __fastcall, GfxObject_ConstructEmbeddedState, 0x006BFD00, (C_00860700* self, void*));
CSF_FUNC(int, __fastcall, GfxScene_FindChildByIdentifier, 0x006CB820, (void* self, void* , int identifier));
CSF_FUNC(int, __cdecl, GfxTextureLoader_ResetRecordRange, 0x006DEC40, (int* begin, int* end));

static char __fastcall GfxObject_RebuildSceneChildCache(void* self, void* /*edx*/)
{
    alignas(8) std::uint8_t stateBytes[0x20];
    C_00860700& state = *reinterpret_cast<C_00860700*>(stateBytes);
    alignas(8) std::uint8_t recordSourceBytes[0x4] = {};
    alignas(8) std::uint8_t positionBytes[0x14];
    alignas(8) std::uint8_t childBytes[0x4];
    alignas(8) std::uint8_t otherChildBytes[0xC];

    const int sceneController = SceneLoader_GetSceneController(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    void* records = CSF_AT(void*, self, 0x80);
    int* begin = CSF_AT(int*, records, 0x4);
    std::memset(positionBytes, 0, 12);
    CSF_AT(int, positionBytes, 0xC) = 0;
    CSF_AT(int, positionBytes, 0x10) = sceneController;

    if (begin)
    {
        GfxTextureLoader_ResetRecordRange(begin, CSF_AT(int*, records, 0x8));
        if (CSF_AT(void*, records, 0x4))
            Mem_FreeDebug(CSF_AT(void*, records, 0x4));
    }
    CSF_AT(std::uint32_t, records, 0x4) = 0;
    CSF_AT(std::uint32_t, records, 0x8) = 0;
    CSF_AT(std::uint32_t, records, 0xC) = 0;

    std::uint32_t groupIndex = 0;
    if (CSF_AT(std::uint16_t, self, 0x24) != 0)
    {
        do
        {
            records = CSF_AT(void*, self, 0x80);
            const std::uint32_t recordBegin = CSF_AT(std::uint32_t, records, 0x4);
            const std::uint32_t recordCount = recordBegin
                ? static_cast<std::uint32_t>(static_cast<std::int32_t>(
                    CSF_AT(std::uint32_t, records, 0x8) - recordBegin) >> 4)
                : 0;

            if (recordBegin && recordCount <
                static_cast<std::uint32_t>(static_cast<std::int32_t>(
                    CSF_AT(std::uint32_t, records, 0xC) - recordBegin) >> 4))
            {
                int* destination = CSF_AT(int*, records, 0x8);
                GfxObject_CopyChildCacheRecords(
                    destination, 1,
                    static_cast<int>(reinterpret_cast<std::uintptr_t>(recordSourceBytes)),
                    static_cast<int>(reinterpret_cast<std::uintptr_t>(records)), 0);
                CSF_AT(int*, records, 0x8) = destination + 4;
            }
            else
            {
                GfxObject_InsertChildCacheRecords(
                    records, nullptr, CSF_AT(void*, records, 0x8), 1,
                    static_cast<int>(reinterpret_cast<std::uintptr_t>(recordSourceBytes)));
            }

            void* group = CSF_AT(void*, CSF_AT(void*, self, 0x20), 4 * groupIndex);
            void* entries = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, group, 0)[0x18 / 4])(group, nullptr);

            int entryIndex = 0;
            if (((int(__fastcall*)(void*, void*))
                CSF_AT(void**, entries, 0)[0x74 / 4])(entries, nullptr) > 0)
            {
                do
                {
                    GfxObject_ConstructEmbeddedState(&state, nullptr);
                    if (((bool(__fastcall*)(void*, void*, int, C_00860700*))
                        CSF_AT(void**, entries, 0)[0x78 / 4])(
                            entries, nullptr, entryIndex, &state))
                    {
                        CSF_AT(int, otherChildBytes, 0) =
                            GfxScene_FindChildByIdentifier(
                                reinterpret_cast<void*>(sceneController),
                                nullptr, state.field_000c);
                        if (CSF_AT(int, otherChildBytes, 0))
                        {
                            auto* childList = CSF_AT(std::uint8_t*,
                                CSF_AT(void*, self, 0x80), 0x8) - 16;
                            const std::uint32_t childBegin =
                                CSF_AT(std::uint32_t, childList, 0x4);
                            const std::uint32_t childCount = childBegin
                                ? static_cast<std::uint32_t>(static_cast<std::int32_t>(
                                    CSF_AT(std::uint32_t, childList, 0x8) - childBegin) >> 2)
                                : 0;

                            if (childBegin && childCount <
                                static_cast<std::uint32_t>(static_cast<std::int32_t>(
                                    CSF_AT(std::uint32_t, childList, 0xC) - childBegin) >> 2))
                            {
                                std::uint32_t* destination =
                                    CSF_AT(std::uint32_t*, childList, 0x8);
                                CopyDwordArrayWithContext_403790(
                                    destination, 1,
                                    reinterpret_cast<std::uint32_t*>(otherChildBytes),
                                    childList, nullptr);
                                CSF_AT(std::uint32_t*, childList, 0x8) = destination + 1;
                            }
                            else
                            {
                                InsertDwordElements_403EE0(
                                    childList, nullptr, CSF_AT(char*, childList, 0x8),
                                    1u, reinterpret_cast<std::uint32_t*>(otherChildBytes));
                            }
                        }
                    }
                    GfxObject_DestroyEmbeddedState(
                        reinterpret_cast<C_00860630*>(stateBytes), nullptr);
                    ++entryIndex;
                }
                while (entryIndex < ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, entries, 0)[0x74 / 4])(entries, nullptr));
            }
            ++groupIndex;
        }
        while (groupIndex < CSF_AT(std::uint16_t, self, 0x24));
    }

    auto* directChildren = CSF_AT(std::uint8_t*, self, 0x80) + 16;
    if (CSF_AT(void*, directChildren, 0x4))
        Mem_FreeDebug(CSF_AT(void*, directChildren, 0x4));
    CSF_AT(std::uint32_t, directChildren, 0x4) = 0;
    CSF_AT(std::uint32_t, directChildren, 0x8) = 0;
    CSF_AT(std::uint32_t, directChildren, 0xC) = 0;

    auto* embeddedEntries = static_cast<std::uint8_t*>(self) + 0x30;
    int entryIndex = 0;
    if (((int(__fastcall*)(void*, void*))
        CSF_AT(void**, embeddedEntries, 0)[0x74 / 4])(embeddedEntries, nullptr) > 0)
    {
        do
        {
            GfxObject_ConstructEmbeddedState(&state, nullptr);
            if (((bool(__fastcall*)(void*, void*, int, C_00860700*))
                CSF_AT(void**, embeddedEntries, 0)[0x78 / 4])(
                    embeddedEntries, nullptr, entryIndex, &state))
            {
                CSF_AT(int, childBytes, 0) = GfxScene_FindChildByIdentifier(
                    reinterpret_cast<void*>(sceneController), nullptr, state.field_000c);
                if (CSF_AT(int, childBytes, 0))
                {
                    auto* childList = CSF_AT(std::uint8_t*, self, 0x80) + 16;
                    const std::uint32_t childBegin =
                        CSF_AT(std::uint32_t, childList, 0x4);
                    const std::uint32_t childCount = childBegin
                        ? static_cast<std::uint32_t>(static_cast<std::int32_t>(
                            CSF_AT(std::uint32_t, childList, 0x8) - childBegin) >> 2)
                        : 0;
                    if (childBegin && childCount <
                        static_cast<std::uint32_t>(static_cast<std::int32_t>(
                            CSF_AT(std::uint32_t, childList, 0xC) - childBegin) >> 2))
                    {
                        std::uint32_t* destination =
                            CSF_AT(std::uint32_t*, childList, 0x8);
                        CopyDwordArrayWithContext_403790(
                            destination, 1, reinterpret_cast<std::uint32_t*>(childBytes),
                            childList, nullptr);
                        CSF_AT(std::uint32_t*, childList, 0x8) = destination + 1;
                    }
                    else
                    {
                        InsertDwordElements_403EE0(
                            childList, nullptr, CSF_AT(char*, childList, 0x8),
                            1u, reinterpret_cast<std::uint32_t*>(childBytes));
                    }
                }
            }
            GfxObject_DestroyEmbeddedState(
                reinterpret_cast<C_00860630*>(stateBytes), nullptr);
            ++entryIndex;
        }
        while (entryIndex < ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, embeddedEntries, 0)[0x74 / 4])(embeddedEntries, nullptr));
    }

    void* transform = CSF_AT(void*, self, 0x6C);
    const bool hasTransform = transform &&
        ((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, transform, 0)[0x28 / 4])(transform, nullptr);
    CSF_AT(bool, CSF_AT(void*, self, 0x80), 0x20) = hasTransform;
    const char result = CSF_AT(char, CSF_AT(void*, self, 0x80), 0x20);
    if (result)
    {
        transform = CSF_AT(void*, self, 0x6C);
        std::uint32_t* position = ((std::uint32_t*(__fastcall*)(void*, void*, int*))
            CSF_AT(void**, transform, 0)[0x50 / 4])(
                transform, nullptr, reinterpret_cast<int*>(positionBytes));
        auto* destination = CSF_AT(std::uint8_t*, self, 0x80) + 36;
        CSF_AT(std::uint32_t, destination, 0) = position[0];
        CSF_AT(std::uint32_t, destination, 0x4) = position[1];
        CSF_AT(std::uint32_t, destination, 0x8) = position[2];

        transform = CSF_AT(void*, self, 0x6C);
        return static_cast<char>(((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, transform, 0)[0x64 / 4])(
                transform, nullptr, CSF_AT(std::uint8_t*, self, 0x80) + 48));
    }
    return result;
}

HOOK(0x006B6160, GfxObject_RebuildSceneChildCache, "auto");
