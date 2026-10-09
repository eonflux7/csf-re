#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Virtual slot signatures. Slots are __thiscall; written as __fastcall with a dummy edx.
using SlotReturnsPointerFn = int(__fastcall*)(void* self, void* edx);
using SlotReturnsByteFn    = uint8_t(__fastcall*)(void* self, void* edx);
using SlotReturnsIndexFn   = int(__fastcall*)(void* self, void* edx, int value);
using SlotTakesThreeFn     = int(__fastcall*)(void* self, void* edx, void* object, int value, int flag);
using SlotVoidFn           = void(__fastcall*)(void* self, void* edx);

static int __stdcall TransferResolvedObjectToContainer(int /*a2*/, int /*a3*/, uint32_t* a4)
{
    // a4[0] -> record; record+4 -> source handle node; its +0xC is a signed status.
    uint8_t* sourceHandleNode = CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(a4[0]), 4);
    // a4[1] -> record; record+4 -> target handle node; its +0xC is a signed status.
    uint8_t* targetHandleNode = CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(a4[1]), 4);
    int sourceHandle = static_cast<int>(reinterpret_cast<uintptr_t>(sourceHandleNode) + 4);
    int targetHandle = static_cast<int>(reinterpret_cast<uintptr_t>(targetHandleNode) + 4);
    int result = sourceHandle;

    if (CSF_AT(int, sourceHandleNode, 0xC) < 0 || CSF_AT(int, targetHandleNode, 0xC) < 0)
        return result;

    void* sourceObject = reinterpret_cast<void*>(CHandleTable_Resolve(g_pCurrentScene, nullptr, sourceHandle));
    result = CHandleTable_Resolve(g_pCurrentScene, nullptr, targetHandle);
    void* targetObject = reinterpret_cast<void*>(result);

    // Component at vtable slot 0x28 of each resolved object.
    void* sourceComponent = nullptr;
    if (sourceObject != nullptr)
    {
        result = ((SlotReturnsPointerFn)CSF_AT(void**, sourceObject, 0)[10])(sourceObject, nullptr);
        sourceComponent = reinterpret_cast<void*>(result);
    }
    void* targetComponent = nullptr;
    if (targetObject != nullptr)
    {
        result = ((SlotReturnsPointerFn)CSF_AT(void**, targetObject, 0)[10])(targetObject, nullptr);
        targetComponent = reinterpret_cast<void*>(result);
    }
    if (sourceComponent == nullptr || targetComponent == nullptr)
        return result;

    // Skip when the source has a child (slot 0x78) whose slot 0x28 returns a nonzero byte.
    result = ((SlotReturnsPointerFn)CSF_AT(void**, sourceComponent, 0)[30])(sourceComponent, nullptr);
    bool shouldTransfer = (result == 0);
    if (!shouldTransfer)
    {
        void* sourceChild = reinterpret_cast<void*>(
            ((SlotReturnsPointerFn)CSF_AT(void**, sourceComponent, 0)[30])(sourceComponent, nullptr));
        result = ((SlotReturnsPointerFn)CSF_AT(void**, sourceChild, 0)[10])(sourceChild, nullptr);
        shouldTransfer = ((uint8_t)result == 0);
    }
    if (!shouldTransfer)
        return result;

    // a4[2] -> record; record+4 -> node; node+4 -> value. Re-read through a4 at each use.
    result = ((SlotReturnsIndexFn)CSF_AT(void**, targetComponent, 0)[38])(
        targetComponent, nullptr,
        CSF_AT(int, CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(a4[2]), 4), 4));
    if (result != -1)
    {
        if (((SlotReturnsByteFn)CSF_AT(void**, sourceComponent, 0)[42])(sourceComponent, nullptr))
            ((SlotVoidFn)CSF_AT(void**, sourceComponent, 0)[41])(sourceComponent, nullptr);

        return ((SlotTakesThreeFn)CSF_AT(void**, targetComponent, 0)[40])(
            targetComponent, nullptr, sourceComponent,
            CSF_AT(int, CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(a4[2]), 4), 4), 0);
    }
    return result;
}

HOOK(0x004601A0, TransferResolvedObjectToContainer, "auto");
