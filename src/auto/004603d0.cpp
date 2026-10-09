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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(unsigned int, __fastcall, ApplyHandleValueChange, 0x0051F900, (void* self, void* edx, int a2, int a3));

static void* ObjectFromWord(uint32_t word)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(word));
}

static void** VtableOf(void* obj)
{
    return CSF_AT(void**, obj, 0);
}

// Reads the argument value reached through a3[1] -> ->+4 -> +4.
// The original reloads this chain at each use, so it is a helper called each time.
static int32_t ReadNestedArgValue(uint32_t* a3)
{
    uint8_t* firstLink = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3[1]));
    uint8_t* secondLink = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, firstLink, 4)));
    return CSF_AT(int32_t, secondLink, 4);
}

static int __stdcall DispatchHandleEventToEntity(int a1, int a2, uint32_t* a3)
{
    uint8_t* eventRecord = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3[0]));
    uint8_t* handleBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, eventRecord, 4)));
    int32_t handleArg = static_cast<int32_t>(reinterpret_cast<uintptr_t>(handleBase + 4));

    int32_t result = CSF_AT(int32_t, handleBase, 12);
    if (result >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleArg);
        void* entity = ObjectFromWord(static_cast<uint32_t>(result));
        if (result != 0)
        {
            // Slot 10: entity->GetOwnedObject()
            void* ownedObj = reinterpret_cast<void*>(
                reinterpret_cast<int (__fastcall*)(void*, void*)>(VtableOf(entity)[10])(entity, nullptr));

            // Slot 38: ownedObj->QueryValue(argValue)
            int32_t queried = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(VtableOf(ownedObj)[38])(
                ownedObj, nullptr, ReadNestedArgValue(a3));
            result = queried;

            if (result != -1)
            {
                // Slot 10 again on the entity, then slot 35 with the queried value.
                void* notifyObj = reinterpret_cast<void*>(
                    reinterpret_cast<int (__fastcall*)(void*, void*)>(VtableOf(entity)[10])(entity, nullptr));
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(VtableOf(notifyObj)[35])(
                    notifyObj, nullptr, result);

                int32_t finalValue = ReadNestedArgValue(a3);
                int32_t globalValue = GetGlobalValue_51F740();
                return static_cast<int>(ApplyHandleValueChange(
                    ObjectFromWord(static_cast<uint32_t>(globalValue)), nullptr, handleArg, finalValue));
            }
        }
    }
    return result;
}

HOOK(0x004603D0, DispatchHandleEventToEntity, "auto");
