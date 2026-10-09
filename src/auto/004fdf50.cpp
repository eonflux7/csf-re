#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, C_0085e7b8_ClearPendingObjects, 0x004F04C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0085e7b8_GetPendingObject, 0x004F04F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_SetClumpContext, 0x006D1030, (void* self, void* edx, int a2));

static int __stdcall SceneLoader_RegisterPendingObjects(int context)
{
    const int rootAddress = GetValue8B0554();
    auto* pendingObjects = CSF_AT(
        C_0085e7b8*, reinterpret_cast<void*>(rootAddress), 0x1F98);

    if (!pendingObjects)
        return rootAddress;

    const auto count = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(
            GetCachedObjectProvider(pendingObjects, nullptr)));

    if (count == 0)
        return 0;

    const int managerId = GetGlobalInt();
    void* manager = reinterpret_cast<void*>(
        ResolveClumpManager(managerId, nullptr));

    SceneLoader_SetClumpContext(manager, nullptr, context);

    std::uint32_t index = 0;
    do
    {
        void* record = reinterpret_cast<void*>(
            C_0085e7b8_GetPendingObject(
                pendingObjects, nullptr, static_cast<int>(index)));

        void* object =
            reinterpret_cast<void* (__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, manager, 0)[0x28 / 4])(
                    manager, nullptr, 2,
                    (CSF_AT(std::uint8_t, record, 0x20) == 0) + 1);

        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, object, 0)[0x44 / 4])(
                object, nullptr, static_cast<std::uint8_t*>(record) + 0x10);

        reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t)>(
            CSF_AT(void**, object, 0)[0x3C / 4])(
                object, nullptr, CSF_AT(std::uint32_t, record, 0x1C));

        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, object, 0)[0x34 / 4])(
                object, nullptr, record);

        ++index;
    } while (index < count);

    C_0085e7b8_ClearPendingObjects(pendingObjects, nullptr);
    return SceneLoader_SetClumpContext(manager, nullptr, 0);
}

HOOK(0x004FDF50, SceneLoader_RegisterPendingObjects, "auto");
