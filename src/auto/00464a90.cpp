#include <cstdint>

#include "globals.h"
#include "hook.h"

// Pointer to the current scene object (game global).
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// When set in the index, the entry is taken from the current scene's table instead of this->+0x20.
static constexpr uint32_t kSharedTableFlag = 0x00800000u;

// Applies a float operation (0 add, 1 subtract, 2 multiply, 3 divide) between an entry's float
// operand and a float in a second record. Returns the entry's record pointer. Unknown opcodes
// return the record without modifying it.
static int __fastcall ApplyFloatOpToIndexedEntry(void* self, void* /*edx*/,
                                                 int /*a2*/, int /*a3*/,
                                                 uint32_t* operands)
{
    // operands[0] is a record whose +8 dword is the entry index (plus optional scene flag).
    uint32_t entryIndex = CSF_AT(uint32_t, reinterpret_cast<void*>(operands[0]), 8);
    uint32_t tableOwnerValue = CSF_AT(uint32_t, self, 0x20);

    if ((entryIndex & kSharedTableFlag) != 0) {
        uint32_t sceneChildValue = CSF_AT(uint32_t, g_pCurrentScene, 0xBC);
        tableOwnerValue = CSF_AT(uint32_t, reinterpret_cast<void*>(sceneChildValue), 0x38);
        entryIndex &= ~kSharedTableFlag;
    }

    // Table pointer at owner+8; entries are 12 bytes.
    uint32_t entryTable = CSF_AT(uint32_t, reinterpret_cast<void*>(tableOwnerValue), 8);
    uint8_t* entrySlotBase = reinterpret_cast<uint8_t*>(entryTable) + 12u * entryIndex;

    // operands[1] -> record whose +4 pointer leads to a byte opcode at +1.
    // This is read before the entry's record pointer (entry+4).
    uint32_t opRecordValue = CSF_AT(uint32_t, reinterpret_cast<void*>(operands[1]), 4);
    int8_t opcode = CSF_AT(int8_t, reinterpret_cast<void*>(opRecordValue), 1);

    // Record pointer stored at entry+4.
    uint32_t resultValue = CSF_AT(uint32_t, entrySlotBase, 4);
    void* resultObj = reinterpret_cast<void*>(resultValue);

    // operands[2] -> record whose +4 pointer leads to the float operand at +4.
    switch (opcode) {
    case 0: {
        // add: RHS record and RHS float are read before the result float.
        void* rhsOwner = reinterpret_cast<void*>(operands[2]);
        void* rhsRecord = reinterpret_cast<void*>(CSF_AT(uint32_t, rhsOwner, 4));
        const float rhs = CSF_AT(float, rhsRecord, 4);
        const float current = CSF_AT(float, resultObj, 4);
        CSF_AT(float, resultObj, 4) = rhs + current;
        break;
    }
    case 1: {
        // subtract: result float is read before the RHS record pointer is dereferenced.
        void* rhsOwner = reinterpret_cast<void*>(operands[2]);
        const float current = CSF_AT(float, resultObj, 4);
        void* rhsRecord = reinterpret_cast<void*>(CSF_AT(uint32_t, rhsOwner, 4));
        const float rhs = CSF_AT(float, rhsRecord, 4);
        CSF_AT(float, resultObj, 4) = current - rhs;
        break;
    }
    case 2: {
        // multiply: RHS record and RHS float are read before the result float.
        void* rhsOwner = reinterpret_cast<void*>(operands[2]);
        void* rhsRecord = reinterpret_cast<void*>(CSF_AT(uint32_t, rhsOwner, 4));
        const float rhs = CSF_AT(float, rhsRecord, 4);
        const float current = CSF_AT(float, resultObj, 4);
        CSF_AT(float, resultObj, 4) = rhs * current;
        break;
    }
    case 3: {
        // divide: result float is read before the RHS record pointer is dereferenced.
        void* rhsOwner = reinterpret_cast<void*>(operands[2]);
        const float current = CSF_AT(float, resultObj, 4);
        void* rhsRecord = reinterpret_cast<void*>(CSF_AT(uint32_t, rhsOwner, 4));
        const float rhs = CSF_AT(float, rhsRecord, 4);
        CSF_AT(float, resultObj, 4) = current / rhs;
        break;
    }
    default:
        break;
    }

    return static_cast<int>(resultValue);
}

HOOK(0x00464A90, ApplyFloatOpToIndexedEntry, "auto");
