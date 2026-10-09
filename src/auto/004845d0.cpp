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

// Reads the object pointer stored at +0x9C of the weapon FSM. It is an embedded
// pointer-sized value, so it is read as a raw dword.
static uint32_t CCharWeaponFsm_GetLinkedStateObjectRaw(CCharWeaponFsm* self) {
    return CSF_AT(uint32_t, self, 0x9C);
}

// Returns true when the child object's state query (vtable slot 0x4C) reports state 10.
static int CCharWeaponFsm_LinkedObjectStateIs10(void* linkedObject) {
    void** vtbl = CSF_AT(void**, linkedObject, 0);
    int state = ((int(__fastcall*)(void*, void*))vtbl[0x4C / 4])(linkedObject, nullptr);
    return state == 10;
}

static char __fastcall CCharWeaponFsm_TryFinishReloadTransition(CCharWeaponFsm* self, void* /*edx*/) {
    uint32_t linkedRaw = CCharWeaponFsm_GetLinkedStateObjectRaw(self);
    if (linkedRaw == 0) {
        return 0;
    }

    void* linkedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(linkedRaw));
    if (!CCharWeaponFsm_LinkedObjectStateIs10(linkedObject)) {
        return 0;
    }

    // Virtual slot 0xD8: obtain the weapon-side object used for the reload finish.
    int weaponObjectValue = self->vGetIndexedEntryOrZero();
    void* weaponObject = reinterpret_cast<void*>(static_cast<uintptr_t>(weaponObjectValue));

    // Virtual slot 0x8C on that object, passing the stored 8-bit mode value.
    int storedMode = static_cast<int>(self->field_0070);
    void** weaponVtbl = CSF_AT(void**, weaponObject, 0);
    ((void(__fastcall*)(void*, void*, int))weaponVtbl[0x8C / 4])(weaponObject, nullptr, storedMode);

    // Virtual slot 0x11C on self: transition to state 13.
    self->vUpdateWeaponSelection(13);
    return 1;
}

HOOK(0x004845D0, CCharWeaponFsm_TryFinishReloadTransition, "auto");
