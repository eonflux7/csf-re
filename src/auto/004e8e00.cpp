#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(unsigned int, __fastcall, SceneTable_ClearSlotState, 0x0051B660, (void* self, void* edx, unsigned int a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static int __fastcall SceneTable_ApplyEntry(S_004e8e00* self, void* /*edx*/, int a2)
{
    int result = a2;
    if (a2 != -1) {
        void* entryManager = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_1fd4));
        // entry = entryManager->vtbl[2](a2)
        void* entry = ((void* (__fastcall*)(void*, void*, int))CSF_AT(void**, entryManager, 0)[2])(entryManager, nullptr, a2);

        void* globalState = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
        void* stateSub = CSF_AT(void*, globalState, 0xE4);
        ((void (__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, stateSub, 0)[7])(
            stateSub, nullptr, CSF_AT(uint32_t, entry, 0x24));

        globalState = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
        stateSub = CSF_AT(void*, globalState, 0xE4);
        ((void (__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, stateSub, 0)[7])(
            stateSub, nullptr, CSF_AT(uint32_t, entry, 0x2C));

        SceneTable_ClearSlotState(entryManager, nullptr, static_cast<unsigned int>(a2));

        void* registry = CSF_AT(void*, self, 0x1FCC);
        result = ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, registry, 0)[4])(
            registry, nullptr, static_cast<int>(CSF_AT(uint32_t, entry, 0x28)));
        if (result != -1) {
            int slotHandle = ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, registry, 0)[2])(
                registry, nullptr, result);
            void* loaderInt = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
            void* objectRegistry = reinterpret_cast<void*>(static_cast<uintptr_t>(
                SceneLoader_GetObjectRegistry(loaderInt, nullptr)));
            return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, objectRegistry, 0)[4])(
                objectRegistry, nullptr, slotHandle + 4);
        }
    }
    return result;
}

HOOK(0x004E8E00, SceneTable_ApplyEntry, "auto");
