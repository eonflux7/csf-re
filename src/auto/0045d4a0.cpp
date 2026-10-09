#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Callee: __thiscall-style lookup on the scene's 47th slot object (ecx=this, one stack arg).
CSF_FUNC(int, __fastcall, Scene_LookupEntryByKey, 0x00468750, (void* self, void* edx, int a2));

// Vtable of the entry type returned by Scene_LookupEntryByKey.
static void* const kSceneEntryVtbl [[maybe_unused]] = CSF_VTBL(0x00468750);

static int __stdcall Scene_LookupAndMarkEntry([[maybe_unused]] int a1, [[maybe_unused]] int a2, int a3)
{
    // a3 points at a record whose first dword points at a node; node+4 holds the key pointer.
    uint32_t* argRecord = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a3));
    uint32_t* node = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(argRecord[0]));
    uint32_t keyHolder = node[1];
    uint32_t* keyHolderPtr = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(keyHolder));
    int key = static_cast<int>(keyHolderPtr[1]);

    void* sceneSlotObject = CSF_AT(void*, g_pCurrentScene, 0xBC); // scene->[47]

    int result = Scene_LookupEntryByKey(sceneSlotObject, nullptr, key);
    if (result != 0) {
        *reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result) + 0x80) = 1;
    }
    return result;
}

HOOK(0x0045D4A0, Scene_LookupAndMarkEntry, "auto");
