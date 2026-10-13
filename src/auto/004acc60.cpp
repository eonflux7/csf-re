#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(char, __fastcall, AiAttackTasks_AddToActionFlag, 0x0048C9D0, (uint8_t* self, void* , char delta));
CSF_FUNC(int, __fastcall, C_00834050_SetActionMode, 0x00492760, (void* self, void* , int newActionMode));

// GetLinkedObjectValue returns a raw address as int; convert it to a byte pointer.
static uint8_t* AiOrder_IntToBytePtr(int value)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

// Returns the object at [linked - 4 + 0x2C], or reads address 0x2C when nothing is linked
// (the original does the same: xor eax, eax; mov eax, [eax + 0x2C]).
static void* AiOrder_GetLinkedOwner(void* a1)
{
    int linked = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    uint8_t* base = linked ? AiOrder_IntToBytePtr(linked - 4) : nullptr;
    return CSF_AT(void*, base, 0x2C);
}

static int __stdcall AiOrder_ResetAttackStateAndActivate(void* a1)
{
    PrepareStaticAiState(a1);

    // Clear the action flag on the owner's +0x204 sub-object.
    void* owner = AiOrder_GetLinkedOwner(a1);
    uint8_t* flagTarget = static_cast<uint8_t*>(CSF_AT(void*, owner, 0x204));
    AiAttackTasks_AddToActionFlag(flagTarget, nullptr, static_cast<char>(-1));

    // Set the owner's action mode to 0.
    void* ownerAgain = AiOrder_GetLinkedOwner(a1);
    C_00834050_SetActionMode(ownerAgain, nullptr, 0);

    // Activate the linked object's +0x28 sub-object (or the fallback read at address 0x2C).
    int linkedObject = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    if (linkedObject)
    {
        void* target = CSF_AT(void*, AiOrder_IntToBytePtr(linkedObject), 0x28);
        return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, target, 0)[180 / 4])(target, nullptr, 1);
    }
    uint8_t* nullBase = nullptr;
    void* fallback = CSF_AT(void*, nullBase, 0x2C);
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, fallback, 0)[180 / 4])(fallback, nullptr, 1);
}

HOOK(0x004ACC60, AiOrder_ResetAttackStateAndActivate, "auto");
