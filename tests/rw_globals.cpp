// Link probes do not include generated standalone image data. Give this one
// global a test-owned cell; production uses the generated shared-image slot.
#include "globals.h"
#include "boundary/rw/fs.h"
#ifdef _WIN32
#include <windows.h>
namespace { CRITICAL_SECTION fsCriticalSection; }
template <> unsigned char* const csf::gv<0x008BEB40>::d = reinterpret_cast<unsigned char*>(&fsCriticalSection);
#endif
namespace {
csf_rw::FileSystem *fsDefault, *fsHead;
int fsMax, fsCount, fsInitialized, fsError;
void (__cdecl *fsErrorCB)(int);
}
template <> unsigned char* const csf::gv<0x008BC7C4>::d = reinterpret_cast<unsigned char*>(&fsDefault);
template <> unsigned char* const csf::gv<0x008BC7C8>::d = reinterpret_cast<unsigned char*>(&fsHead);
template <> unsigned char* const csf::gv<0x008BC7CC>::d = reinterpret_cast<unsigned char*>(&fsMax);
template <> unsigned char* const csf::gv<0x008BC7D0>::d = reinterpret_cast<unsigned char*>(&fsCount);
template <> unsigned char* const csf::gv<0x008BC7D4>::d = reinterpret_cast<unsigned char*>(&fsInitialized);
template <> unsigned char* const csf::gv<0x008BC7D8>::d = reinterpret_cast<unsigned char*>(&fsError);
template <> unsigned char* const csf::gv<0x008BC7DC>::d = reinterpret_cast<unsigned char*>(&fsErrorCB);
namespace { void* engineCell; }
template <> unsigned char* const csf::gv<0x008B68C0>::d = reinterpret_cast<unsigned char*>(&engineCell);
namespace { int pluginCell_008BE558; }
template <> unsigned char* const csf::gv<0x008BE558>::d = reinterpret_cast<unsigned char*>(&pluginCell_008BE558);
namespace { int pluginCell_008B1240; }
template <> unsigned char* const csf::gv<0x008B1240>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B1240);
namespace { int pluginCell_008B1244; }
template <> unsigned char* const csf::gv<0x008B1244>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B1244);
namespace { int pluginCell_008B1248; }
template <> unsigned char* const csf::gv<0x008B1248>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B1248);
namespace { int pluginCell_008B0984; }
template <> unsigned char* const csf::gv<0x008B0984>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B0984);
namespace { int pluginCell_008B0994; }
template <> unsigned char* const csf::gv<0x008B0994>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B0994);
namespace { int pluginCell_008B0998; }
template <> unsigned char* const csf::gv<0x008B0998>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B0998);
namespace { int pluginCell_008BE560; }
template <> unsigned char* const csf::gv<0x008BE560>::d = reinterpret_cast<unsigned char*>(&pluginCell_008BE560);
namespace { int pluginCell_008BE564; }
template <> unsigned char* const csf::gv<0x008BE564>::d = reinterpret_cast<unsigned char*>(&pluginCell_008BE564);
namespace { int pluginCell_008BE568; }
template <> unsigned char* const csf::gv<0x008BE568>::d = reinterpret_cast<unsigned char*>(&pluginCell_008BE568);
namespace { int pluginCell_008BE56C; }
template <> unsigned char* const csf::gv<0x008BE56C>::d = reinterpret_cast<unsigned char*>(&pluginCell_008BE56C);
namespace { int pluginCell_008BE570; }
template <> unsigned char* const csf::gv<0x008BE570>::d = reinterpret_cast<unsigned char*>(&pluginCell_008BE570);
namespace { int pluginCell_008BE574; }
template <> unsigned char* const csf::gv<0x008BE574>::d = reinterpret_cast<unsigned char*>(&pluginCell_008BE574);
namespace { int pluginCell_008B09DC; }
template <> unsigned char* const csf::gv<0x008B09DC>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B09DC);
namespace { int pluginCell_008B09E0; }
template <> unsigned char* const csf::gv<0x008B09E0>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B09E0);
namespace { int pluginCell_008B0904; }
template <> unsigned char* const csf::gv<0x008B0904>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B0904);
namespace { int pluginCell_008B0908; }
template <> unsigned char* const csf::gv<0x008B0908>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B0908);
namespace { int pluginCell_008B0924; }
template <> unsigned char* const csf::gv<0x008B0924>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B0924);
namespace { int pluginCell_008B0928; }
template <> unsigned char* const csf::gv<0x008B0928>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B0928);
namespace { int pluginCell_008B09F0; }
template <> unsigned char* const csf::gv<0x008B09F0>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B09F0);
namespace { int pluginCell_008B09EC; }
template <> unsigned char* const csf::gv<0x008B09EC>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B09EC);
namespace { int pluginCell_008BE6A0; }
template <> unsigned char* const csf::gv<0x008BE6A0>::d = reinterpret_cast<unsigned char*>(&pluginCell_008BE6A0);
namespace { int pluginCell_008BE6A4; }
template <> unsigned char* const csf::gv<0x008BE6A4>::d = reinterpret_cast<unsigned char*>(&pluginCell_008BE6A4);
namespace { int pluginCell_008BE6A8; }
template <> unsigned char* const csf::gv<0x008BE6A8>::d = reinterpret_cast<unsigned char*>(&pluginCell_008BE6A8);
namespace { int pluginCell_008BE5E0; }
template <> unsigned char* const csf::gv<0x008BE5E0>::d = reinterpret_cast<unsigned char*>(&pluginCell_008BE5E0);
namespace { int pluginCell_008B0A14; }
template <> unsigned char* const csf::gv<0x008B0A14>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B0A14);
namespace { int pluginCell_008BE7A4; }
template <> unsigned char* const csf::gv<0x008BE7A4>::d = reinterpret_cast<unsigned char*>(&pluginCell_008BE7A4);
namespace { int pluginCell_008BE7A8; }
template <> unsigned char* const csf::gv<0x008BE7A8>::d = reinterpret_cast<unsigned char*>(&pluginCell_008BE7A8);
namespace { int pluginCell_008B1294; }
template <> unsigned char* const csf::gv<0x008B1294>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B1294);
namespace { int pluginCell_008B1298; }
template <> unsigned char* const csf::gv<0x008B1298>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B1298);
namespace { int pluginCell_008B129C; }
template <> unsigned char* const csf::gv<0x008B129C>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B129C);
namespace { int pluginCell_008B12A0; }
template <> unsigned char* const csf::gv<0x008B12A0>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B12A0);
namespace { int pluginCell_008B12A4; }
template <> unsigned char* const csf::gv<0x008B12A4>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B12A4);
namespace { int pluginCell_008B12A8; }
template <> unsigned char* const csf::gv<0x008B12A8>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B12A8);
namespace { int pluginCell_008B12AC; }
template <> unsigned char* const csf::gv<0x008B12AC>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B12AC);
namespace { int pluginCell_008B12B0; }
template <> unsigned char* const csf::gv<0x008B12B0>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B12B0);
namespace { int pluginCell_008B12B4; }
template <> unsigned char* const csf::gv<0x008B12B4>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B12B4);
namespace { int pluginCell_008B12B8; }
template <> unsigned char* const csf::gv<0x008B12B8>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B12B8);
namespace { int pluginCell_008B12BC; }
template <> unsigned char* const csf::gv<0x008B12BC>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B12BC);
namespace { int pluginCell_008B12C0; }
template <> unsigned char* const csf::gv<0x008B12C0>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B12C0);
namespace { int pluginCell_008B12C4; }
template <> unsigned char* const csf::gv<0x008B12C4>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B12C4);
namespace { int pluginCell_008B12C8; }
template <> unsigned char* const csf::gv<0x008B12C8>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B12C8);
namespace { int pluginCell_008B0918; }
template <> unsigned char* const csf::gv<0x008B0918>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B0918);
namespace { int pluginCell_008B091C; }
template <> unsigned char* const csf::gv<0x008B091C>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B091C);
namespace { int pluginCell_008B09E4; }
template <> unsigned char* const csf::gv<0x008B09E4>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B09E4);
namespace { int pluginCell_008B09E8; }
template <> unsigned char* const csf::gv<0x008B09E8>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B09E8);
namespace { int pluginCell_008B1348; }
template <> unsigned char* const csf::gv<0x008B1348>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B1348);
namespace { int pluginCell_008B134C; }
template <> unsigned char* const csf::gv<0x008B134C>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B134C);
namespace { int pluginCell_008B1350; }
template <> unsigned char* const csf::gv<0x008B1350>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B1350);
namespace { int pluginCell_008B1354; }
template <> unsigned char* const csf::gv<0x008B1354>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B1354);
namespace { int pluginCell_008B13B0; }
template <> unsigned char* const csf::gv<0x008B13B0>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B13B0);
namespace { int pluginCell_008B13B4; }
template <> unsigned char* const csf::gv<0x008B13B4>::d = reinterpret_cast<unsigned char*>(&pluginCell_008B13B4);

