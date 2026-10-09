#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, sub_519B50, 0x00519B50, (void* self, void* edx, int a2, unsigned int* a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, sub_6B13A0, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_Unknown_GetValue, 0x006C2050, (void* self, void*));
CSF_FUNC(int, __fastcall, sub_6C8EE0, 0x006C8EE0, (void* self, void* edx, int a2));

// Resolves an entry index from a nested record through the global pool and, if
// the chain resolves to a valid object, stores a float value into the owner record.
static void __stdcall UpdateTaskValueFromObjectLookup(int a1, int /*unusedArg2*/, int a3)
{
    // Clear the float slot of the owner record at a1->+4 before resolving.
    uint8_t* ownerRecord = CSF_AT(uint8_t*, a1, 4);
    CSF_AT(uint32_t, ownerRecord, 4) = 0;

    // entryIndex = *(*(*(a3)) + 4) + 4 chain.
    uint8_t* firstLevel = CSF_AT(uint8_t*, a3, 0);
    uint8_t* secondLevel = CSF_AT(uint8_t*, firstLevel, 4);
    int32_t entryIndex = CSF_AT(int32_t, secondLevel, 4);

    if (entryIndex == -1) {
        return;
    }

    int valueBase = GetValue8B0554();
    void* pool = CSF_AT(void*, valueBase, 0x1FC4);
    int handleValue = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, pool, 0)[2])(pool, nullptr, entryIndex);

    if (handleValue == -1) {
        return;
    }

    int globalInt = GetGlobalInt();
    int taskBase = sub_6B13A0(reinterpret_cast<void*>(static_cast<uintptr_t>(globalInt)), nullptr);

    // Virtual slot 5 on taskBase; its result is the receiver for sub_6C8EE0.
    void* taskObj = reinterpret_cast<void*>(static_cast<uintptr_t>(taskBase));
    void* taskSelf = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, taskObj, 0)[5])(taskObj, nullptr);

    a3 = 0;
    int resolvedId = sub_519B50(pool, nullptr, handleValue, reinterpret_cast<unsigned int*>(&a3));
    int resolvedObject = sub_6C8EE0(taskSelf, nullptr, resolvedId);

    if (resolvedObject != 0) {
        void* objectPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(resolvedObject));
        double value = C_Unknown_GetValue(objectPtr, nullptr);

        // a1 is reloaded here, after the calls above.
        uint8_t* ownerAfter = CSF_AT(uint8_t*, a1, 4);
        CSF_AT(float, ownerAfter, 4) = static_cast<float>(value);
    }
}

HOOK(0x0045AC90, UpdateTaskValueFromObjectLookup, "auto");
