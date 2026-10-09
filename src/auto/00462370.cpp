#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_4619C0, 0x004619C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4686A0, 0x004686A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4686F0, 0x004686F0, (void* self, void* edx, int a2, int a3));

// Vtable of the S_004686a0 object referenced by the original's call site at 0x4623DF.
[[maybe_unused]] static void* const s_SceneDispatchVtableRef = CSF_VTBL(0x004686A0);

// Resolves the handle stored in the message, looks the target up in the current
// scene's manager, and either clears the flag on the found object or forwards to
// the fallback handler.
static int __stdcall Scene_DispatchHandleEvent(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    // v3 = *(*a3 + 4)
    uint8_t* outer = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3[0]));
    uint8_t* header = *reinterpret_cast<uint8_t**>(outer + 4);

    // v4 = header->field_000C (signed test)
    int32_t handleField = *reinterpret_cast<int32_t*>(header + 12);
    int result = reinterpret_cast<int>(header + 4);

    if (handleField >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result)
        {
            // *(*(*(a3[1] + 4)) + 4): read through the second message pointer.
            uint8_t* payload = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3[1]));
            uint8_t* payloadInner = *reinterpret_cast<uint8_t**>(payload + 4);
            int targetArg = *reinterpret_cast<int32_t*>(payloadInner + 4);

            void* manager = *CSF_AT(void**, g_pCurrentScene, 0xBC);
            int v6 = sub_4686F0(manager, nullptr, result, targetArg);
            if (v6)
            {
                result = sub_4619C0(reinterpret_cast<void*>(static_cast<uintptr_t>(v6)), nullptr);
                *reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(v6) + 0x80) = 0;
            }
            else
            {
                uint8_t* payloadAgain = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3[1]));
                uint8_t* payloadInnerAgain = *reinterpret_cast<uint8_t**>(payloadAgain + 4);
                int fallbackArg = *reinterpret_cast<int32_t*>(payloadInnerAgain + 4);

                void* managerAgain = *CSF_AT(void**, g_pCurrentScene, 0xBC);
                return sub_4686A0(managerAgain, nullptr, fallbackArg);
            }
        }
    }
    return result;
}

HOOK(0x00462370, Scene_DispatchHandleEvent, "auto");
