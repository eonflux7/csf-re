#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callees, bound by address

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// The original passes two stack arguments (ret 8) but also reads ECX (the caller's this, saved
// from the entry ECX) and leaves EDX holding the third argument. Binding it as __fastcall with
// (self, edx, a1, a2) reproduces that register state exactly. The stack layout and callee pop
// are the same as the stdcall form.
CSF_FUNC(bool, __fastcall, IsEntityAllowedForSide, 0x0045BB30, (void* self, void* edx, int a1, int a2));

CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resolves a pair of handle records through the scene handle table, checks that the
// entity's permission for the given side passes, then sends a 24-byte event (type 32,
// vtable 0x82D448, two ints from the second record, a scaled int, and self) to the
// entity's controller at +0x34 via virtual slot 42. Returns a byte result.
static char __fastcall DispatchPairedHandleEvent(void* self, void* /*edx*/, int /*a2*/, int ownerId, uint32_t* handlePairRefs)
{
    // First handle record: *a4 -> link; link+4 -> record; key is record+4
    uint8_t* firstLink = reinterpret_cast<uint8_t*>(handlePairRefs[0]);
    uint8_t* firstRecord = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, firstLink, 4));
    uintptr_t firstKey = reinterpret_cast<uintptr_t>(firstRecord) + 4;

    // Second handle record: a4[1] -> link; link+4 -> record; key is record+4
    uint8_t* secondLink = reinterpret_cast<uint8_t*>(handlePairRefs[1]);
    uint8_t* secondRecord = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, secondLink, 4));
    uintptr_t secondKey = reinterpret_cast<uintptr_t>(secondRecord) + 4;

    // Both records must have a non-negative level at +12
    int32_t firstLevel = CSF_AT(int32_t, firstRecord, 12);
    if (firstLevel < 0 || CSF_AT(int32_t, secondRecord, 12) < 0)
        return static_cast<char>(firstKey);

    int firstEntityHandle = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(firstKey));
    int resolvedSecond = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(secondKey));
    if (firstEntityHandle == 0)
        return static_cast<char>(resolvedSecond);

    uint8_t* entity = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(firstEntityHandle));
    if (CSF_AT(void*, entity, 0x34) == nullptr)
        return static_cast<char>(resolvedSecond);

    // ECX = this (self), EDX = ownerId, stack = (ownerId, resolvedSecond)
    bool permitted = IsEntityAllowedForSide(self,
                                            reinterpret_cast<void*>(static_cast<uintptr_t>(ownerId)),
                                            ownerId,
                                            resolvedSecond);
    if (!permitted)
        return 0;

    // Event payload: 32, vtable, two ints from the second record, scaled int, self
    uint32_t eventArgs[6];
    eventArgs[0] = 32;
    eventArgs[1] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    eventArgs[2] = CSF_AT(uint32_t, secondRecord, 8);
    eventArgs[3] = CSF_AT(uint32_t, secondRecord, 12);

    // handlePairRefs[2] -> link; link+4 -> record; float at record+4
    uint8_t* scaleLink = reinterpret_cast<uint8_t*>(handlePairRefs[2]);
    uint8_t* scaleRecord = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, scaleLink, 4));
    float scaleValue = CSF_AT(float, scaleRecord, 4);
    eventArgs[4] = static_cast<uint32_t>(static_cast<int64_t>(scaleValue));
    eventArgs[5] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));

    // Controller is re-read after the calls above, as the original does
    void* controller = CSF_AT(void*, entity, 0x34);
    void** controllerVtable = CSF_AT(void**, controller, 0);
    ((void(__fastcall*)(void*, void*, uint32_t*))controllerVtable[42])(controller, nullptr, eventArgs);

    // Low byte of the entity flags is the result unless bit 0x2000 is set
    uint32_t entityFlags = CSF_AT(uint32_t, entity, 0x170);
    char result = static_cast<char>(entityFlags);
    if ((entityFlags & 0x2000) != 0)
        result = static_cast<char>(IsField1BCNonNegative(entity, nullptr));
    return result;
}

HOOK(0x00463A20, DispatchPairedHandleEvent, "auto");
