#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, C_00860848_Construct, 0x006C4490, (C_00860848* self, void*));
CSF_FUNC(void, __fastcall, C_00860848_Destroy_6C37B0, 0x006C37B0, (C_00860848* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureGeometry, 0x006C35C0, (void* self, void*, int entryCount, int secondaryCount));
CSF_FUNC(int, __fastcall, Geometry_AppendColoredBox, 0x006C3A20, (void* self, void*, float* minimum, float* maximum, std::uint8_t* color));
CSF_FUNC(int, __fastcall, ParticleInner_Update, 0x006C4330, (void* self, void*));

// Builds a colored bounding-box geometry for a PTank object (fields at +0x48 / +0x54)
// and returns the same PTank pointer.
static int __cdecl PTank_InitColoredBoundsGeometry(int a1)
{
    // Scratch C_00860848 object on the stack (0x34 bytes of room in the original frame).
    alignas(8) std::uint8_t geometryBytes[0x34];
    C_00860848* geometry = reinterpret_cast<C_00860848*>(geometryBytes);

    C_00860848_Construct(geometry, nullptr);

    // White color, four bytes.
    alignas(4) std::uint8_t color[4];
    color[0] = 0xFF;
    color[1] = 0xFF;
    color[2] = 0xFF;
    color[3] = 0xFF;

    std::uint8_t* base = reinterpret_cast<std::uint8_t*>(a1);

    SceneLoader_ConfigureGeometry(geometry, nullptr, 8, 24);
    Geometry_AppendColoredBox(
        geometry,
        nullptr,
        reinterpret_cast<float*>(base + 0x54),
        reinterpret_cast<float*>(base + 0x48),
        color);
    ParticleInner_Update(geometry, nullptr);
    C_00860848_Destroy_6C37B0(geometry, nullptr);

    return a1;
}

HOOK(0x6EE540, PTank_InitColoredBoundsGeometry, "auto");
