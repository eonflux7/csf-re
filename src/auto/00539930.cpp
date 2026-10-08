// ===========================================================================
//  CMenuLoadGame::ActivateSelectedEntry  (0x00539930, tail block 0x00539720)
//
//  Handler for the "load game" menu: it first notifies the global notifier
//  object (GetGlobalValue_8B056C) and tells the embedded control that this
//  menu was entered, then (0x00539720, reached by tail `jmp` with ecx = this)
//  it asks the menu whether it may activate itself.  Depending on that gate
//  and on the "in game" flag inside the global value at 0x8B0568 (offset
//  0x5C == 1) it either starts the tactic game through the global state or
//  navigates back to the Juego / Campaña screens.
//
//  One path ends with an indirect tail jump (`jmp dword ptr [edx+0xA8]`,
//  0x005397FB) into the control's virtual method; it is reproduced as a naked
//  thunk so the caller's frame is forwarded untouched.
// ===========================================================================

#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// ---------------------------------------------------------------------------
// Constant data referenced by the original
// ---------------------------------------------------------------------------
CSF_GLOBAL(const char*, g_pszMenu_P_Campan, 0x00899AD8);
CSF_GLOBAL(const char*, g_pszMenu_P_Juego,  0x00899AF0);
CSF_GLOBAL(const char*, g_pszMenu_P_MeJueg, 0x00899AF4);
CSF_GLOBAL(const char*, g_pszMenu_P_Tactic, 0x00899B14);

// ---------------------------------------------------------------------------
// Functions called by the original (bound by address)
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x4FDBB0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190,        0x50D190, ());
CSF_FUNC(int, __fastcall, MenuSystem_ToggleChildFlag,  0x50D300, (int state, void* edx));
CSF_FUNC(int, __fastcall, CMenuMissionInfo_EnterPanelState, 0x53D740, (int item, void* edx, int arg));

// ---------------------------------------------------------------------------
// Indirect tail jump of 0x005397FB: entered with ecx = CMenuLoadGame*, it
// forwards control to the embedded control's virtual method at vtable +0xA8.
// (The original's `pop edi` is omitted: edi of our own frame is restored by
// our own epilogue.)
// ---------------------------------------------------------------------------
static __declspec(naked) int __fastcall CMenuLoadGame_TailDispatchToControl(void* self, void* /*edx*/)
{
    __asm
    {
        mov ecx, dword ptr [ecx + 0x18]   // control pointer
        mov edx, dword ptr [ecx]          // control vtable
        jmp dword ptr [edx + 0xA8]        // forward to the control's virtual method
    }
}

// ---------------------------------------------------------------------------
// Target 0x00539930 (ret 0)
// ---------------------------------------------------------------------------
static int __fastcall CMenuLoadGame_ActivateSelectedEntry(CMenuLoadGame* self, void* /*edx*/)
{
    // ------------------------------------------------------------- 0x00539930
    int notifierOwner = GetGlobalValue_8B056C();
    int notifier = *reinterpret_cast<int*>(notifierOwner + 4);
    if (notifier != 0)
    {
        void** notifierVtbl = *reinterpret_cast<void***>(notifier);
        reinterpret_cast<void(__fastcall*)(int, void*, void*)>(notifierVtbl[0x2C / 4])(
            notifier, nullptr, reinterpret_cast<uint8_t*>(self) + 0x1C);
    }

    void* control = reinterpret_cast<void*>(self->field_0018);
    void** controlVtbl = *reinterpret_cast<void***>(control);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(controlVtbl[0x60 / 4])(
        control, nullptr, static_cast<void*>(self), 1);

    // ------------------------------------------------------------- 0x00539720
    void** selfVtbl = *reinterpret_cast<void***>(self);
    int gate = reinterpret_cast<int(__fastcall*)(CMenuLoadGame*, void*)>(selfVtbl[0x30 / 4])(self, nullptr);

    if (static_cast<uint8_t>(gate) != 0)
    {
        if (*reinterpret_cast<int*>(GetGlobalValue_8B0568() + 0x5C) == 1)
        {
            // 0x0053973C: we are in a game -> start the selected tactic game
            int gameState = GetGlobalState_50D190();
            MenuSystem_ToggleChildFlag(gameState, nullptr);

            gameState = GetGlobalState_50D190();
            int child = *reinterpret_cast<int*>(gameState + 0x10);
            if (child != 0)
            {
                void** childVtbl = *reinterpret_cast<void***>(child);
                reinterpret_cast<void(__fastcall*)(int, void*)>(childVtbl[0x24 / 4])(child, nullptr);
            }

            gameState = GetGlobalState_50D190();
            void* listBox = *reinterpret_cast<void**>(gameState + 0x14);
            void** listVtbl = *reinterpret_cast<void***>(listBox);
            int entry = reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(listVtbl[0x5C / 4])(
                listBox, nullptr, g_pszMenu_P_Tactic);
            listVtbl = *reinterpret_cast<void***>(listBox);
            int activated = reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(listVtbl[0x74 / 4])(
                listBox, nullptr, entry, 0);
            if (static_cast<uint8_t>(activated) != 0)
            {
                void* itemOwner = reinterpret_cast<void*>(self->field_0018);
                void** itemOwnerVtbl = *reinterpret_cast<void***>(itemOwner);
                int item = reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(itemOwnerVtbl[0x5C / 4])(
                    itemOwner, nullptr, g_pszMenu_P_Tactic);
                return CMenuMissionInfo_EnterPanelState(item, nullptr, 0);
            }
            return activated;
        }

        // 0x00539798: not in game -> only refresh the selection
        int gameState = GetGlobalState_50D190();
        void* listBox = *reinterpret_cast<void**>(gameState + 0x14);
        void** listVtbl = *reinterpret_cast<void***>(listBox);
        int entry = reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(listVtbl[0x5C / 4])(
            listBox, nullptr, g_pszMenu_P_Tactic);
        listVtbl = *reinterpret_cast<void***>(listBox);
        int activated = reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(listVtbl[0x74 / 4])(
            listBox, nullptr, entry, 0);
        if (static_cast<uint8_t>(activated) == 0)
        {
            void* ctrl = reinterpret_cast<void*>(self->field_0018);
            void** ctrlVtbl = *reinterpret_cast<void***>(ctrl);
            return reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(ctrlVtbl[0x58 / 4])(
                ctrl, nullptr, g_pszMenu_P_MeJueg);
        }
        return activated;
    }

    // ------------------------------------------------------------- 0x005397CE
    // menu is not active -> go back to the previous screen
    int mode = *reinterpret_cast<int*>(GetGlobalValue_8B0568() + 0x5C);
    control = reinterpret_cast<void*>(self->field_0018);
    controlVtbl = *reinterpret_cast<void***>(control);
    if (mode == 1)
    {
        reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(controlVtbl[0x58 / 4])(
            control, nullptr, g_pszMenu_P_Juego);

        void* control2 = reinterpret_cast<void*>(self->field_0018);
        void** control2Vtbl = *reinterpret_cast<void***>(control2);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(control2Vtbl[0x9C / 4])(control2, nullptr, 0);

        return CMenuLoadGame_TailDispatchToControl(self, nullptr);
    }

    return reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(controlVtbl[0x58 / 4])(
        control, nullptr, g_pszMenu_P_Campan);
}

HOOK(0x539930, CMenuLoadGame_ActivateSelectedEntry, "auto");
