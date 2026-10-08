#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x8AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x44FC30, (void* self, void* edx, int handleKey));

static int __fastcall ForwardMoveOrderToSceneEntity(uint8_t* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* a4)
{
    // a4[0] -> record whose word at +4 is the address of the scene entry.
    uint32_t* entryRef = reinterpret_cast<uint32_t*>(a4[0]);
    uint32_t entryAddress = entryRef[1];
    uint8_t* entry = reinterpret_cast<uint8_t*>(entryAddress);

    int result = CSF_AT(int32_t, entry, 12);
    if (result >= 0)
    {
        // Embedded sub-object of the entry; its address doubles as the handle key.
        uint32_t handleKey = entryAddress + 4;
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleKey));
        if (result != 0)
        {
            uint8_t* resolved = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));
            uint32_t targetObjectAddress = CSF_AT(uint32_t, resolved, 52);
            if (targetObjectAddress != 0)
            {
                if (CSF_AT(int32_t, resolved, 80) == 4)
                {
                    // a4[1] -> record whose word at +4 points to a vector triple.
                    uint32_t* commandRef = reinterpret_cast<uint32_t*>(a4[1]);
                    uint8_t* vectorSource = reinterpret_cast<uint8_t*>(commandRef[1]) + 4;

                    int32_t messageArgs[4] = {30, 0, 0, 0};
                    messageArgs[1] = CSF_AT(int32_t, vectorSource, 0);
                    messageArgs[2] = CSF_AT(int32_t, vectorSource, 4);
                    messageArgs[3] = CSF_AT(int32_t, vectorSource, 8);

                    uint8_t* targetObject = reinterpret_cast<uint8_t*>(targetObjectAddress);
                    // vtbl slot 0xA8 / 4 = 42; no typed wrapper known for this object.
                    result = ((int(__fastcall*)(void*, void*, int32_t*))CSF_AT(void**, targetObject, 0)[42])(targetObject, nullptr, messageArgs);

                    // Re-read the embedded sub-object after the call, as the original does.
                    uint8_t* handleSlot = reinterpret_cast<uint8_t*>(handleKey);
                    CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, handleSlot, 4);
                    CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, handleSlot, 8);
                    CSF_AT(uint8_t, self, 0x82) = 0;
                }
            }
        }
    }
    return result;
}

HOOK(0x4600A0, ForwardMoveOrderToSceneEntity, "auto");
