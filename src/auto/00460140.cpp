#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// __thiscall (ecx = self), 3 stack args, ret 12.
static int __fastcall NodeHandle_ApplyTargetAndClearFlag(void* self, void* /*edx*/,
                                                         [[maybe_unused]] int a2,
                                                         [[maybe_unused]] int a3,
                                                         void* a4)
{
    uint8_t* senderNode = *reinterpret_cast<uint8_t**>(a4);
    uint8_t* v5 = *reinterpret_cast<uint8_t**>(senderNode + 4);
    int32_t result = *reinterpret_cast<int32_t*>(v5 + 12);
    uint8_t* handleSlot = v5 + 4;

    if (result >= 0) {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                      static_cast<int>(reinterpret_cast<uintptr_t>(handleSlot)));
        if (result) {
            void* resolved = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
            void* controlObj = CSF_AT(void*, resolved, 0x34);
            if (controlObj) {
                int32_t opcode = 31;
                // vtable slot 0xA8 / 4 = 42, __thiscall with one stack arg
                result = reinterpret_cast<int(__fastcall*)(void*, void*, int32_t*)>(
                             CSF_AT(void**, controlObj, 0)[42])(controlObj, nullptr, &opcode);

                CSF_AT(uint32_t, self, 0x60) = *reinterpret_cast<uint32_t*>(handleSlot + 4);
                CSF_AT(uint32_t, self, 0x64) = *reinterpret_cast<uint32_t*>(handleSlot + 8);
                CSF_AT(uint8_t, self, 0x82) = 0;
            }
        }
    }
    return result;
}

HOOK(0x00460140, NodeHandle_ApplyTargetAndClearFlag, "auto");
