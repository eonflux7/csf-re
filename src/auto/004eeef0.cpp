#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callees (bound by address)
CSF_FUNC(int, __fastcall, SceneLoader_RegisterObject, 0x004E95C0, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(char, __fastcall, SceneLoader_SetupObject, 0x004E9660, (void* self, void* edx, void* a2, int a3, char a4));
CSF_FUNC(C_00833908*, __fastcall, SceneLoader_CreateObjectAlt, 0x004EE260, (S_004e9090* self, void* edx, C_00833908* a2, uint32_t a3));
CSF_FUNC(void*, __fastcall, SceneLoader_CreateObject, 0x004EEB90, (void* self, void* edx, void* a3, int arg4));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, IsResolvedEntryActive, 0x00755880, (int a1));

static C_00833908* __fastcall S_004e9090_ResolveObject(S_004e9090* self, void* /*edx*/, int nameKey, uint32_t param, char altMode)
{
    // Look up the entry index through the name table (vtable slot 0x10 / 4)
    void* nameTable = reinterpret_cast<void*>(self->field_1fc8);
    int entryIndex = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, nameTable, 0)[0x10 / 4])(nameTable, nullptr, nameKey);
    if (entryIndex == -1)
        return nullptr;

    C_00833908* object;
    if (altMode)
        object = SceneLoader_CreateObjectAlt(self, nullptr, reinterpret_cast<C_00833908*>(entryIndex), param);
    else
        object = static_cast<C_00833908*>(SceneLoader_CreateObject(self, nullptr, reinterpret_cast<void*>(entryIndex), static_cast<int>(param)));

    if (object)
    {
        SceneLoader_RegisterObject(self, nullptr, object, entryIndex);
        SceneLoader_SetupObject(self, nullptr, object, entryIndex, altMode);

        if (altMode)
            object->field_0170 = object->field_0170 | 0x400;
        else
            object->field_0170 = object->field_0170 & 0xFFFFFBFF;

        object->vSlot1C();

        int globalState = GetGlobalState_50D190();
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(globalState), 0xD0) != 0)
        {
            int objectType = static_cast<int>(object->field_0050);
            if (objectType == 12 || static_cast<uint8_t>(IsResolvedEntryActive(objectType)) != 0)
            {
                object->field_0170 &= ~0x200u;
            }
        }
    }
    return object;
}

HOOK(0x004EEEF0, S_004e9090_ResolveObject, "auto");
