#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiCore_GetSceneEntry, 0x0040F790, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, AiCore_UpdateSceneBinding, 0x00449640, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char*, __fastcall, AiCore_GetCoverAttackState, 0x0047AE10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CoverAttack_SetMode_47B210, 0x0047B210, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, C_0082f9f8_SetCoverAttackParameter, 0x0047BF40,
         (C_0082f9f8* self, void* edx, int a2, uint8_t a3));
CSF_FUNC(int, __fastcall, AiCore_CopyCoverAttackState, 0x004D16C0,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_ApplyLinkedPosition, 0x004BAD20,
         (void* self, void* edx, int a2, char a3, uint32_t* a4, int a5, char a6));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static const auto g_CoverAttackStateVtable = CSF_VTBL(0x0047AE10);

static int __stdcall AiCore_SynchronizeLinkedObjectWithScene(C_0085e7b8* self)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    void* linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);

    if (CSF_AT(int, linkedObject, 0x608) != -1 ||
        CSF_AT(int, linkedObject, 0x60C) != -1)
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        void* stateOwner = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);

        linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
        void* coverAttack = CSF_AT(void*, linkedObject, 0x718);
        auto* destination = reinterpret_cast<uint32_t*>(
            reinterpret_cast<uintptr_t>(stateOwner) + 1612);

        char* source = AiCore_GetCoverAttackState(coverAttack, nullptr);
        AiCore_CopyCoverAttackState(source, nullptr, destination);
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    void* actor = CSF_AT(void*, linkedObject, 0x2C);

    if (CSF_AT(int, actor, 0x1D8) == 3)
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
        CoverAttack_SetMode_47B210(CSF_AT(void*, linkedObject, 0x718), nullptr, 0);

        linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
        C_0082f9f8_SetCoverAttackParameter(
            CSF_AT(C_0082f9f8*, linkedObject, 0x718), nullptr, 1120403456, 0);
    }

    void* scene = g_pCurrentScene;
    int linkedData = GetLinkedObjectField10(self, nullptr);
    void* sceneObject = reinterpret_cast<void*>(
        CHandleTable_Resolve(scene, nullptr, linkedData + 20));

    void* sceneEntry = reinterpret_cast<void*>(
        AiCore_GetSceneEntry(CSF_AT(void*, sceneObject, 0x4C), nullptr, 0));

    void* manager =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, sceneObject, 0)[0x28 / 4])(sceneObject, nullptr);

    int entryId = CSF_AT(int, sceneEntry, 0x4);
    int resolvedId =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, manager, 0)[0x98 / 4])(manager, nullptr, entryId);

    manager =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, sceneObject, 0)[0x28 / 4])(sceneObject, nullptr);

    int result =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, manager, 0)[0x8C / 4])(manager, nullptr, resolvedId);

    if (!result)
        return result;

    void* resolvedObject = reinterpret_cast<void*>(result);
    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    actor = CSF_AT(void*, linkedObject, 0x2C);

    C_0085e7b8* sceneBindings = CSF_AT(C_0085e7b8*, g_pCurrentScene, 0xC4);
    linkedData = GetLinkedObjectField10(self, nullptr);
    AiCore_UpdateSceneBinding(
        &sceneBindings->field_0004, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(actor) + 28),
        linkedData + 20);

    void* transform =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, resolvedObject, 0)[0x18 / 4])(resolvedObject, nullptr);
    uint32_t* position =
        reinterpret_cast<uint32_t*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);

    linkedData = GetLinkedObjectField10(self, nullptr);
    uint32_t* destination = reinterpret_cast<uint32_t*>(linkedData + 32);
    destination[0] = position[0];
    destination[1] = position[1];
    destination[2] = position[2];

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    int linkedBase = linkedValue ? linkedValue - 4 : 0;
    UnnamedApply_4DC180(self, nullptr, linkedBase + 432);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedBase = linkedValue ? linkedValue - 4 : 0;
    linkedData = GetLinkedObjectField10(self, nullptr);
    position = reinterpret_cast<uint32_t*>(linkedData + 32);
    void* provider = GetCachedObjectProvider(self, nullptr);

    return AiCore_ApplyLinkedPosition(
        reinterpret_cast<void*>(linkedBase + 432), nullptr,
        reinterpret_cast<int>(provider), 0, position, 0, 0);
}

HOOK(0x004BD030, AiCore_SynchronizeLinkedObjectWithScene, "auto");
