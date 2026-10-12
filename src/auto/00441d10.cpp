#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, LookupDynamicObjectEntry, 0x00441CA0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char*, __fastcall, SceneLoader_SetObjectName, 0x0048E0B0, (void* self, void* edx, char* Source));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(C_00833908*, __fastcall, S_004e9090_ResolveObject, 0x004EEEF0, (S_004e9090* self, void* , int nameKey, uint32_t param, char altMode));

[[maybe_unused]] static void* const s_LookupObjectVtable = CSF_VTBL(0x00441CA0);

static int __fastcall S_00441ca0_ResolveDynamicObject(
    S_00441ca0* self, void* /*edx*/, int a2, int a3, int a4)
{
    int result = LookupDynamicObjectEntry(self, nullptr, a2, a3);
    if (!result)
        return result;

    GetValue8B0554();
    int nameKey = CSF_AT(int, reinterpret_cast<void*>(result), 0x0);
    S_004e9090* resolver =
        reinterpret_cast<S_004e9090*>(GetValue8B0554());
    C_00833908* object = S_004e9090_ResolveObject(
        resolver, nullptr, nameKey, 0xFFFFFFFF, static_cast<char>(a4));
    SceneLoader_SetObjectName(
        object, nullptr, const_cast<char*>(CSF_STR(0x0083104C, "DynamicObj")));
    return reinterpret_cast<int>(object);
}

HOOK(0x00441D10, S_00441ca0_ResolveDynamicObject, "auto");
