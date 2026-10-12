#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (S_00470ab0* self, void* , void* a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(char, __fastcall, CoreFiles_DestroyOptionsStorage, 0x00698010, (void* self, void* edx));
CSF_FUNC(char, __cdecl, SceneLoader_ResetArchiveSelection, 0x006980D0, (char* String1));
CSF_FUNC(void, __fastcall, GameBddLoader_WriteLoadingLog, 0x006986E0, (int a1, int a2));
CSF_FUNC(char, __fastcall, CoreFiles_DestroySceneFileState, 0x0069F2E0, (void* self, void* edx));

static int __cdecl CoreFiles_ReleaseOptions()
{
    void* options = g_pOptions;
    const std::uint8_t accessLocked = CSF_AT(std::uint8_t, options, 0x01);
    CSF_AT(std::uint8_t, options, 0x02) = 0;

    if (accessLocked)
    {
        alignas(8) std::uint8_t messageBytes[0x1C];
        C_0082d410& message =
            *reinterpret_cast<C_0082d410*>(messageBytes);

        CSF_AT(std::uint32_t, messageBytes, 0x18) = 15;
        CSF_AT(std::uint32_t, messageBytes, 0x14) = 0;
        reinterpret_cast<std::uint8_t*>(&message.field_0004)[0] = 0;

        AssignStringRange_429B60(
            &message, nullptr,
            const_cast<char*>(CSF_STR(0x0085E6C0, "- ACCESS UNLOCKED\r\n")),
            19);
        MenuManager_AppendPageName(
            reinterpret_cast<S_00470ab0*>(
                static_cast<std::uint8_t*>(g_pOptions) + 0x30),
            nullptr, &message);

        if (CSF_AT(std::uint32_t, messageBytes, 0x18) >= 0x10)
        {
            if (message.field_0004)
                Mem_FreeDebug(reinterpret_cast<void*>(message.field_0004));
        }
    }

    // Retain the prescribed binding and supply the live options context.
    // The supplied callee implementation actually uses g_pOptions directly.
    GameBddLoader_WriteLoadingLog(reinterpret_cast<int>(g_pOptions), 0);
    options = g_pOptions;

    const std::uint32_t archiveBegin =
        CSF_AT(std::uint32_t, options, 0x54);
    std::uint32_t archiveCount = 0;
    if (archiveBegin)
    {
        const std::uint32_t difference =
            CSF_AT(std::uint32_t, options, 0x58) - archiveBegin;
        archiveCount = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(difference) >> 5);
    }

    std::uint32_t archiveOffset = 0;
    while (archiveCount != 0)
    {
        std::uint8_t* archive =
            CSF_AT(std::uint8_t*, options, 0x54) + archiveOffset;
        char* name;
        if (CSF_AT(std::uint32_t, archive, 0x18) < 0x10)
            name = reinterpret_cast<char*>(archive + 4);
        else
            name = CSF_AT(char*, archive, 0x04);

        SceneLoader_ResetArchiveSelection(name);
        options = g_pOptions;
        archiveOffset += 32;
        --archiveCount;
    }

    void* sceneState = CSF_AT(void*, options, 0x60);
    if (sceneState)
    {
        CloseSceneFile(CSF_AT(void*, sceneState, 0x04));
        options = g_pOptions;
        sceneState = CSF_AT(void*, options, 0x60);
        if (sceneState)
        {
            CoreFiles_DestroySceneFileState(sceneState, nullptr);
            Mem_Free(sceneState);
            options = g_pOptions;
            CSF_AT(void*, options, 0x60) = nullptr;
        }
    }

    const std::uint32_t objectCount =
        CSF_AT(std::uint32_t, options, 0x28);
    for (std::uint32_t index = 0; index < objectCount; ++index)
    {
        void* object = CSF_AT(void**, options, 0x20)[index];
        if (object)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
            options = g_pOptions;
            CSF_AT(void**, options, 0x20)[index] = nullptr;
        }
    }

    CSF_AT(std::uint32_t, options, 0x28) = 0;
    void* allocation = options;
    CoreFiles_DestroyOptionsStorage(options, nullptr);
    const int result = Mem_Free(allocation);
    g_pOptions = nullptr;
    return result;
}

HOOK(0x00698820, CoreFiles_ReleaseOptions, "auto");
