#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_list.h"
#include "types/vc71_string.h"
using List = legacy::vc71::list<uint32_t>;
using String = legacy::vc71::string<char>;
CSF_FUNC(void*, __fastcall, Assign, 0x00403C60, (String* self, uint32_t edx, const char* source, uint32_t count));
CSF_FUNC(void*, __fastcall, AssignRange, 0x00403B60, (String* self, void* edx, String* source, uint32_t pos, uint32_t count));
CSF_FUNC(void*, __fastcall, ExceptionCtor, 0x007AF6A0, (void* self, void* edx));
CSF_FUNC(void, __stdcall, Throw, 0x007AF666, (void* object, void* info));
struct Operations {
    using value_type = uint32_t;
    static constexpr uint32_t max_size = 0xFFFFFFFFu;
    static __forceinline void length_error(uint32_t count) {
        String message;
        message.capacity = 15;
        message.length = 0;
        message.storage.buffer[0] = 0;
        Assign(&message, count, CSF_STR(0x0084469C, "list<T> too long"), 16);
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
};
static uint32_t __fastcall StlListInc_0073fe80(List* self, void*, uint32_t count) {
    return legacy::vc71::list_incsize<Operations>(self, count);
}
HOOK(0x0073FE80, StlListInc_0073fe80, "stl");
