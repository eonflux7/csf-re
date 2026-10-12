#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00830578_ResetMenuController, 0x0042CAB0, (C_00830578* self, void*));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuSystem_UpdateDialogPanel, 0x00712CE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MenuSystem_AssignDialogPanel, 0x00712E80, (void* self, void* edx, int panel));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __stdcall, NetSession_GetEntryFlags, 0x0073CCE0, (int a1));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(int, g_Value8B055C, 0x008B055C);

static uint32_t* __fastcall C_0083f1f8_ActivateCheatOnObject(
    C_0083f1f8* self, void* /*edx*/, int activationMode)
{
    uint32_t* result = nullptr;
    self->field_0104 = -1;

    if (activationMode == 1)
    {
        result = reinterpret_cast<uint32_t*>(
            GetSceneMenuContext(g_pCurrentScene, nullptr));

        if (result && CSF_AT(int, result, 0x248) >= 0)
        {
            const int handleAddress = reinterpret_cast<int>(
                reinterpret_cast<uint8_t*>(result) + 0x240);

            result = reinterpret_cast<uint32_t*>(
                CHandleTable_Resolve(g_pCurrentScene, nullptr, handleAddress));

            if (result && CSF_AT(uint32_t, result, 0x50) == 18)
            {
                result = CSF_AT(uint32_t*, result, 0x18);
                if (result)
                {
                    const int entry = reinterpret_cast<int>(result);
                    GetGlobalDword_73C020();
                    const int flags = NetSession_GetEntryFlags(entry);
                    result = reinterpret_cast<uint32_t*>(flags);
                    self->field_0104 = static_cast<uint32_t>(flags);
                }
            }
        }

        if (self->field_0104 == static_cast<uint32_t>(-1))
            return result;
    }

    self->field_0012 = 0;
    self->field_00dc = 0;

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    C_00830578_ResetMenuController(
        CSF_AT(C_00830578*, globalState, 0x18), nullptr);

    globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    C_00830578* controller = CSF_AT(C_00830578*, globalState, 0x18);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, controller, 0)[0x10 / 4])(controller, nullptr, 0);

    int panel = g_Value8B055C;
    void* referencedObject = GetReferencedObject(self, nullptr);
    MenuSystem_AssignDialogPanel(referencedObject, nullptr, panel);

    referencedObject = GetReferencedObject(self, nullptr);
    self->field_000c = CSF_AT(uint32_t, referencedObject, 0x18);

    panel = g_Value8B055C;
    referencedObject = GetReferencedObject(self, nullptr);
    result = reinterpret_cast<uint32_t*>(
        MenuSystem_UpdateDialogPanel(referencedObject, nullptr, panel));

    self->field_0100 = activationMode;
    self->field_0010 = 1;
    self->field_0109 = 0;
    return result;
}

HOOK(0x004F98A0, C_0083f1f8_ActivateCheatOnObject, "auto");
