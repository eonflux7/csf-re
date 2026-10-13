#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_Value_8B0568, 0x008B0568);

CSF_FUNC(uint32_t*, __fastcall, C_008310d8_Initialize, 0x00444FB0, (C_008310d8* self, void*));
CSF_FUNC(char, __fastcall, S_00444fd0_ClearSecondaryResources, 0x00445240, (S_00444fd0* self, void*));
CSF_FUNC(C_008311c4*, __fastcall, C_008311c4_InitDefaults, 0x0044CC50, (C_008311c4* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_008312e0_Construct, 0x00456C90, (C_008312e0* self, void*));
CSF_FUNC(CSceneLoader*, __fastcall, CSceneLoader_Construct, 0x00505BD0, (CSceneLoader* self, void*));
CSF_FUNC(void, __fastcall, ClearFirstField, 0x00509950, (void* self, void* , int));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

static char __cdecl GameBddLoader_PrepareScenarioSelection()
{
    if (g_Value_8B0568)
        return static_cast<char>(g_Value_8B0568);

    auto* loaderStorage =
        reinterpret_cast<CSceneLoader*>(CoreMemory_AllocUntagged(284));
    g_Value_8B0568 = loaderStorage
        ? reinterpret_cast<int>(CSceneLoader_Construct(loaderStorage, nullptr))
        : 0;

    int ownerStorage = CoreMemory_AllocUntagged(4);
    CSF_AT(int, reinterpret_cast<void*>(g_Value_8B0568), 0xFC) =
        ownerStorage;
    ClearFirstField(
        reinterpret_cast<void*>(
            CSF_AT(int, reinterpret_cast<void*>(g_Value_8B0568), 0xFC)),
        nullptr, g_Value_8B0568);

    auto* selectionStorage =
        reinterpret_cast<C_008312e0*>(CoreMemory_AllocUntagged(48));
    uint32_t* selection = selectionStorage
        ? C_008312e0_Construct(selectionStorage, nullptr)
        : nullptr;
    reinterpret_cast<CSceneLoader*>(g_Value_8B0568)->field_00e4 =
        reinterpret_cast<uint32_t>(selection);

    auto* defaultsStorage =
        reinterpret_cast<C_008311c4*>(CoreMemory_AllocUntagged(36));
    C_008311c4* defaults = defaultsStorage
        ? C_008311c4_InitDefaults(defaultsStorage, nullptr)
        : nullptr;
    reinterpret_cast<CSceneLoader*>(g_Value_8B0568)->field_00ec =
        reinterpret_cast<uint32_t>(defaults);

    auto* resourcesStorage =
        reinterpret_cast<C_008310d8*>(CoreMemory_AllocUntagged(100));
    uint32_t* resources = resourcesStorage
        ? C_008310d8_Initialize(resourcesStorage, nullptr)
        : nullptr;
    reinterpret_cast<CSceneLoader*>(g_Value_8B0568)->field_00e8 =
        reinterpret_cast<uint32_t>(resources);

    return S_00444fd0_ClearSecondaryResources(
        reinterpret_cast<S_00444fd0*>(
            reinterpret_cast<CSceneLoader*>(g_Value_8B0568)->field_00e8),
        nullptr);
}

HOOK(0x004FDAD0, GameBddLoader_PrepareScenarioSelection, "auto");
