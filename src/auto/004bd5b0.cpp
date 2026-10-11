#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Fills a 16-byte result object (vtable, two handle-table range values, scene tag) for the debug IA window.
static uint32_t* __fastcall S_004bd310_FillDebugSceneHandleInfo(S_004bd310* self, void* /*edx*/, uint32_t* outObj, int handle)
{
    void* scene = g_pCurrentScene;
    int sceneTag = CSF_AT(int, CSF_AT(void*, scene, 0xB0), 8);

    int resultA = -1;
    int resultB = -1;

    void* ownerObj = reinterpret_cast<void*>(self->field_0000);
    if (CSF_AT(int, ownerObj, 0x20) != CSF_AT(int, reinterpret_cast<void*>(handle), 4))
    {
        void* entry = reinterpret_cast<void*>(CHandleTable_Resolve(scene, nullptr, handle));
        if (entry != nullptr)
        {
            void* control = CSF_AT(void*, entry, 0x38);
            if (control != nullptr)
            {
                int controlBusy = ((int(__fastcall*)(void*, void*))CSF_AT(void**, control, 0)[31])(control, nullptr);
                if (controlBusy == 0)
                {
                    void* list = CSF_AT(void*, entry, 0x38);
                    int count = CSF_AT(int, list, 0x3C);
                    if (count != 0)
                    {
                        int* elements = CSF_AT(int*, list, 0x34);
                        int index3 = 3 * count;
                        resultA = *reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(elements) + 4 * index3 - 8);
                        resultB = *reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(elements) + 4 * index3 - 4);
                    }
                }
            }
        }
    }

    outObj[1] = static_cast<uint32_t>(resultA);
    outObj[2] = static_cast<uint32_t>(resultB);
    *reinterpret_cast<void***>(outObj) = CSF_VTBL(0x0082D448);
    outObj[3] = static_cast<uint32_t>(sceneTag);
    return outObj;
}

HOOK(0x4BD5B0, S_004bd310_FillDebugSceneHandleInfo, "auto");
