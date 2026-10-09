#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

struct CScene;

CSF_FUNC(int, __cdecl, GetGlobalValue_401090, 0x00401090, ());
CSF_FUNC(char, __fastcall, RemoveGlobalObjectReference, 0x00401230,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue_8afc80, 0x004419C0, ());
CSF_FUNC(int, __fastcall, Scene_RemoveReferenceLinks, 0x004528D0,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(void, __fastcall, SceneRegistry_RemoveObjectEntry, 0x00475480,
         (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneSubsystem_RemoveObject, 0x004DE7D0,
         (void* self, void* edx, void* a2));
CSF_FUNC(int, __stdcall, DestroyObject, 0x00750C00, (void* object));

static int __fastcall CScene_RemoveObjectReference(
    CScene* self, void* /*edx*/, int referenceAddress)
{
    void* reference = reinterpret_cast<void*>(referenceAddress);
    const int index = CSF_AT(int, reference, 0x8);
    if (index < 0 || index >= CSF_AT(int, self, 0x18))
        return index;

    auto* object = CSF_AT(C_00867260**, self, 0x10)[index];
    if (!object || CSF_AT(uint32_t, object, 0x20) !=
                       CSF_AT(uint32_t, reference, 0x4))
        return index;

    const int globalRegistry = GetGlobalValue_401090();
    RemoveGlobalObjectReference(
        reinterpret_cast<void*>(globalRegistry), nullptr, referenceAddress);

    Scene_RemoveReferenceLinks(
        self, nullptr, referenceAddress, CSF_AT(int, object, 0x94));

    CSF_AT(C_00867260**, self, 0x10)[CSF_AT(int, reference, 0x8)] = nullptr;
    object->field_0024 = -1;
    object->vSlot44(0);

    // This object's dynamic slot +0x28 is a no-argument getter, unlike
    // the generated C_00867260 wrapper for that slot.
    if (reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            object->vtbl[0x28 / 4])(object, nullptr))
    {
        void* attached = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            object->vtbl[0x28 / 4])(object, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, attached, 0)[0x34 / 4])(attached, nullptr, 0);
    }

    void* attached = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        object->vtbl[0x28 / 4])(object, nullptr);
    if (attached &&
        reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, attached, 0)[0xA8 / 4])(attached, nullptr))
    {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, attached, 0)[0xA4 / 4])(attached, nullptr);
    }

    SceneRegistry_RemoveObjectEntry(
        CSF_AT(void*, self, 0x184), nullptr,
        reinterpret_cast<int>(&object->field_0010));

    void* subsystem = CSF_AT(void*, self, 0x12C);
    if (subsystem)
        SceneSubsystem_RemoveObject(subsystem, nullptr, object);

    if (CSF_AT(int8_t, object, 0x170) >= 0)
    {
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            object->vtbl[0])(object, nullptr, 1);
    }

    GetValue_8afc80();
    return DestroyObject(object);
}

HOOK(0x00453560, CScene_RemoveObjectReference, "auto");
