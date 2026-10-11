#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_004f5420_SelectModelEntry, 0x004F54A0, (void* self, void* edx, int a2, char a3, char a4, char a5));
CSF_FUNC(int, __fastcall, ObjectProvider_SetUpdateMode, 0x004F7810, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetArrayElementAt38, 0x004FBFC0, (void* self, void* , int index));

static int __fastcall S_004f5420_UpdateFromSceneObject(
    S_004f5420* self, void* /*edx*/, C_00833b88* sceneObject, int /*unused*/)
{
    if (static_cast<uint8_t>(SceneObj_IsInactive(sceneObject, nullptr)) == 0 &&
        static_cast<uint8_t>(SceneObject_PassesFirstCheck(sceneObject, nullptr)) == 0)
    {
        const uint32_t flags = CSF_AT(uint32_t, sceneObject, 0x2E0);
        if (self->field_0008 == 0)
        {
            if ((flags & 1) == 0 && (flags & 0x40) != 0)
            {
                const int priority = self->field_0018 ? 0 : -1;
                self->field_0020 = 0;

                void* provider = reinterpret_cast<void*>(self->field_0010);
                void* current = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, provider, 0)[0x68 / 4])(provider, nullptr);

                if (priority == 0 ||
                    priority >= static_cast<int32_t>(self->field_001c) ||
                    reinterpret_cast<bool (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, current, 0)[0x40 / 4])(current, nullptr, 0))
                {
                    self->field_001c = priority;
                    S_004f5420_SelectModelEntry(self, nullptr, -1, 1, 0, 0);
                }
            }
            else
            {
                int entryIndex;
                if ((flags & 1) != 0)
                {
                    if ((flags & 2) != 0)
                        entryIndex = 1;
                    else if ((flags & 4) != 0)
                        entryIndex = 3;
                    else
                        entryIndex = 2;
                }
                else
                {
                    entryIndex = 7;
                }

                void* provider = reinterpret_cast<void*>(self->field_0010);
                self->field_0020 = 0;
                void* current = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, provider, 0)[0x68 / 4])(provider, nullptr);

                if (static_cast<int32_t>(self->field_001c) <= 1 ||
                    reinterpret_cast<bool (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, current, 0)[0x40 / 4])(current, nullptr, 0))
                {
                    self->field_001c = 1;
                    void* entries = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
                    const int entry = GetArrayElementAt38(entries, nullptr, entryIndex);
                    S_004f5420_SelectModelEntry(self, nullptr, entry, 1, 0, 0);
                }
            }
        }
    }

    const int result = static_cast<int32_t>(self->field_0008);
    if (result != 1 && result != 8)
    {
        if (CSF_AT(uint8_t, sceneObject, 0x229) != 0)
        {
            if (result != 2)
                return ObjectProvider_SetUpdateMode(self, nullptr, 2);
        }
        else if (result != 0)
        {
            return ObjectProvider_SetUpdateMode(self, nullptr, 0);
        }
    }
    return result;
}

HOOK(0x004F7940, S_004f5420_UpdateFromSceneObject, "auto");
