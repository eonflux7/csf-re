#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee bindings (addresses and signatures from the packet).
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(void*, __fastcall, GetGlobalSubObject_4FC0A0, 0x004FC0A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ProcessGlobalSubObjectWithBlock, 0x00443000, (void* self, void* edx, int a2));

// Local argument block passed by address to ProcessGlobalSubObjectWithBlock.
// Layout matches the original stack locals: 4 dwords at +0,+4,+8,+12 and one byte at +16.
struct LinkedValueBlock {
    int32_t flag;        // v8  (initialized to 1)
    int32_t valueA;      // v9  (from first linked object chain)
    int32_t reserved;    // v10 (initialized to 0)
    int32_t valueB;      // v11 (from second linked object chain)
    uint8_t byteValue;   // v12 (from third linked object chain)
};

static int __stdcall DispatchLinkedValuesToGlobalSubObject(int /*a1*/, int /*a2*/, int* a3) {
    // a3 is an array of three object pointers.
    auto* argPtrs = reinterpret_cast<uint32_t*>(a3);

    LinkedValueBlock block;
    block.valueA = 0;
    block.flag = 1;
    block.reserved = 0;

    // valueA = argPtrs[0]->[4]->[4]
    uint32_t firstObj = argPtrs[0];
    uint32_t firstInner = CSF_AT(uint32_t, reinterpret_cast<void*>(firstObj), 4);
    block.valueA = static_cast<int32_t>(CSF_AT(uint32_t, reinterpret_cast<void*>(firstInner), 4));

    // valueB = argPtrs[1]->[4]->[4]
    uint32_t secondObj = argPtrs[1];
    uint32_t secondInner = CSF_AT(uint32_t, reinterpret_cast<void*>(secondObj), 4);
    block.valueB = static_cast<int32_t>(CSF_AT(uint32_t, reinterpret_cast<void*>(secondInner), 4));

    // byteValue = argPtrs[2]->[4] byte at +1
    uint32_t thirdObj = argPtrs[2];
    uint32_t thirdInner = CSF_AT(uint32_t, reinterpret_cast<void*>(thirdObj), 4);
    block.byteValue = CSF_AT(uint8_t, reinterpret_cast<void*>(thirdInner), 1);

    // The original pushes &block before the first call and leaves it on the stack
    // for ProcessGlobalSubObjectWithBlock, which consumes it (ret 4).
    int blockArg = static_cast<int>(reinterpret_cast<uintptr_t>(&block));

    int globalValue = GetGlobalValue_4FBDE0();
    void* globalSubObject = GetGlobalSubObject_4FC0A0(reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr);
    return ProcessGlobalSubObjectWithBlock(globalSubObject, nullptr, blockArg);
}

HOOK(0x0045B810, DispatchLinkedValuesToGlobalSubObject, "auto");
