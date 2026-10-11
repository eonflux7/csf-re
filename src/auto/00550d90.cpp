#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetInteractionMode, 0x00550CA0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MpSpawnPoints_ApplyInteractionWithFlag, 0x00745150, (void* self, void* edx, char arg0, char a3, char a2, char a5));
CSF_FUNC(char, __fastcall, MpSpawnPoints_ApplyInteraction, 0x00747940, (void* self, void* edx, char a2, char a3, char a4));
CSF_FUNC(int, __cdecl, Scene_IsObjectRemovable, 0x007558C0, (int a1));

static char __stdcall MpSpawnPoints_DispatchObjectInteraction(void* record)
{
    int status = CSF_AT(int, record, 0x10);
    if (status < 0)
        return static_cast<char>(status);

    status = CSF_AT(int, record, 0x1C);
    if (status < 0)
        return static_cast<char>(status);

    auto* bytes = static_cast<uint8_t*>(record);
    const int sourceAddress = CHandleTable_Resolve(
        g_pCurrentScene, nullptr, reinterpret_cast<int>(bytes + 0x14));
    const int destinationAddress = CHandleTable_Resolve(
        g_pCurrentScene, nullptr, reinterpret_cast<int>(bytes + 8));

    if (!sourceAddress || !destinationAddress)
        return static_cast<char>(destinationAddress);

    void* source = reinterpret_cast<void*>(sourceAddress);
    void* destination = reinterpret_cast<void*>(destinationAddress);
    const int sourceType = CSF_AT(int, source, 0x50);

    if (sourceType == 18 && CSF_AT(int, destination, 0x50) == 0)
    {
        const uintptr_t sourceAttachment = CSF_AT(uint32_t, source, 0x18);
        void* sourceObject = sourceAttachment
            ? reinterpret_cast<void*>(sourceAttachment - 1712)
            : nullptr;

        const uintptr_t destinationAttachment = CSF_AT(uint32_t, destination, 0x18);
        void* destinationObject = destinationAttachment
            ? reinterpret_cast<void*>(destinationAttachment - 1712)
            : nullptr;

        if (!sourceObject || !destinationObject)
            return static_cast<char>(destinationAttachment);

        const int mode = MpSpawnPoints_GetInteractionMode(destinationObject, nullptr);
        const char parameter0 = static_cast<char>(CSF_AT(int, record, 0x60));
        const char flag = CSF_AT(char, record, 0x64);
        const char parameter1 = static_cast<char>(CSF_AT(int, record, 0x6C));
        return MpSpawnPoints_ApplyInteractionWithFlag(
            sourceObject, nullptr, parameter0, parameter1, flag,
            static_cast<char>(mode));
    }

    const char removable = static_cast<char>(Scene_IsObjectRemovable(sourceType));
    if (!removable)
        return removable;

    const int destinationType = CSF_AT(int, destination, 0x50);
    if (destinationType != 0)
        return static_cast<char>(destinationType);

    const uintptr_t sourceAttachment = CSF_AT(uint32_t, source, 0x18);
    void* sourceObject = sourceAttachment
        ? reinterpret_cast<void*>(sourceAttachment - 112)
        : nullptr;

    const uintptr_t destinationAttachment = CSF_AT(uint32_t, destination, 0x18);
    void* destinationObject = destinationAttachment
        ? reinterpret_cast<void*>(destinationAttachment - 1712)
        : nullptr;

    if (!sourceObject || !destinationObject)
        return static_cast<char>(destinationAttachment);

    const int mode = MpSpawnPoints_GetInteractionMode(destinationObject, nullptr);
    const char parameter1 = static_cast<char>(CSF_AT(int, record, 0x6C));
    const char parameter0 = static_cast<char>(CSF_AT(int, record, 0x60));
    return MpSpawnPoints_ApplyInteraction(
        sourceObject, nullptr, parameter0, parameter1, static_cast<char>(mode));
}

HOOK(0x00550D90, MpSpawnPoints_DispatchObjectInteraction, "auto");
