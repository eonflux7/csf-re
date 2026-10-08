#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(const char*, g_pszMenu_P_EstaMP, 0x00899B18);

CSF_FUNC(void*, __cdecl, GetGlobalState, 0x0050D190, ());
CSF_FUNC(void*, __fastcall, C_unk50E280_GetInnerField4C, 0x0050E280, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, InnerField4C_Update, 0x005322A0, (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, MpCharSelect_CleanupSlot, 0x0042CAB0, (int** self, void* edx));
CSF_FUNC(int, __stdcall, MpCharSelect_ApplyPreviewFlag, 0x0042DF80, (int a1));
CSF_FUNC(void*, __cdecl, GetGlobalSubsystem8AFC78, 0x0042D6D0, ());
CSF_FUNC(void*, __cdecl, GetGlobalValue8B055C, 0x004F9660, ());

// Sets the MP character-select preview parameters, then (on first entry, i.e. when
// the cached text slot at self[1] is empty) builds the menu label and resets the
// related global state. Returns the cached slot value or the global value's field.
static int __fastcall MenuMpCharSelect_SetPreviewAndInit(int* self, void* /*edx*/, char a2, int a3)
{
    void* globalState = GetGlobalState();
    void* inner = C_unk50E280_GetInnerField4C(globalState, nullptr);
    CSF_AT(uint32_t, inner, 0x1E4) = static_cast<uint32_t>(a3);
    CSF_AT(uint8_t, inner, 0x1E0) = static_cast<uint8_t>(a2);
    InnerField4C_Update(inner, nullptr, 1);

    int result = self[1];
    if (result == 0)
    {
        if (a2 != 0)
        {
            void* state = GetGlobalState();
            MpCharSelect_CleanupSlot(CSF_AT(int**, state, 0x18), nullptr);
        }

        void* owner = reinterpret_cast<void*>(self[0]);
        void* ownerSub = CSF_AT(void*, owner, 0x18);
        void** vtable = CSF_AT(void**, ownerSub, 0);
        using LabelFn = int(__fastcall*)(void*, void*, const char*);
        self[1] = reinterpret_cast<LabelFn>(vtable[0x58 / 4])(ownerSub, nullptr, g_pszMenu_P_EstaMP);

        GetGlobalSubsystem8AFC78();
        MpCharSelect_ApplyPreviewFlag(0);

        void* valueObj = GetGlobalValue8B055C();
        CSF_AT(uint8_t, valueObj, 0xF8) = 0;
        result = static_cast<int>(reinterpret_cast<uintptr_t>(valueObj));
    }
    return result;
}

HOOK(0x00542640, MenuMpCharSelect_SetPreviewAndInit, "auto");
