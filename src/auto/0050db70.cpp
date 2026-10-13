#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(void, __fastcall, ForwardCallToChildVirtualSlot3, 0x006B1570, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, ForwardToChildSlot5At8C, 0x006B1590, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ProcessObjectsAtOrBeyondThreshold, 0x0071AB00, (int* self, void*));

static hook::Tail __fastcall S_0050f2f0_ResetSceneEntries(
    void* self, const uint32_t* /*args*/)
{
    auto* sceneLoader = static_cast<S_0050f2f0*>(self);

    if (ForwardToChildSlot5At8C(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr))
    {
        void* object = MenuControls_ResolveObject(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);

        if (((bool(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[0x74 / 4])(object, nullptr, 0))
        {
            ForwardCallToChildVirtualSlot3(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr);

            object = MenuControls_ResolveObject(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr);
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x78 / 4])(object, nullptr);
        }
    }

    void* object = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    bool resetPending = ((bool(__fastcall*)(void*, void*))
        CSF_AT(void**, object, 0)[0x6C / 4])(object, nullptr);

    if (!resetPending)
        return hook::tail_ret(0);

    if (!ForwardToChildSlot5At8C(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr))
    {
        ForwardCallToChildVirtualSlot3(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    }

    ProcessObjectsAtOrBeyondThreshold(
        reinterpret_cast<int*>(sceneLoader->field_0014), nullptr);

    object = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    return hook::tail_jump(
        object, (*reinterpret_cast<void***>(object))[0x70 / 4]);
}

HOOK_TAIL(0x0050DB70, S_0050f2f0_ResetSceneEntries, "auto");
