#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Binds as given in the packet.
CSF_FUNC(int, __fastcall, sub_655E40, 0x00655E40, (void* self, void* edx, char* a2));
CSF_FUNC(uint32_t*, __fastcall, sub_655F00, 0x00655F00, (void* self, void* edx, int a2));

// Converts a pointer to the 32-bit integer the callee takes by value.
static int AddressAsInt(const void* p) {
    return static_cast<int>(reinterpret_cast<uintptr_t>(p));
}

// Writes this object's header bytes and its eight fields into the sink object (a2).
// The receiver of every callee is the sink, as in the disassembly (ecx = a2).
static uint32_t* __fastcall C_008622e8_SerializeFields(C_008622e8* self, void* /*edx*/, C_008622e8* sink) {
    sub_655E40(sink, nullptr, reinterpret_cast<char*>(self));
    sub_655E40(sink, nullptr, reinterpret_cast<char*>(self) + 2);

    sub_655F00(sink, nullptr, AddressAsInt(&self->field_0004));
    sub_655F00(sink, nullptr, AddressAsInt(&self->field_0008));
    sub_655F00(sink, nullptr, AddressAsInt(&self->field_000c));
    sub_655F00(sink, nullptr, AddressAsInt(&self->field_0010));
    sub_655F00(sink, nullptr, AddressAsInt(&self->field_0014));
    sub_655F00(sink, nullptr, AddressAsInt(&self->field_0018));
    sub_655F00(sink, nullptr, AddressAsInt(&self->field_001c));
    return sub_655F00(sink, nullptr, AddressAsInt(&CSF_AT(uint32_t, self, 0x20)));
}

HOOK(0x00742990, C_008622e8_SerializeFields, "auto");
