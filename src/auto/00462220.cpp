#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, sub_4582E0, 0x004582E0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_45A5E0, 0x0045A5E0, (void* self, void* edx, char* a2));

static void* AsPointer(int32_t value) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

static int32_t ReadDwordAt(int32_t base, int32_t offset) {
    uintptr_t address = static_cast<uintptr_t>(static_cast<uint32_t>(base)) +
                        static_cast<uintptr_t>(static_cast<uint32_t>(offset));
    return *reinterpret_cast<int32_t*>(address);
}

static float ReadFloatAt(int32_t base, int32_t offset) {
    uintptr_t address = static_cast<uintptr_t>(static_cast<uint32_t>(base)) +
                        static_cast<uintptr_t>(static_cast<uint32_t>(offset));
    return *reinterpret_cast<float*>(address);
}

static void __stdcall SubmitEntityPlacementToSubsystem(int /*a1*/, int /*unused*/, int32_t* a3) {
    void* subsystem = AsPointer(ReadDwordAt(GetGlobal8afc78(), 0x248));
    if (!subsystem) {
        return;
    }

    alignas(4) uint8_t record[0x28];
    sub_4582E0(record, nullptr);

    CSF_AT(int32_t, record, 0x20) = -1;
    CSF_AT(int32_t, record, 0x24) = -1;

    int32_t firstNode = a3[0];
    int32_t firstNodeInner = ReadDwordAt(firstNode, 4);
    CSF_AT(void**, record, 0x1C) = CSF_VTBL(0x0082D448);
    CSF_AT(int32_t, record, 0x20) = ReadDwordAt(firstNodeInner, 8);
    CSF_AT(int32_t, record, 0x24) = ReadDwordAt(firstNodeInner + 4, 8);

    int32_t secondNode = a3[1];
    int32_t secondNodeInner = ReadDwordAt(secondNode, 4);
    CSF_AT(int32_t, record, 0) = static_cast<int32_t>(ReadFloatAt(secondNodeInner, 4));

    int32_t thirdNode = a3[2];
    CSF_AT(int32_t, record, 0x10) = ReadDwordAt(ReadDwordAt(thirdNode, 4), 4);

    int32_t fourthNode = a3[3];
    CSF_AT(int32_t, record, 0x8) = ReadDwordAt(ReadDwordAt(fourthNode, 4), 4);

    sub_45A5E0(subsystem, nullptr, reinterpret_cast<char*>(record));
}

HOOK(0x462220, SubmitEntityPlacementToSubsystem, "auto");
