#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, IsObjectReadyForCommand, 0x00755810, (unsigned int a1));

static int __fastcall CommandHandler_ApplyResolvedTarget(void* self, void* /*edx*/,
                                                         int /*a2*/, int /*a3*/, uint32_t* a4Ptr)
{
    uint8_t* sourceRecord = reinterpret_cast<uint8_t*>(*a4Ptr);
    uint8_t* sourceData = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, sourceRecord, 4));
    int result = CSF_AT(int, sourceData, 12);

    if (result >= 0)
    {
        uint8_t* handleSlot = sourceData + 4;

        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<int>(handleSlot));
        uint8_t* control = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));
        if (result != 0)
        {
            result = CSF_AT(int, control, 0x34);
            if (result != 0)
            {
                result = IsObjectReadyForCommand(CSF_AT(uint32_t, control, 0x50));
                if (static_cast<uint8_t>(result) != 0)
                {
                    result = CSF_AT(int, control, 0x1C4);
                    if (result == 0)
                    {
                        void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, control, 0x34));
                        uint32_t requestCode = 6;
                        result = ((int(__fastcall*)(void*, void*, uint32_t*))
                                  CSF_AT(void**, obj, 0)[42])(obj, nullptr, &requestCode);

                        CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, handleSlot, 4);
                        CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, handleSlot, 8);
                        CSF_AT(uint8_t, self, 0x82) = 0;
                    }
                }
            }
        }
    }
    return result;
}

HOOK(0x0045ED70, CommandHandler_ApplyResolvedTarget, "auto");
