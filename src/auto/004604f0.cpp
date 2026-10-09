#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// CHandleTable::Resolve(self = g_pCurrentScene, handle) -> object pointer or 0.
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Looks up the scene object for a handle record and, when its component at +0x34
// reports state 1 (virtual slot 0x98), copies the value from the source record
// into the component's +0x2F8 field (760 decimal). Returns the last result computed.
static int __stdcall SceneHandle_CopyAltFieldFromRecord(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    // v3 = *(*a3 + 4): record reached through the pointer stored in a3[0].
    uint8_t* recordBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3[0]));
    uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, recordBase, 4)));

    int32_t recordFlag = CSF_AT(int32_t, record, 12);
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(record + 4));

    if (recordFlag >= 0)
    {
        int handleObject = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (handleObject != 0)
        {
            uint8_t* sceneObject = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(handleObject));
            uint32_t component = CSF_AT(uint32_t, sceneObject, 52);
            if (component != 0)
            {
                void* componentObj = reinterpret_cast<void*>(static_cast<uintptr_t>(component));
                // Virtual slot 0x98 / 4 = 38, __thiscall with no extra args.
                result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, componentObj, 0)[38])(componentObj, nullptr);
                if (result == 1)
                {
                    // eax = a3[1]; ecx = [eax+4]; result = [ecx+4]
                    uint8_t* sourceRecord = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3[1]));
                    uint8_t* sourceInner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, sourceRecord, 4)));
                    result = static_cast<int>(CSF_AT(uint32_t, sourceInner, 4));

                    // Reload the component pointer from the scene object after the call.
                    uint32_t componentAfterCall = CSF_AT(uint32_t, sceneObject, 52);
                    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(componentAfterCall)), 0x2F8) =
                        static_cast<uint32_t>(result);
                }
            }
        }
    }
    return result;
}

HOOK(0x004604F0, SceneHandle_CopyAltFieldFromRecord, "auto");
