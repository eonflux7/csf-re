// Reimplementation of 0x0050D4C0 (CMenuManager, menu_system, Menus/P_Common.fbs RwEngineTerm).
// Releases the owned screen objects in the original's exact order: for each non-null slot,
// calls its scalar deleting destructor (vtable slot 0, flag 1) and clears the slot to 0.
// The object at +0x7C is then queried through vtable slot 3 with no null check. Finally the
// object at +0x80 is released; the returned value is whatever eax holds at the end, which is
// the destructor's return value if +0x80 was non-null, otherwise the +0x7C query result.

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

// Calls slot 0 (scalar deleting destructor) with the delete flag 1 pushed on the stack.
// __fastcall: ecx = obj, edx unused, one stack argument popped by the callee (ret 4).
// The return value is kept because the original's eax survives this call.
static int CallScalarDeletingDestructor(void* obj)
{
    void** vtbl = *reinterpret_cast<void***>(obj);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(vtbl[0])(obj, nullptr, 1);
}

// Releases one owned screen slot: if non-null, destroy it and clear the slot to 0.
static void ReleaseScreenSlot(uint32_t& slot)
{
    if (slot != 0)
    {
        void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(slot));
        CallScalarDeletingDestructor(obj);
        slot = 0;
    }
}

// __thiscall method written as a free function (ecx = this, edx unused). No stack arguments, ret 0.
static int __fastcall CMenuManager_ReleaseOwnedScreens(CMenuManager* self, void* /*edx*/)
{
    ReleaseScreenSlot(self->m_pScreenGame);                 // +0x18
    ReleaseScreenSlot(self->m_pScreenGameMenu);             // +0x20
    ReleaseScreenSlot(self->m_pScreenProfiles);             // +0x24
    ReleaseScreenSlot(self->field_0028);                    // +0x28
    ReleaseScreenSlot(self->field_002c);                    // +0x2C
    ReleaseScreenSlot(self->m_pScreenIntro);                // +0x54
    ReleaseScreenSlot(self->field_0044);                    // +0x44
    ReleaseScreenSlot(self->m_pScreenGameOptionsInGame);    // +0x48
    ReleaseScreenSlot(self->m_pScreenGameOptions);          // +0x4C
    ReleaseScreenSlot(self->field_003c);                    // +0x3C
    ReleaseScreenSlot(self->field_0040);                    // +0x40
    ReleaseScreenSlot(self->field_0058);                    // +0x58
    ReleaseScreenSlot(self->m_pScreenTactic);               // +0x38
    ReleaseScreenSlot(self->field_0034);                    // +0x34
    ReleaseScreenSlot(self->m_pScreenLoading);              // +0x1C
    ReleaseScreenSlot(self->m_pScreenMainMenu);             // +0x30
    ReleaseScreenSlot(self->field_0050);                    // +0x50
    ReleaseScreenSlot(self->m_pScreenMsgBox);               // +0x5C
    ReleaseScreenSlot(self->field_00ec);                    // +0xEC
    ReleaseScreenSlot(self->field_0060);                    // +0x60

    // Unconditional virtual query on the object at +0x7C: vtable slot 3 (offset 0x0C), no stack args.
    // Its eax result survives unless the +0x80 release below runs.
    void* obj7C = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_007c));
    void** vtbl7C = *reinterpret_cast<void***>(obj7C);
    int eaxResult = reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl7C[3])(obj7C, nullptr);

    // Release +0x80. If non-null, its destructor's return value replaces eax; otherwise eax is unchanged.
    if (self->field_0080 != 0)
    {
        void* obj80 = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0080));
        eaxResult = CallScalarDeletingDestructor(obj80);
        self->field_0080 = 0;
    }

    return eaxResult;
}

HOOK(0x0050D4C0, CMenuManager_ReleaseOwnedScreens, "auto");
