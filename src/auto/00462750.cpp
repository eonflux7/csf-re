#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GuiControl_SetInteractionState, 0x0048EEC0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, ColorAnimBuffer_Init, 0x00412AB0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ColorAnim_StartForControl, 0x00445050, (void* self, void* edx, void* arg0, uint32_t* a2));
CSF_FUNC(int, __fastcall, ColorAnim_ClearForControl, 0x00445580, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, CGuiControl_GetColorAnim, 0x004FE4B0, (CGuiControl* self, void*));

// Replaces only the low byte of a word (the original does this via LOBYTE on a register value,
// so the upper 24 bits pass through unchanged).
static uint32_t ReplaceLowByte(uint32_t word, uint8_t low) {
    return (word & 0xFFFFFF00u) | low;
}

// Reads the byte at +1 of the object that the word at +4 of recordWord points to.
static uint8_t ReadRecordFlagByte(uint32_t recordWord) {
    const uint32_t innerObject = *reinterpret_cast<uint32_t*>(recordWord + 4u);
    return *(reinterpret_cast<uint8_t*>(innerObject) + 1);
}

static int __stdcall GuiControl_ApplyToggleFromHandle(int /*a1*/, int /*a2*/, uint32_t* argPack) {
    const uint32_t entryWord = *reinterpret_cast<uint32_t*>(argPack[0] + 4u);
    const int32_t entryFlags = *reinterpret_cast<int32_t*>(entryWord + 12u);
    int result = static_cast<int>(entryWord + 4u);

    if (entryFlags >= 0) {
        const int controlHandle = static_cast<int>(entryWord + 4u);
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, controlHandle);
        CPlayerCharacter* control = reinterpret_cast<CPlayerCharacter*>(result);

        if (result != 0) {
            const uint32_t firstRecord = argPack[1];
            const uint8_t firstFlag = static_cast<uint8_t>(ReadRecordFlagByte(firstRecord) == 0);
            // Slot 0x6C / 4 = 27; the typed wrapper is declared void, but the original keeps eax.
            const int stateReturn = ((int(__fastcall*)(void*, void*, int))control->vtbl[27])(
                control, nullptr, static_cast<int>(ReplaceLowByte(firstRecord, firstFlag)));

            const uint8_t secondFlag = static_cast<uint8_t>(ReadRecordFlagByte(argPack[1]) == 0);
            const uint32_t secondArg = ReplaceLowByte(static_cast<uint32_t>(stateReturn), secondFlag);
            result = GuiControl_SetInteractionState(control, nullptr, static_cast<int>(secondArg));

            if (control->field_0050 == 2) {
                if (ReadRecordFlagByte(argPack[1]) != 0) {
                    uint32_t colorAnimBuffer[11];
                    ColorAnimBuffer_Init(colorAnimBuffer, nullptr);
                    const int globalAnimHandle = GetGlobalValue_8B0568();
                    const int colorAnim = CGuiControl_GetColorAnim(reinterpret_cast<CGuiControl*>(globalAnimHandle), nullptr);
                    return ColorAnim_StartForControl(reinterpret_cast<void*>(colorAnim), nullptr, control, colorAnimBuffer);
                } else {
                    const int globalAnimHandle = GetGlobalValue_8B0568();
                    const int colorAnim = CGuiControl_GetColorAnim(reinterpret_cast<CGuiControl*>(globalAnimHandle), nullptr);
                    return ColorAnim_ClearForControl(reinterpret_cast<void*>(colorAnim), nullptr, reinterpret_cast<uint32_t*>(control));
                }
            }
        }
    }
    return result;
}

HOOK(0x00462750, GuiControl_ApplyToggleFromHandle, "auto");
