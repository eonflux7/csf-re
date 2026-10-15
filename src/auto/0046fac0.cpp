#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_string.h"
using String = legacy::vc71::string<char>;
CSF_FUNC(void*, __fastcall, Assign, 0x00403C60, (String* self, void* edx, const char* source, uint32_t count));
CSF_FUNC(void*, __fastcall, AssignRange, 0x00403B60, (String* self, void* edx, String* source, uint32_t pos, uint32_t count));
CSF_FUNC(void*, __fastcall, ExceptionCtor, 0x007AF6A0, (void* self, void* edx));
CSF_FUNC(void, __stdcall, Throw, 0x007AF666, (void* object, void* info));
static void __fastcall LengthError_0046fac0(void*, void* edx) {
    String message;
    message.capacity = 15;
    message.length = 0;
    message.storage.buffer[0] = 0;
    Assign(&message, edx, CSF_STR(0x008315E0, "vector<bool> too long"), 21);
    alignas(4) uint8_t exception[40];
    ExceptionCtor(exception, nullptr);
    *reinterpret_cast<void**>(exception) = CSF_VTBL(0x008769D4);
    *reinterpret_cast<uint32_t*>(exception + 36) = 15;
    *reinterpret_cast<uint32_t*>(exception + 32) = 0;
    exception[16] = 0;
    AssignRange(reinterpret_cast<String*>(exception + 12), nullptr, &message, 0, 0xFFFFFFFFu);
    *reinterpret_cast<void**>(exception) = CSF_VTBL(0x008769E0);
    Throw(exception, reinterpret_cast<void*>(CSF_ADDR_DATA(0x008867F0)));
}
HOOK(0x0046FAC0, LengthError_0046fac0, "stl");
