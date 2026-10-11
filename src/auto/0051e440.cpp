#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_00840f48_CreateRegistryObject(C_00840f48* self, void* /*edx*/, int handle)
{
    void* mainObj = reinterpret_cast<void*>(GetValue8B0554());
    void* table = CSF_AT(void*, mainObj, 0x1fcc);
    int idx = ((int(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, table, 0)[2])(table, nullptr, self->field_0008);
    int key = idx + 4;

    int globalInt = GetGlobalInt();
    void* registry = reinterpret_cast<void*>(
        SceneLoader_GetObjectRegistry(reinterpret_cast<void*>(globalInt), nullptr));

    void* obj = ((void*(__fastcall*)(void*, void*, int))CSF_AT(void**, registry, 0)[5])(registry, nullptr, key);
    if (obj) {
        void* entity = reinterpret_cast<void*>(CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));

        void* sub = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, entity, 0)[10])(entity, nullptr);
        int found = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, sub, 0)[38])(sub, nullptr, 223);

        void* cur = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, entity, 0)[10])(entity, nullptr);
        if (found != -1) {
            cur = ((void*(__fastcall*)(void*, void*, int))CSF_AT(void**, cur, 0)[35])(cur, nullptr, found);
        }

        void** objVtbl = CSF_AT(void**, obj, 0);
        int value = ((int(__fastcall*)(void*, void*))CSF_AT(void**, cur, 0)[6])(cur, nullptr);
        ((void(__fastcall*)(void*, void*, int))objVtbl[17])(obj, nullptr, value);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[10])(obj, nullptr, 0);
    }
    return reinterpret_cast<int>(obj);
}

HOOK(0x0051E440, C_00840f48_CreateRegistryObject, "auto");
