#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Bound callees (names are chosen here; signatures are fixed by the original).
CSF_FUNC(int, __fastcall, Scene_RemoveObjectSlot, 0x00453390, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Scene_IsObjectActiveType, 0x00755810, (unsigned int a1));
CSF_FUNC(int, __cdecl, Scene_IsObjectRemovable, 0x007558C0, (int a1));

// Walks the current scene's object array (count at +0x18, items at +0x10).
// Objects flagged removable get their slot removed; active-type objects with
// flag 0x8 at +0x1F0 receive a message through the vtable slot at +0x48.
static unsigned int __stdcall Scene_DispatchObjectsByState(int a1, int a2, int a3)
{
    (void)a1;
    (void)a2;
    (void)a3;

    const uint32_t objectCount = CSF_AT(uint32_t, g_pCurrentScene, 0x18);
    unsigned int result = objectCount;
    if (objectCount == 0)
        return result;

    uint32_t index = 0;
    do
    {
        uint32_t* itemArray = CSF_AT(uint32_t*, g_pCurrentScene, 0x10);
        uint8_t* object = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(itemArray[index]));

        if (object != nullptr)
        {
            const uint32_t objectKind = CSF_AT(uint32_t, object, 0x50);

            if (static_cast<uint8_t>(Scene_IsObjectRemovable(static_cast<int>(objectKind))))
            {
                Scene_RemoveObjectSlot(g_pCurrentScene, nullptr,
                                       static_cast<int>(reinterpret_cast<uintptr_t>(object + 0x1C)));
            }
            else if (static_cast<uint8_t>(Scene_IsObjectActiveType(objectKind)))
            {
                if (CSF_AT(uint8_t, object, 0x1F0) & 8)
                {
                    uint8_t* subObject = object + 4;
                    void** vtable = *reinterpret_cast<void***>(subObject);

                    uint32_t message[3];
                    message[0] = 8;
                    message[1] = 0;
                    message[2] = 0;

                    auto slotFn = reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(vtable[0x48 / 4]);
                    slotFn(subObject, nullptr, message);
                }
            }
        }

        result = objectCount;
        ++index;
    } while (index < objectCount);

    return result;
}

HOOK(0x004618A0, Scene_DispatchObjectsByState, "auto");
