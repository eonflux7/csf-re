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

// Vtable call shapes. Every original call is __thiscall; it is emitted as __fastcall with a
// dummy edx so the stack arguments and the callee's stack cleanup match the original.
using FindControlByNameFn = void*(__fastcall*)(void* self, void* edx, const char* name);
using SetWordFn           = void(__fastcall*)(void* self, void* edx, uint32_t value);
using ReadFlagFn          = uint8_t(__fastcall*)(void* self, void* edx);
using SetIntFlagFn        = int(__fastcall*)(void* self, void* edx, int value);
using SetModeFn           = int(__fastcall*)(void* self, void* edx, int a, int b, int c);

static void* WordAsPtr(uint32_t word)
{
    return reinterpret_cast<void*>(word);
}

// screen->vtbl[+0xFC]: __thiscall lookup of a named control; returns the control pointer.
static void* FindScreenControlByName(void* screen, const char* name)
{
    void* findFn = CSF_AT(void**, screen, 0)[63];
    return reinterpret_cast<FindControlByNameFn>(findFn)(screen, nullptr, name);
}

// CMenuSoundOptionsPC: binds the Sound Blaster X-Fi controls from the menu screen
// (Switch_SB_XFI, SB_XFI, SB_XFINo) and sets their state from the sound driver.
static int __fastcall CMenuSoundOptionsPC_InitSbXfiControls(CMenuSoundOptionsPC* self, void* /*edx*/, void* screen)
{
    if (self->field_002c == 0)
        return 0;

    // Switch_SB_XFI toggle control.
    void* switchCtrl = FindScreenControlByName(screen, CSF_STR(0x00843E68, "Switch_SB_XFI"));
    self->field_005c = reinterpret_cast<uint32_t>(switchCtrl);
    if (switchCtrl) {
        void** switchVtbl = CSF_AT(void**, switchCtrl, 0);
        reinterpret_cast<SetWordFn>(switchVtbl[167])(switchCtrl, nullptr, 0x534F3130u);

        void* switchNow = WordAsPtr(self->field_005c);
        reinterpret_cast<SetWordFn>(CSF_AT(void**, switchNow, 0)[167])(switchNow, nullptr, 0x534F3131u);

        // Driver state decides which value the switch gets.
        void* soundDriverObj = WordAsPtr(self->field_002c);
        void* switchForState = WordAsPtr(self->field_005c);
        void** driverVtbl = CSF_AT(void**, soundDriverObj, 0);
        void** switchStateVtbl = CSF_AT(void**, switchForState, 0);
        uint8_t driverFlag = reinterpret_cast<ReadFlagFn>(driverVtbl[14])(soundDriverObj, nullptr);
        uint32_t stateValue = 0x534F3131u - (driverFlag != 0 ? 1u : 0u);
        reinterpret_cast<SetWordFn>(switchStateVtbl[169])(WordAsPtr(self->field_005c), nullptr, stateValue);
    }

    // SB_XFI control: field_0060, then enabled with argument 1.
    void* sbXfiCtrl = FindScreenControlByName(screen, CSF_STR(0x00843E60, "SB_XFI"));
    self->field_0060 = reinterpret_cast<uint32_t>(sbXfiCtrl);
    if (sbXfiCtrl)
        reinterpret_cast<SetIntFlagFn>(CSF_AT(void**, sbXfiCtrl, 0)[42])(sbXfiCtrl, nullptr, 1);

    // SB_XFINo control: field_0064, then enabled with argument 1. Its return value
    // is kept because the original leaves it in eax for the early-exit return.
    void* sbXfiNoCtrl = FindScreenControlByName(screen, CSF_STR(0x00843E54, "SB_XFINo"));
    self->field_0064 = reinterpret_cast<uint32_t>(sbXfiNoCtrl);
    int lastEax = 0;
    if (sbXfiNoCtrl)
        lastEax = reinterpret_cast<SetIntFlagFn>(CSF_AT(void**, sbXfiNoCtrl, 0)[42])(sbXfiNoCtrl, nullptr, 1);

    if (self->field_005c == 0)
        return lastEax;
    if (self->field_0060 == 0)
        return 0;
    if (self->field_0064 == 0)
        return 0;

    // All three controls present: switch->vtbl[+0x68](0, 1, 1).
    void* switchFinal = WordAsPtr(self->field_005c);
    return reinterpret_cast<SetModeFn>(CSF_AT(void**, switchFinal, 0)[26])(switchFinal, nullptr, 0, 1, 1);
}

HOOK(0x548A30, CMenuSoundOptionsPC_InitSbXfiControls, "auto");
