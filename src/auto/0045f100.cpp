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

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x44FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x8AFC90);

// Turns a 32-bit address value read from the original game's memory into a byte pointer.
static uint8_t* AddressToBytePtr(uint32_t address)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(address));
}

// Resolves the handle stored in the command source object in the current scene and, if the
// resolved object has a child at +0x34, sends it a 6-dword move command through its vtable
// slot 42. On success, copies two source fields into self and clears the flag at self+0x82.
static int __fastcall SceneHandle_ResolveAndSendMoveCommand(void* self, void* /*edx*/,
                                                            int /*unusedA2*/, int /*unusedA3*/,
                                                            uint32_t* a4)
{
    // a4[0] -> command record; record+4 -> source object. The handle slot is source+4.
    uint8_t* commandRecord = AddressToBytePtr(a4[0]);
    uint8_t* sourceObj = AddressToBytePtr(CSF_AT(uint32_t, commandRecord, 4));
    uint8_t* handleSlot = sourceObj + 4;

    int handleIndex = CSF_AT(int, sourceObj, 12);
    if (handleIndex < 0)
        return handleIndex;

    int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                        static_cast<int>(reinterpret_cast<uintptr_t>(handleSlot)));
    if (resolved == 0)
        return resolved;

    uint8_t* resolvedObj = AddressToBytePtr(resolved);
    int targetObj = CSF_AT(int, resolvedObj, 0x34);
    if (targetObj == 0)
        return resolved;

    // Message payload, read in the original's order:
    // 1) vector: a4[1] -> holder; holder+4 -> three ints (x, y, z).
    uint8_t* vectorHolder = AddressToBytePtr(CSF_AT(uint32_t, AddressToBytePtr(a4[1]), 4));
    uint8_t* vectorValues = vectorHolder + 4;

    int message[6];
    message[0] = 13;
    message[1] = CSF_AT(int, vectorValues, 0);
    message[2] = CSF_AT(int, vectorValues, 4);
    message[3] = CSF_AT(int, vectorValues, 8);

    // 2) scalar B: a4[2] -> holder; holder+4 -> value at +4.
    uint8_t* scalarHolderB = AddressToBytePtr(CSF_AT(uint32_t, AddressToBytePtr(a4[2]), 4));
    message[4] = CSF_AT(int, scalarHolderB, 4);

    // 3) scalar C: a4[3] -> holder; holder+4 -> value at +4.
    uint8_t* scalarHolderC = AddressToBytePtr(CSF_AT(uint32_t, AddressToBytePtr(a4[3]), 4));
    message[5] = CSF_AT(int, scalarHolderC, 4);

    uint8_t* target = AddressToBytePtr(static_cast<uint32_t>(targetObj));
    int commandResult = ((int(__fastcall*)(void*, void*, int*))CSF_AT(void**, target, 0)[42])(
        target, nullptr, message);

    // Re-read the source fields after the call: the callee may have changed them.
    CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, sourceObj, 8);
    CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, sourceObj, 12);
    CSF_AT(uint8_t, self, 0x82) = 0;

    return commandResult;
}

HOOK(0x0045F100, SceneHandle_ResolveAndSendMoveCommand, "auto");