// Particle defaults store these identities, but simulation/advanced stream
// callbacks are deliberately still fatal in production and in these fixtures.
#include <cstdlib>
namespace { int staticFramePluginCell; }
template <> unsigned char* const csf::gv<0x008A6450>::d = reinterpret_cast<unsigned char*>(&staticFramePluginCell);
#include "boundary/rw/toolkit9.h"
#include "boundary/rw/world.h"
namespace {
csf_rw::World* pvsCurrentCell;
uint32_t ltMapStyleCell=2,ltMapRasterCell=38404;
csf_rw::ToolkitDescriptor visibilityDescriptorCell{};
void unavailableVisibilityCallback() { std::abort(); }
}
template <> unsigned char* const csf::gv<0x008BE7A0>::d = reinterpret_cast<unsigned char*>(&pvsCurrentCell);
template <> unsigned char* const csf::gv<0x008BE588>::d = reinterpret_cast<unsigned char*>(&ltMapStyleCell);
template <> unsigned char* const csf::gv<0x008BE5B0>::d = reinterpret_cast<unsigned char*>(&ltMapRasterCell);
template <> unsigned char* const csf::gv<0x008851D8>::d = reinterpret_cast<unsigned char*>(&visibilityDescriptorCell);
template <> void* const csf::fn_slot<0x007FF600>::p = reinterpret_cast<void*>(&unavailableVisibilityCallback);
namespace { void unavailableParticleCallback() { std::abort(); } }
template <> void* const csf::fn_slot<0x00578670>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x00578760>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x00578BA0>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x00578C10>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x00578C60>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x00578CE0>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x00578D40>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x00578DE0>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x00579000>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057A090>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057A1B0>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057A230>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057A380>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057A450>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057A600>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057A9A0>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057ACD0>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057AD40>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057AD80>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057ADE0>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057AE40>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057B280>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057B330>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057B3A0>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057B4A0>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057B5B0>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057B640>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057BA00>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057BA70>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057BAB0>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057BB40>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057BF10>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057BF70>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057BFB0>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057C020>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057EDE0>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057F0F0>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057F450>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
template <> void* const csf::fn_slot<0x0057F7A0>::p = reinterpret_cast<void*>(&unavailableParticleCallback);
