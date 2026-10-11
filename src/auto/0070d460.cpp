#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(void*, __fastcall, WaterEntry_PrepareFromDescriptor, 0x006FE280, (void* self, void* edx, void* a2));
CSF_FUNC(bool, __fastcall, WaterShader_ApplyNamedSource, 0x0070CD40, (void* self, void* edx, char* Source));

// Referenced by the original at the 0x70cd40 call sites (packet-provided vtable binding).
[[maybe_unused]] static const auto kS0070cdVtblRef = CSF_VTBL(0x0070CD40);

static bool __fastcall S_0070cd40_MatchNameThenInvoke(S_0070cd40* self, void* /*edx*/, int a2)
{
    uint8_t* src = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));
    uint8_t* selfBytes = reinterpret_cast<uint8_t*>(self);
    uint8_t* nameField = selfBytes + 0x3c;

    // Source text: inline buffer when capacity < 16, otherwise heap pointer.
    const char* sourceText;
    if (CSF_AT(uint32_t, src, 0x48) < 0x10u)
        sourceText = reinterpret_cast<const char*>(src + 0x34);
    else
        sourceText = CSF_AT(const char*, src, 0x34);

    bool matched = CompareStringSubstring(nameField, nullptr, 0u,
                                          CSF_AT(unsigned int, nameField, 0x14),
                                          sourceText,
                                          CSF_AT(unsigned int, src, 0x44)) != 0;

    WaterEntry_PrepareFromDescriptor(selfBytes + 0x0c, nullptr, src);

    using VirtualSlot3 = void(__fastcall*)(void*, void*, void*);
    ((VirtualSlot3)CSF_AT(void**, self, 0)[3])(self, nullptr, selfBytes + 0x0c);

    if (matched) {
        if (CSF_AT(uint32_t, nameField, 0x18) < 0x10u)
            return WaterShader_ApplyNamedSource(self, nullptr, reinterpret_cast<char*>(selfBytes + 0x40));
        else
            return WaterShader_ApplyNamedSource(self, nullptr, CSF_AT(char*, selfBytes, 0x40));
    }
    return matched;
}

HOOK(0x0070D460, S_0070cd40_MatchNameThenInvoke, "auto");
