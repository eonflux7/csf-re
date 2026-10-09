#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, ColorAnimBufferInit, 0x00412AB0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ApplyColorAnimToControl, 0x00445050, (void* self, void* edx, void* arg0, uint32_t* a2));
CSF_FUNC(int, __fastcall, SceneReleaseHandle, 0x00453390, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, CGuiControl_GetColorAnim, 0x004FE4B0, (CGuiControl* self, void*));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void* AsPtr(uint32_t value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(value));
}

static int __stdcall ApplyColorAnimForHandleEvent(int unusedArg1, int unusedArg2, uint32_t* eventRef)
{
    // eventRef -> node; node+4 -> handle record; record+12 -> handle index (signed)
    uint32_t nodeAddr = *eventRef;
    uint32_t handleRecord = CSF_AT(uint32_t, AsPtr(nodeAddr), 4);
    int recordIndex = CSF_AT(int, AsPtr(handleRecord), 12);
    uint32_t handleAddr = handleRecord + 4;

    int result = recordIndex;
    if (recordIndex >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleAddr));
        if (result != 0)
        {
            void* entry = AsPtr(static_cast<uint32_t>(result));
            if (CSF_AT(int, entry, 0x50) != 26)
            {
                uint32_t animatorRef = CSF_AT(uint32_t, entry, 0x38);
                if (animatorRef != 0)
                {
                    void* animator = AsPtr(animatorRef);
                    auto slotFn = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, animator, 0)[41]);
                    slotFn(animator, nullptr, 0);

                    uint32_t scratch[11];
                    ColorAnimBufferInit(scratch, nullptr);
                    int guiValue = GetGlobalValue_8B0568();
                    int colorAnim = CGuiControl_GetColorAnim(
                        reinterpret_cast<CGuiControl*>(AsPtr(static_cast<uint32_t>(guiValue))), nullptr);
                    ApplyColorAnimToControl(AsPtr(static_cast<uint32_t>(colorAnim)), nullptr, entry, scratch);
                }
                return SceneReleaseHandle(g_pCurrentScene, nullptr, static_cast<int>(handleAddr));
            }
        }
    }
    return result;
}

HOOK(0x00462460, ApplyColorAnimForHandleEvent, "auto");
