#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, SceneLoader_SetEntryText, 0x00457770, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(uint32_t, __fastcall, SceneLoader_SetIndexedState, 0x004ED1C0, (S_004e8e00* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_SetIndexedValue, 0x004EE630, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceByIndex, 0x007567C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall CSceneLoaderBase_ApplySceneEntries(
    C_0082d410* self, void* /*edx*/, int entriesAddress)
{
    void* flagRegistry = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC4);
    void* stateRegistry = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC8);
    void* objectRegistry = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);
    void* actionRegistry = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FD0);
    void* valueRegistry = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC0);

    void* entries = reinterpret_cast<void*>(entriesAddress);
    uint32_t begin = CSF_AT(uint32_t, entries, 0x4);
    int count = 0;
    if (begin)
    {
        count = static_cast<int32_t>(
            CSF_AT(uint32_t, entries, 0x8) - begin) / 68;
    }
    if (!count)
        return 0;

    // The original eight-byte payload shares its last dword with the iteration offset.
    struct EntryUpdate
    {
        uint8_t enabled;
        uint8_t reserved[3];
        uint32_t entryOffset;
    } update{};

    uint32_t remaining = static_cast<uint32_t>(count);
    do
    {
        auto* entry = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, entries, 0x4))
                    + update.entryOffset;

        switch (CSF_AT(int, entry, 0x4))
        {
        case 0:
            if (static_cast<int32_t>(static_cast<int64_t>(
                    CSF_AT(float, entry, 0x10))) == 4058)
            {
                void* receiver = CSF_AT(void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xE4);
                void* resources = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                void** receiverVtable = CSF_AT(void**, receiver, 0);
                int resource = SceneLoader_GetResourceByIndex(
                    static_cast<uint8_t*>(resources) + 0x22A4, nullptr, 17);
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    receiverVtable[0x1C / 4])(receiver, nullptr, resource);

                receiver = CSF_AT(void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xE4);
                resources = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                receiverVtable = CSF_AT(void**, receiver, 0);
                resource = SceneLoader_GetResourceByIndex(
                    static_cast<uint8_t*>(resources) + 0x22A4, nullptr, 18);
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    receiverVtable[0x1C / 4])(receiver, nullptr, resource);
            }
            break;

        case 18:
        {
            void* text;
            if (CSF_AT(uint32_t, entry, 0x30) < 0x10u)
                text = entry + 0x1C;
            else
                text = CSF_AT(void*, entry, 0x1C);
            SceneLoader_SetEntryText(CSF_AT(void*, self, 0xE4), nullptr, text);
            break;
        }

        case 32:
        {
            int key = static_cast<int32_t>(static_cast<int64_t>(CSF_AT(float, entry, 0x10)));
            int index = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, stateRegistry, 0)[0x10 / 4])(stateRegistry, nullptr, key);
            if (index != -1)
            {
                auto* state = reinterpret_cast<S_004e8e00*>(GetValue8B0554());
                SceneLoader_SetIndexedState(state, nullptr, index, 1);
            }
            break;
        }

        case 36:
        {
            int key = static_cast<int32_t>(static_cast<int64_t>(CSF_AT(float, entry, 0x10)));
            int index = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, flagRegistry, 0)[0x8 / 4])(flagRegistry, nullptr, key);
            if (index != -1)
            {
                void** vtable = CSF_AT(void**, flagRegistry, 0);
                update.reserved[0] = 0;
                update.enabled = 1;
                reinterpret_cast<void (__fastcall*)(void*, void*, int, void*)>(
                    vtable[0x1C / 4])(flagRegistry, nullptr, index, &update);
            }
            break;
        }

        case 38:
        {
            void** vtable = CSF_AT(void**, actionRegistry, 0);
            int key = static_cast<int32_t>(static_cast<int64_t>(CSF_AT(float, entry, 0x10)));
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                vtable[0x1C / 4])(actionRegistry, nullptr, key);
            break;
        }

        case 41:
        {
            void** vtable = CSF_AT(void**, objectRegistry, 0);
            int key = static_cast<int32_t>(static_cast<int64_t>(CSF_AT(float, entry, 0x10)));
            int index = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                vtable[0x10 / 4])(objectRegistry, nullptr, key);
            if (index != -1)
            {
                int object = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, objectRegistry, 0)[0x8 / 4])(objectRegistry, nullptr, index);
                void* global = reinterpret_cast<void*>(GetGlobalInt());
                void* registry = reinterpret_cast<void*>(SceneLoader_GetObjectRegistry(global, nullptr));
                reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, registry, 0)[0x10 / 4])(
                        registry, nullptr, reinterpret_cast<uint8_t*>(object) + 4);
            }
            break;
        }

        case 47:
        {
            int key = static_cast<int32_t>(static_cast<int64_t>(CSF_AT(float, entry, 0x10)));
            int index = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, valueRegistry, 0)[0xC / 4])(valueRegistry, nullptr, key);
            if (index != -1)
            {
                void* state = reinterpret_cast<void*>(GetValue8B0554());
                SceneLoader_SetIndexedValue(state, nullptr, index, -1);
            }
            break;
        }

        default:
            break;
        }

        update.entryOffset += 68;
        --remaining;
    } while (remaining != 0);

    return 0;
}

HOOK(0x004FFB70, CSceneLoaderBase_ApplySceneEntries, "auto");
