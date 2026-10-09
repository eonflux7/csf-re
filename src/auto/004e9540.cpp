#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue_8afc80, 0x004419C0, ());
CSF_FUNC(int, __fastcall, sub_441F10, 0x00441F10, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, CScene_AddObject, 0x00453420, (CScene* self, void* , C_00867260* object));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static C_00833908* __fastcall C_0085e7b8_LookupStateEntry(C_0085e7b8* self, void* /*edx*/, int a2, int a3, int a4, int a5)
{
    int sceneIndex = a2;
    if (a2 == -1) {
        void* indexProvider = CSF_AT(void*, self, 0x1FC8);
        int resolvedIndex = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, indexProvider, 0)[4])(indexProvider, nullptr, a3);
        if (resolvedIndex != -1) {
            void* stateInfo = ((void*(__fastcall*)(void*, void*, int))CSF_AT(void**, indexProvider, 0)[2])(indexProvider, nullptr, resolvedIndex);
            sceneIndex = CSF_AT(int, stateInfo, 0xB4);
        }
    }

    C_00833908* result = nullptr;
    if (sceneIndex != -1) {
        S_00441ca0* valueTable = reinterpret_cast<S_00441ca0*>(GetValue_8afc80());
        void* createdObject = reinterpret_cast<void*>(sub_441F10(valueTable, nullptr, sceneIndex, a3, a4));
        result = reinterpret_cast<C_00833908*>(createdObject);
        if (createdObject) {
            CScene_AddObject(reinterpret_cast<CScene*>(g_pCurrentScene), nullptr, reinterpret_cast<C_00867260*>(createdObject));
            if (!static_cast<uint8_t>(a4)) {
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, createdObject, 0)[25])(createdObject, nullptr, a5);
            }
        }
    }
    return result;
}

HOOK(0x004E9540, C_0085e7b8_LookupStateEntry, "auto");
