#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x695590, __cdecl, 3 stack args. Copies 33 bytes of the edit-field sub-object into a text buffer.
CSF_FUNC(unsigned int, __cdecl, CopyTrucoStringToWide, 0x695590,
         (void* sourceField, void* destBuffer, int maxLength));

// Original: 0x692660, __cdecl, 4 stack args. Looks up localized text by key "ae03" and writes the result into a handle.
CSF_FUNC(int, __cdecl, FormatScenarioText, 0x692660,
         (void* outHandle, const char* key, void* argBuffer, int flag));

// Original: 0x50D190, __cdecl, no args. Returns the global game state object.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x50D190, ());

// Original: 0x50F860, __thiscall (ret 12). Shows a UI message from a text block with the given id and flags.
CSF_FUNC(unsigned int, __fastcall, SaveGame_ShowMessage, 0x50F860,
         (int self, void* edx, int messageBlock, int messageId, int flags));

// Original: 0x690E60, __cdecl, 1 arg. Frees a block allocated by the debug heap.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x690E60, (void* block));

// Account-edit screen: builds the "ae03" message from the edit text, shows it, and sets the edit state to 2.
// Returns the message-show result, or the freed block's result when a heap buffer was used.
static void* __fastcall CMenuAccountEdit_ShowAe03MessageAndSetState2(CMenuAccountEdit* self, void* /*edx*/)
{
    uint8_t textBuffer[68];
    uint8_t stringHandle[4];
    void* block[5];
    uint32_t stateFlag = 7;

    block[4] = nullptr;
    *reinterpret_cast<uint16_t*>(&block[0]) = 0;

    CopyTrucoStringToWide(reinterpret_cast<uint8_t*>(self) + 0x3c, textBuffer, 33);
    FormatScenarioText(stringHandle, CSF_STR(0x845500, "ae03"), textBuffer, 1);

    // Small-buffer optimisation: inline storage when stateFlag < 8, otherwise heap pointer in block[0].
    void* messageBlock = (stateFlag < 8) ? static_cast<void*>(block) : block[0];
    int messageBlockArg = static_cast<int>(reinterpret_cast<uintptr_t>(messageBlock));

    int globalState = GetGlobalState_50D190();
    unsigned int showResult = SaveGame_ShowMessage(globalState, nullptr, messageBlockArg, 12, 0);
    void* result = reinterpret_cast<void*>(static_cast<uintptr_t>(showResult));

    self->field_0280 = 2;

    if (stateFlag >= 8) {
        if (block[0] != nullptr) {
            return reinterpret_cast<void*>(static_cast<uintptr_t>(Mem_FreeDebug(block[0])));
        }
        return block[0];
    }
    return result;
}

HOOK(0x0055CC30, CMenuAccountEdit_ShowAe03MessageAndSetState2, "auto");
