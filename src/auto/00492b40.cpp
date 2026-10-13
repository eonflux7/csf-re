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

CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* /*edx*/, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_00833b88_DetachFromScene(C_00833b88* self, void* /*edx*/)
{
    // Call slot +0x9C on the child object stored at +0x38 (its type is not declared here).
    void* childObject = reinterpret_cast<void*>(self->field_0038);
    using ChildSlot9CFn = void(__fastcall*)(void* self, void* edx);
    ChildSlot9CFn childSlot9C = reinterpret_cast<ChildSlot9CFn>(CSF_AT(void**, childObject, 0)[0x9C / 4]);
    childSlot9C(childObject, nullptr);

    // Remove this object's slot (field at +0x1C) from the current scene.
    CScene* currentScene = reinterpret_cast<CScene*>(g_pCurrentScene);
    int slotAddress = static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x1C));
    return CScene_RemoveObjectSlot(currentScene, nullptr, slotAddress);
}

HOOK(0x00492B40, C_00833b88_DetachFromScene, "auto");
