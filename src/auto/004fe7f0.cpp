#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Binding of the unnamed stdcall helper that initializes one scene element (ret 4).
CSF_FUNC(int, __stdcall, SceneElement_Initialize, 0x004FDCA0, (int a1));

// Binding of the cdecl getter whose result is the `this` of the release call.
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

// Binding of the fastcall helper that releases an element handle (ret 4).
CSF_FUNC(unsigned int, __fastcall, ObjectHandle_Release, 0x006B1840, (void* self, void* edx, int a2));

// Sets the load-state byte at +8 and syncs every element of the array at +0x3c
// (count at +0x44, element stride 0x44, per-element handle at +0x40).
// Returns the element count as the original does.
static unsigned int __fastcall SceneLoader_SetLoadedAndSyncElements(void* self, void* /*edx*/, char a2)
{
    CSF_AT(uint8_t, self, 0x8) = static_cast<uint8_t>(a2);
    uint32_t result = CSF_AT(uint32_t, self, 0x44);

    if (a2) {
        // Load phase: initialize every element that has no handle yet.
        if (result) {
            uint32_t elementIndex = 0;
            uint32_t byteOffset = 0;
            do {
                uint8_t* elementBase = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x3c));
                uint8_t* element = elementBase + byteOffset;
                uint32_t handle = CSF_AT(uint32_t, element, 0x40);
                if (!handle) {
                    SceneElement_Initialize(static_cast<int>(reinterpret_cast<uintptr_t>(element)));
                }
                result = CSF_AT(uint32_t, self, 0x44);
                ++elementIndex;
                byteOffset += 0x44;
            } while (elementIndex < result);
        }
    } else {
        // Unload phase: release every element that holds a handle.
        uint32_t counter = 0;
        uint32_t byteOffset = 0;
        if (result) {
            do {
                uint8_t* elementBase = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x3c));
                uint8_t* element = elementBase + byteOffset;
                int handle = CSF_AT(int, element, 0x40);
                if (handle) {
                    int globalValue = GetGlobalInt();
                    ObjectHandle_Release(reinterpret_cast<void*>(globalValue), nullptr, handle);
                    CSF_AT(uint32_t, element, 0x40) = 0;
                }
                result = counter + 1;
                byteOffset += 0x44;
                ++counter;
            } while (counter < CSF_AT(uint32_t, self, 0x44));
        }
    }

    return result;
}

HOOK(0x004FE7F0, SceneLoader_SetLoadedAndSyncElements, "auto");
