#!/usr/bin/env python3
"""Offline RW boundary inventory; only librw is consulted for replacement APIs.

Input is an unmodified linktest snapshot (before this slice). This is deliberately
not a decompiler or a type oracle. Offset evidence includes a conservative grep
candidate list as well as direct-return local-variable evidence.
"""
import argparse, collections, csv, json, pathlib, re, subprocess, sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
RW = ROOT / 'work/tools/librw'

def classify(n, a):
    if not n:
        if a in (0x57cfe0, 0x57dba0, 0x57eda0, 0x57ef90, 0x57f050): return 'particles/ptank'
        if a in (0x7c5680, 0x7c5ca0, 0x7c8d30, 0x7c9050): return 'raster/texture/image'
        if a == 0x58e680: return 'other boundary/pipeline'
        return 'toolkit/team' if a == 0x7fedc0 else 'toolkit/unknown'
    for pattern, group in [
        ('D3DX|D3D9|DeviceSystem', 'D3D9 driver'),
        ('Prt|PTank|MultiTexture', 'particles/ptank'),
        ('LtMap|PVS|Patch|Morph|ADC|Mipmap', 'other plugins'),
        ('UserData', 'user data'),
        ('Rt2d|Gfx2d|Maestro|Brush|Scene|Water|Toon|Team|WorldImport|BitMatrix|BinMtx|RtDict|MTEffect', 'toolkits'),
        ('HAnim|Skin|RtAnim|CompressedKey', 'hanim/skin/animation'),
        ('Intersection|Collision|BBox', 'collision/bounds'),
        ('MatFX', 'material/matfx'),
        ('Stream|Chunk', 'stream/chunk I/O'),
        ('RegisterPlugin|PluginAttach|PluginRegistry|StaticPlugins', 'plugin registry'),
        ('^(RwFrame|RwMatrix|RwV2d|RwV3d|_rwSqrt|_rwInvSqrt|RtQuat|_rwFrame|_rwObjectHasFrame)', 'core/frame/matrix'),
        ('^RwEngine|^_rwGetNumEngine', 'engine'),
        ('Raster|Texture|TexDictionary|Image|RGBA|Anisot', 'raster/texture/image'),
        ('Camera', 'camera'), ('Light', 'light'), ('Material|MatFX', 'material/matfx'),
        ('World|Sector', 'world/sector'), ('Atomic|Clump', 'geometry/atomic/clump'),
        ('Geometry', 'geometry/atomic/clump'), ('Im2D|Im3D', 'immediate mode'),
        ('FreeList|Resources', 'memory/freelist'), ('File|FSManager', 'file system'),
        ('Random', 'random'),
        ('Audio|DirectSound|DirectInput|DeviceID|Pipeline|LockedPipe', 'other boundary/pipeline'),
    ]:
        if re.search(pattern, n): return group
    return 'unclassified'

# Explicit exceptions; generic object operations below are checked against the
# actual librw headers before they are admitted as replacement candidates.
SPECIAL = {
    'RwOsGetFileInterface': ('rwengine.h', 'Engine::filefuncs [field; not the original filesystem interface layout]'),
    # Evidence-backed name corrections are documented, not written to db/.
    'RpClumpRegisterPluginStream': ('rwplg.h', 'Material::registerPluginStream [database name is wrong]'),
    'RpClumpGetWorld': ('rwobjects.h', 'Light::clump [database name is suspect; light callers]'),
    '_rwPluginRegistryReadDataChunks': ('plg.cpp', 'PluginList::streamRead'),
    'RwImageAllocatePixels': ('image.cpp', 'Image::allocate'),
    'RwImageSetPath': ('image.cpp', 'Image::setSearchPath'),
    'RwImageRegisterImageFormat': ('image.cpp', 'Image::registerFileFormat'),
    'RwStreamRead': ('base.cpp', 'Stream::read8'),
    'RwStreamWrite': ('base.cpp', 'Stream::write8'),
    'RwStreamSkip': ('rwbase.h', 'Stream::seek'),
    'RwStreamOpen': ('base.cpp', 'StreamFile::open / StreamMemory::open'),
    'RwStreamClose': ('rwbase.h', 'Stream::close'),
    'RwStreamFindChunk': ('base.cpp', 'findChunk'),
    'RwStreamReadChunkHeaderInfo': ('base.cpp', 'readChunkHeaderInfo'),
    '_rwStreamWriteVersionedChunkHeader': ('base.cpp', 'writeChunkHeader [uses global version/build]'),
    'RwStreamWriteReal': ('base.cpp', 'Stream::write32 [float array bytes]'),
    'RwStreamWriteInt32': ('base.cpp', 'Stream::write32'),
    'RwStreamReadInt32': ('base.cpp', 'Stream::read32'),
    'RwBBoxCalculate': ('base.cpp', 'BBox::calculate'),
    'RwBBoxAddPoint': ('base.cpp', 'BBox::addPoint'),
    'RwBBoxContainsPoint': ('base.cpp', 'BBox::containsPoint'),
    'RtAnimAnimationStreamGetSize': ('anim.cpp', 'Animation::streamGetSize'),
    'RpWorldForAllClumps': ('rwobjects.h', 'World::clumps [list iteration]'),
    'RpWorldForAllLights': ('rwobjects.h', 'World::localLights / globalLights [list iteration]'),
    'RpClumpForAllAtomics': ('rwobjects.h', 'Clump::atomics [list iteration]'),
    'RpClumpForAllLights': ('rwobjects.h', 'Clump::lights [list iteration]'),
    'RpGeometryForAllMaterials': ('rwobjects.h', 'Geometry::matList [array iteration]'),
    'RwTexDictionaryForAllTextures': ('rwobjects.h', 'TexDictionary::textures [list iteration]'),
    'RwTexDictionaryAddTexture': ('texture.cpp', 'TexDictionary::add'),
    'RwTexDictionaryRemoveTexture': ('texture.cpp', 'TexDictionary::remove'),
    'RwRasterGetPluginOffset': ('rwplg.h', 'Raster::getPluginOffset [PLUGINBASE]'),
    'RwMatrixOptimize': ('base.cpp', 'Matrix::optimize'),
    'RwMatrixUpdate': ('rwbase.h', 'Matrix::update'),
    'RwMatrixMultiply': ('base.cpp', 'Matrix::mult'),
    'RwMatrixInvert': ('base.cpp', 'Matrix::invert'),
    'RwV3dTransformPoint': ('base.cpp', 'V3d::transformPoints'),
    'RwV3dTransformPoints': ('base.cpp', 'V3d::transformPoints'),
    'RwV3dTransformVector': ('base.cpp', 'V3d::transformVectors'),
    'RwV3dTransformVectors': ('base.cpp', 'V3d::transformVectors'),
    'RwV3dLength': ('rwbase.h', 'length(const V3d&)'),
    'RwV2dLength': ('rwbase.h', 'length(const V2d&)'),
    'RwV3dNormalize': ('rwbase.h', 'normalize(const V3d&)'),
    '_rwSqrt': ('rwbase.h', 'length(const V3d&) [sqrtf; no standalone API]'),
    '_rwInvSqrt': ('rwbase.h', 'normalize(const V3d&) [reciprocal sqrt; no standalone API]'),
    'RwFrameSetIdentity': ('rwbase.h', 'Matrix::setIdentity [Frame::matrix + updateObjects]'),
    '_rwFrameSyncDirty': ('frame.cpp', 'Frame::syncDirty'),
    'RwFrameGetRoot': ('rwobjects.h', 'Frame::root [field; no function]'),
    'RwFrameForAllObjects': ('rwobjects.h', 'Frame::objectList [iteration; no function]'),
    '_rwObjectHasFrameSetFrame': ('rwobjects.h', 'ObjectWithFrame::setFrame'),
    'RtQuatRotate': ('base.cpp', 'Quat::rotate'),
    'RtQuatConvertFromMatrix': ('base.cpp', 'Matrix::getRotation'),
    'RpHAnimHierarchyGetMatrixArray': ('rwplugins.h', 'HAnimHierarchy::matrices [field]'),
    'RpHAnimFrameGetHierarchy': ('hanim.cpp', 'HAnimHierarchy::get'),
    'RpHAnimFrameGetID': ('rwplugins.h', 'HAnimData::id [field]'),
    'RpHAnimFrameSetID': ('hanim.cpp', 'HAnimData::get [id field]'),
    'RpHAnimHierarchyAttach': ('hanim.cpp', 'HAnimHierarchy::attach'),
    'RpHAnimHierarchyUpdateMatrices': ('hanim.cpp', 'HAnimHierarchy::updateMatrices'),
    'RpHAnimHierarchyDestroy': ('hanim.cpp', 'HAnimHierarchy::destroy'),
    'RpSkinAtomicSetHAnimHierarchy': ('rwplugins.h', 'Skin::setHierarchy'),
    'RpSkinGeometryGetSkin': ('rwplugins.h', 'Skin::get'),
    'RpSkinIsSplit': ('rwplugins.h', 'Skin::numMeshes [field; inspect split data]'),
    'RpAnisotGetMaxSupportedMaxAnisotropy': ('texture.cpp', 'getMaxSupportedMaxAnisotropy'),
    'RpAnisotTextureSetMaxAnisotropy': ('texture.cpp', 'Texture::setMaxAnisotropy'),
    'RpMatFXAtomicEnableEffects': ('matfx.cpp', 'MatFX::enableEffects'),
    'RpMatFXAtomicQueryEffects': ('matfx.cpp', 'MatFX::getEffects(Atomic*)'),
    'RpMatFXMaterialGetEffects': ('matfx.cpp', 'MatFX::getEffects(Material*)'),
    'RpMatFXMaterialSetEffects': ('matfx.cpp', 'MatFX::setEffects'),
    'RpMatFXMaterialSetEnvMapFrame': ('matfx.cpp', 'MatFX::setEnvFrame'),
    'RpMatFXMaterialSetEnvMapFrameBufferAlpha': ('matfx.cpp', 'MatFX::setEnvFBAlpha'),
    'RpMatFXMaterialSetUVTransformMatrices': ('matfx.cpp', 'MatFX::setUVTransformMatrices'),
    'RwEngineRegisterPlugin': ('rwengine.h', 'Engine::registerPlugin'),
    'RwTextureSetFindCallBack': ('rwobjects.h', 'Texture::findCB [field]'),
    'RwTextureSetReadCallBack': ('rwobjects.h', 'Texture::readCB [field]'),
    'RwTexDictionarySetCurrent': ('texture.cpp', 'TexDictionary::setCurrent'),
    'RwTexDictionaryGetCurrent': ('texture.cpp', 'TexDictionary::getCurrent'),
    'RwTexDictionaryForAllTexDictionaries': ('texture.cpp', 'TextureGlobals::texDicts [private list; new enumeration adapter needed]'),
    'RwRasterGetWidth': ('rwobjects.h', 'Raster::width [field]'),
    'RwImageSetHeight': ('rwobjects.h', 'Image::height [field]'),
    'RwImageSetWidth': ('rwobjects.h', 'Image::width [field]'),
    'RwRasterSetFromImage': ('raster.cpp', 'Raster::setFromImage'),
    'RwImageSetFromRaster': ('raster.cpp', 'Raster::toImage'),
    'RwImageFindRasterFormat': ('raster.cpp', 'Raster::imageFindRasterFormat'),
    'RwD3D9GetCurrentD3DDevice': ('d3d/rwd3d.h', 'd3d::d3ddevice [native device pointer]'),
    'RwD3D9SetTexture': ('d3d/d3ddevice.cpp', 'd3d::setTexture'),
    'RwD3D9EngineSetMultiSamplingLevels': ('engine.cpp', 'Engine::setMultiSamplingLevels'),
    'RtAnimAnimationCreate': ('anim.cpp', 'Animation::create'),
    'RtAnimAnimationGetNumNodes': ('anim.cpp', 'Animation::getNumNodes'),
    'RtAnimRegisterInterpolationScheme': ('anim.cpp', 'AnimInterpolatorInfo::registerInterp'),
    'RtAnimInterpolatorDestroy': ('anim.cpp', 'AnimInterpolator::destroy'),
    'RtAnimInterpolatorSetCurrentAnim': ('anim.cpp', 'AnimInterpolator::setCurrentAnim'),
    'RtAnimInterpolatorAddAnimTime': ('anim.cpp', 'AnimInterpolator::addTime'),
    'RtAnimAnimationRead': ('anim.cpp', 'Animation::streamRead [needs StreamFile opening]'),
    'S_006fdfe0_FindWorldFromClump': ('rwobjects.h', 'Clump::world [field]'),
}
PLUGINS = {'Anisot':'registerAnisotropyPlugin','HAnim':'registerHAnimPlugin',
           'Skin':'registerSkinPlugin','UserData':'registerUserDataPlugin',
           'MatFX':'registerMatFXPlugin','World':'registerWorldPlugin'}
OBJECTS = {'Frame':('rwobjects.h','frame.cpp'), 'Matrix':('rwbase.h','base.cpp'),
           'Image':('rwobjects.h','image.cpp'), 'Raster':('rwobjects.h','raster.cpp'),
           'Camera':('rwobjects.h','camera.cpp'), 'Texture':('rwobjects.h','texture.cpp'),
           'TexDictionary':('rwobjects.h','texture.cpp'), 'World':('rwobjects.h','world.cpp'),
           'WorldSector':('rwobjects.h','world.cpp'), 'Atomic':('rwobjects.h','clump.cpp'),
           'Clump':('rwobjects.h','clump.cpp'), 'Geometry':('rwobjects.h','geometry.cpp'),
           'Light':('rwobjects.h','light.cpp'), 'Material':('rwobjects.h','geometry.cpp'),
           'Engine':('rwengine.h','engine.cpp')}
DIFFERENCES = {
    'RwImageAllocatePixels':'Image::allocate returns void; preserve input Image handle and allocation-failure result; bpp inserts a layout field.',
    'RwImageSetFromRaster':'Raster::toImage allocates/returns an Image rather than filling the caller-provided Image; copy/convert into the supplied ABI image and preserve ownership.',
    'RwImageSetPath':'librw owns a search-path representation, not the old returned char-pointer contract; keep ABI-visible path storage.',
    'RwTextureSetFindCallBack':'Native findCB is a field; setter must return the previous callback and bridge Texture pointers.',
    'RwTextureSetReadCallBack':'Native readCB is a field; setter must return the previous callback and bridge Texture/Raster ownership.',
    'RwTextureSetName':'Native name is a 32-byte field, not a setter API; implement truncation/NUL policy and return the ABI texture.',
    'RwTexDictionaryForAllTexDictionaries':'Native global dictionary list is private TextureGlobals storage; expose an enumeration adapter, translating pointers and early-stop callbacks.',
    'RwStreamOpen':'Construct StreamFile or StreamMemory from original stream type/access enums and unions; C stream object cannot be reinterpreted as a virtual Stream.',
    'RwStreamClose':'Virtual close/destructor versus C close callback/data contract; preserve close ownership and original success return.',
    'RwStreamWrite':'Native write8 returns byte count; game API returns stream handle/null on failure, not the count.',
    'RwStreamSkip':'Native seek is void and takes whence; check/retain failure policy and return original stream handle.',
    '_rwStreamWriteVersionedChunkHeader':'librw writeChunkHeader uses global version/build; original has explicit version parameter, so an explicit header adapter is needed.',
    'RwD3D9CreateVertexShader':'Native createVertexShader returns a shader pointer; original takes an output pointer/status contract. Native COM handle can be shared.',
    'RwD3D9CreatePixelShader':'Native createPixelShader returns a shader pointer; original takes an output pointer/status contract. Native COM handle can be shared.',
    'RwD3D9SetTexture':'Native setTexture takes stage first and Texture second; translate original argument order and Texture/Raster sidecar before touching the cache.',
    'RwD3D9EngineSetMultiSamplingLevels':'Native setter is Engine-level; translate levels, pre-open lifecycle and status policy rather than merely storing a global.',
    'RtAnimAnimationCreate':'Original uses interpolation type ID; native create uses AnimInterpolatorInfo*. Resolve scheme registration and bridge keyframe/custom-data ownership.',
    'RtAnimInterpolatorAddAnimTime':'Native addTime returns void and supports a reduced interpolator contract; original callbacks/time wrapping/subhierarchy policy require an adapter.',
    'RpHAnimFrameSetID':'No native setter function: write HAnimData::get(mappedFrame)->id; constructor/plugin offset differs.',
    'RpMatFXMaterialSetEnvMapFrame':'Obtain MatFX extension then setEnvFrame; translate Frame to sidecar and return original Material pointer.',
    'RpMatFXMaterialSetEnvMapFrameBufferAlpha':'Obtain MatFX extension then setEnvFBAlpha; native method returns void, original returns Material handle.',
    'RpMatFXMaterialSetUVTransformMatrices':'Extension method versus Material API; matrices may share the compatible POD layout, but extension offsets/ownership do not.',
    'RpClumpGetWorld':'Existing name likely wrong: light callers use result as Clump. Native Light::clump is a field, so confirm package/plugin identity before installing.',
    'RpClumpRegisterPluginStream':'Existing name wrong: original uses Material registry (same as MaterialRegisterPlugin); native Material registry offsets/callback pointers must be translated.',
}

def replacement(n):
    if n in SPECIAL: return SPECIAL[n]
    if n.endswith('PluginAttach'):
        p = n[2:-12]
        if p in PLUGINS:
            fn = PLUGINS[p]
            for f in (RW/'src').glob('*.cpp'):
                if fn+'(' in f.read_text(): return (f.name, fn)
        return None
    # Native D3D cache/resource wrappers, not all are public.
    if n.startswith('RwD3D9'):
        fn = n[6:]; fn = fn[0].lower()+fn[1:]
        for f in (RW/'src/d3d').glob('*.cpp'):
            if re.search(r'\b'+re.escape(fn)+r'\s*\(', f.read_text()): return ('d3d/'+f.name, 'd3d::'+fn)
        return None
    if 'UserDataArray' in n:
        if n.startswith('RpUserDataArray'):
            op = n.removeprefix('RpUserDataArray')
            op = {'GetReal':'getFloat','GetInt':'getInt','GetName':'name [field]'}.get(op)
        else:
            match = re.match(r'(?:Rp|Rw)(\w+?)(Remove|Get)UserDataArray(Count)?$',n)
            if not match: return None
            cl,op,count = match.groups(); op = cl.lower()+('Remove' if op=='Remove' else 'GetCount' if count else 'Get')
        return ('rwuserdata.h', 'UserDataArray::'+op) if op else None
    match = re.match(r'(?:Rp|Rw)('+ '|'.join(sorted(OBJECTS,key=len,reverse=True))+r')(.+)', n)
    if match:
        cl, op = match.groups(); h, cpp = OBJECTS[cl]
        if op in ('RegisterPlugin','RegisterPluginStream'):
            return ('rwplg.h',cl+'::'+op[0].lower()+op[1:]+' [PLUGINBASE]')
        op = {'GetNumLevels':'getNumLevels','SetViewWindow':'setViewWindow',
              'SetNearClipPlane':'setNearPlane','SetFarClipPlane':'setFarPlane',
              'GetConeAngle':'getAngle','SetConeAngle':'setAngle',
              'SetRadius':'radius','GetWorld':'world','SetPipeline':'pipeline',
              'GetNumAtomics':'countAtomics','GetNumLights':'countLights',
              'SetSectorRenderCallBack':'sectorRenderCB','GetSectorRenderCallBack':'sectorRenderCB',
              'FindNamedTexture':'find','SetName':'name','SetProjection':'projection',
              'StreamGetSize':'streamGetSize','RegisterPluginStream':'registerPluginStream',
              'RegisterPlugin':'registerPlugin'}.get(op,op[0].lower()+op[1:])
        text = (RW/'src'/h).read_text()
        block = text.split('struct '+cl+'\n',1)[-1].split('\n};',1)[0]
        if re.search(r'\b'+re.escape(op)+r'\s*\(',block): return (cpp if (RW/'src'/cpp).exists() else h, cl+'::'+op)
        if re.search(r'\b'+re.escape(op)+r'\s*(?:;|\[)',block): return (h,cl+'::'+op+' [field]')
    if n.startswith('RwStream') or n.startswith('_rwStream'):
        op=n.split('Stream',1)[1]; op=op[0].lower()+op[1:]
        if re.search(r'\b'+op+r'\s*\(', (RW/'src/rwbase.h').read_text()): return ('base.cpp','Stream::'+op)
    im = {'RwIm3DTransform':'im3DTransform','RwIm3DEnd':'im3DEnd',
          'RwIm3DRenderIndexedPrimitive':'im3DRenderIndexedPrimitive','RwIm3DRenderPrimitive':'im3DRenderPrimitive'}
    if n in im: return ('render.cpp',im[n])
    return None

def fit(n, group, repl):
    if not repl: return 'missing', 'No corresponding API found in this checkout; implement game boundary independently, not original library code.'
    if n in DIFFERENCES:return 'differs',DIFFERENCES[n]
    if n in ('_rwSqrt','_rwInvSqrt','RwV3dLength','RwV2dLength','RwV3dNormalize'):
        return 'differs','librw uses sqrtf, not the original lookup approximation; Normalize returns a vector, game API also returns length; zero-vector guard needed.'
    if group == 'core/frame/matrix' and 'Frame' not in n and 'Quat' not in n:
        return 'same semantics','Compatible 32-bit value layout and operation; last-bit float differences allowed. No bitwise equivalence claim.'
    if group == 'core/frame/matrix' and 'Frame' in n:
        return 'differs','Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns.'
    if group == 'user data' and n.startswith('RpUserDataArray'):
        return 'same semantics','Equivalent value/name retrieval; array handle must be the mapped extension.'
    if group == 'D3D9 driver':
        return 'differs','Native D3D9 backend only; translate handles/signatures and respect or invalidate librw state caches after direct COM calls.'
    if group == 'stream/chunk I/O':
        return 'differs','Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions.'
    if group == 'hanim/skin/animation':
        return 'differs','Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate.'
    if group == 'plugin registry' or 'RegisterPlugin' in n or n.endswith('PluginAttach'):
        return 'differs','librw plugin sizes/offsets and callback object pointers are different; maintain separate ABI registry and bridge callbacks/streams.'
    if group == 'immediate mode':
        return 'differs','Vertex declaration/UV and camera-Z conventions are backend-specific; convert arrays and primitive enum, not raw passthrough.'
    return 'differs','Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section.'

def evidence(rows):
    """Count direct local return taints, and separately grep candidates by carrier name.

    Full interprocedural alias/field tracking is NOT claimed. Candidate lists are
    useful for the next layout audit, not safe rewrites.
    """
    direct=collections.defaultdict(lambda:collections.defaultdict(set))
    candidates=collections.defaultdict(lambda:collections.defaultdict(set))
    details=[]; device=[]
    for f in sorted((ROOT/'src/auto').glob('*.cpp')):
        text=f.read_text(); seeds={}
        for m in re.finditer(r'\b(\w+)\s*=\s*(?:reinterpret_cast<[^;]+?>\s*\()?((?:Rw|Rp|Rt)\w+)\s*\(',text):
            var,n=m.groups(); cl=None
            if n in ('RwFrameGetLTM', 'RwMatrixCreate','RwMatrixInvert'): cl='RwMatrix'
            else:
                mm=re.match(r'(Rw|Rp|Rt)(TexDictionary|HAnimHierarchy|Atomic|Clump|Geometry|Material|Texture|Raster|Camera|Light|World|Frame|Image|AnimAnimation|AnimInterpolator)(?:Create|Clone|StreamRead|Read|GetSkin)',n)
                if mm: cl=mm[1]+mm[2]
            if cl: seeds[var]=cl
        # A simple local passed as object argument 0 gives additional boundary
        # evidence even when it arrived via a wrapper field or callback argument.
        for m in re.finditer(r'\b((?:Rw|Rp)(?:Frame|Matrix|Atomic|Clump|Geometry|Material|Texture|Raster|Camera|Light|World|Image)\w+)\s*\(\s*(\w+)\s*[,)]',text):
            n,var=m.groups()
            if var in ('int','float','void','const','uint32_t') or re.search('Create|Register|Plugin|SetPath|GetPath|SetFindCallBack|SetReadCallBack',n): continue
            cl = re.match(r'(Rw|Rp)(Frame|Matrix|Atomic|Clump|Geometry|Material|Texture|Raster|Camera|Light|World|Image)',n)
            if cl:
                objtype=cl[1]+cl[2]
                if var in seeds and seeds[var] != objtype: continue
                seeds[var]=objtype
        for m in re.finditer(r'CSF_AT\(\s*([^,]+),\s*(?:reinterpret_cast<[^>]+>\s*\(\s*)?(\w+)\s*\)?\s*,\s*(0x[\da-fA-F]+|\d+)\s*\)',text):
            typ,var,off=m.groups(); off=int(off,0)
            if var in seeds:
                cl=seeds[var];direct[cl][off].add(f.name)
                details.append([cl,hex(off),f.name,m.group(0),'boundary local (return or argument)'])
            # Variable spelling is only a search aid; offsets on owner's fields can
            # still be false positives. Keep these apart from proved return taints.
            for pat,cl in [('frame','RwFrame'),('matrix|ltm','RwMatrix'),('clump','RpClump'),
                           ('atomic','RpAtomic'),('geometry','RpGeometry'),('texture','RwTexture'),
                           ('raster','RwRaster'),('camera','RwCamera'),('light','RpLight'),
                           ('world|sector','RpWorld/sector'),('material','RpMaterial'),
                           ('image','RwImage'),('hierarchy','RpHAnimHierarchy'),
                           ('skin','RpSkin'),('interpolator','RtAnimInterpolator'),('anim','RtAnimAnimation')]:
                if re.search(pat,var,re.I):
                    candidates[cl][off].add(f.name)
                    details.append([cl,hex(off),f.name,m.group(0),'name-only candidate'])
        if re.search(r'D3D|device|Device',text):
            for m in re.finditer(r'(?:CSF_AT\(void\*\*,\s*\w+,\s*0\)|deviceVtable)\s*\[([^\]]+)\]',text):
                device.append([f.name,m.group(0)])
    return direct,candidates,details,device

def main():
    ap=argparse.ArgumentParser();ap.add_argument('snapshot');ap.add_argument('--output',default='docs/RW_PORT_INVENTORY.md');ap.add_argument('--layouts',action='store_true',help='Compile Win32 librw record layouts with linktest.flags()');ap.add_argument('--check-cpu-abi',action='store_true',help='Check conventions/stack argument counts against export/abi.json');a=ap.parse_args()
    snap=json.load(open(a.snapshot)); refs=snap['unresolved_functions']['rw']
    rows={int(r['ea'],16):r for r in csv.DictReader(open(ROOT/'db/functions.csv'))}
    out=['# RW slot inventory\n','Generated with `tools/rw_survey.py` from the pre-slice linktest snapshot. References are distinct compiled caller files, not call-site counts. Paths below are relative to `work/tools/librw/src/`. Read [RW_PORT.md](RW_PORT.md) for qualifications and architecture.\n']
    inventory=[];totals=collections.defaultdict(collections.Counter)
    for key,count in sorted(refs.items(),key=lambda kv:int(kv[0],16)):
        ea=int(key,16);n=rows.get(ea,{}).get('name','');g=classify(n,ea)
        image_callbacks={0x7c5680:('bmp.cpp','readBMP'),0x7c5ca0:('bmp.cpp','writeBMP'),
                         0x7c8d30:('png.cpp','writePNG'),0x7c9050:('png.cpp','readPNG')}
        r=image_callbacks.get(ea) if not n else replacement(n)
        f,why=fit(n,g,r)
        totals[g][f]+=1
        inventory.append(dict(address=key,name=n or 'unnamed',references=count,subsystem=g,
                              replacement=(r[0]+': rw::'+r[1]) if r else 'none',fit=f,notes=why))
        if not n:
            inventory[-1]['notes'] += {
                0x57cfe0:' High confidence particle batch update callback: caller installs in particle class; dt argument, property offsets and PTank arrays. Formal PrtStd callback name uncertain.',
                0x57dba0:' High confidence emitter emission callback: class callback slot 0, emitter property tables, randomized particle initialization and PTank output. Formal PrtStd name uncertain.',
                0x57eda0:' High confidence emitter render callback shape: tests flag 0x200, property ID 1, calls RpAtomicRender on the PTank atomic; formal name uncertain.',
                0x57ef90:' High confidence emitter begin-update callback shape: caches atomic frame LTM, adds dt to emitter time, updates particle rotation using 2*pi wrap; installed in callback slot 1. Formal name uncertain.',
                0x57f050:' High confidence emitter end-update/scheduling callback shape: installed in slot 2; LCG constants 196314165/907633515, next emission time from bias/spread. Formal name uncertain.',
                0x58e680:' High confidence atomic pipeline render callback shape: chooses atomic +0x70 pipeline or engine default, invokes RxPipelineExecute, returns atomic/null; medium confidence RpAtomicDefaultRenderCallBack identity. No equivalent Rx graph in librw.',
                0x7c5680:' High confidence BMP filename reader: stream mode read, signature 19778 (BM), DIB header sizes and palette handling. Native image layout differs.',
                0x7c5ca0:' High confidence BMP filename writer: paired BMP header/palette/pixel writers, stream mode write; original returns image/null whereas librw returns void.',
                0x7c8d30:' High confidence PNG filename writer: libpng 1.0.12 create-write context, setjmp, image palette/RGBA conversion; original returns image/null whereas librw returns void. This is RW image glue, not a reimplementation of libpng.',
                0x7c9050:' High confidence PNG filename reader: libpng 1.0.12 create-read context, setjmp, read info/palette/pixels; native Image layout differs. This is RW image glue, not libpng internals.',
                0x7e4970:' High confidence: one-byte RET, no side effects; formal toolkit name unknown. Caller passes child count.',
                0x7f1810:' High confidence shape: registers three dictionary/schema descriptors for type IDs 4, 9, 2; exact package/name uncertain, audio-facing caller.',
                0x7fedc0:' Medium confidence identity: team/player transform-output query gated by membership bitset; +0x20 player ID, +0x68 membership array, caller consumes six output dwords. Formal API name unknown.',
            }.get(ea,' Identity unknown; no speculative API name assigned.')
    installed={int(s,16) for s in re.findall(r'HOOK\((0x[\da-fA-F]+)',(ROOT/'src/boundary/rw/cpu.cpp').read_text())}
    tail=[]
    for r in inventory:
        n,g=r['name'],r['subsystem']
        if int(r['address'],16) in installed: job=1
        elif g in ('engine','plugin registry','file system','random','memory/freelist','core/frame/matrix') or n.startswith(('RwStream','_rwStream')): job=2
        elif g=='raster/texture/image':job=3
        elif g in ('camera','light','geometry/atomic/clump') or (g=='material/matfx' and 'MatFX' not in n):job=4
        elif g in ('hanim/skin/animation','user data','material/matfx','other plugins'):job=5
        elif g in ('world/sector','collision/bounds','stream/chunk I/O'):job=6
        elif g in ('D3D9 driver','immediate mode','other boundary/pipeline','unclassified'):job=7
        elif g=='particles/ptank':job=8
        else:tail.append(r);continue
        r['slice']=job
    midpoint=(len(tail)+1)//2
    for i,r in enumerate(tail):r['slice']=9 if i<midpoint else 10
    out += ['## Totals\n','| Subsystem | Same semantics | Differs | Missing | Total |','|---|---:|---:|---:|---:|']
    for g,c in sorted(totals.items()):out.append(f"| {g} | {c['same semantics']} | {c['differs']} | {c['missing']} | {sum(c.values())} |")
    c=sum(totals.values(),collections.Counter());out.append(f"| **Total** | **{c['same semantics']}** | **{c['differs']}** | **{c['missing']}** | **{sum(c.values())}** |")
    out+=['\n## Slice allocation\n','Each slot is allocated exactly once. Slice 1 is implemented; 2–10 are a plan, not stubs.\n','| Slice | Slots |','|---|---:|']
    for job,count in sorted(collections.Counter(r['slice'] for r in inventory).items()):out.append(f'| {job} | {count} |')
    out+=['\n## All slots\n','Candidate mappings are source-level correspondences, NOT completed adapters or proven behavioral equivalence. `none` includes absent exact APIs and missing toolkits. Database names (especially toolkit names) remain provisional; see corrections in RW_PORT.md.\n','| Address | Name | Refs | Subsystem | Slice | librw file / function or field | Fit / adaptation |','|---|---|---:|---|---:|---|---|']
    for r in inventory:out.append(f"| `{r['address']}` | {r['name']} | {r['references']} | {r['subsystem']} | {r['slice']} | {r['replacement']} | **{r['fit']}**: {r['notes']} |")
    (ROOT/a.output).write_text('\n'.join(out)+'\n')
    (ROOT/'docs/RW_PORT_SLOTS.json').write_text(json.dumps(inventory,indent=2)+'\n')
    direct,candidates,details,device=evidence(rows)
    out=['# RW offset evidence\n','This is a conservative grep audit, NOT complete interprocedural type recovery. Boundary-local taints (returns or simple arguments) and name-only candidates are deliberately separate. A file is counted once per offset. Reassignments/aliases need manual review; these are not a reaching-definition proof. For exact carriers and matches see RW_PORT_OFFSETS.csv. Offsets that refer to the game wrapper rather than an RW object are possible in the candidate column.\n','| Structure | Boundary locals: offset (files) | Name-only candidates: offset (files) |','|---|---|---|']
    for cl in sorted(direct.keys()|candidates.keys()):
        fmt=lambda d:', '.join(f'`{hex(o)}` ({len(fs)})' for o,fs in sorted(d.items())) or 'none'
        out.append(f'| {cl} | {fmt(direct[cl])} | {fmt(candidates[cl])} |')
    out+=['\n## Direct device vtable grep\n','These are candidates, not an exhaustive device-flow proof. The manually confirmed methods are in RW_PORT.md.\n']
    out += [f'- `{f}`: `{s}`' for f,s in device]
    (ROOT/'docs/RW_PORT_OFFSETS.md').write_text('\n'.join(out)+'\n')
    with open(ROOT/'docs/RW_PORT_OFFSETS.csv','w') as f:
        w=csv.writer(f,lineterminator='\n');w.writerow(['structure','offset','file','expression','confidence']);w.writerows(details)
    design=ROOT/'docs/RW_PORT_DESIGN.md'
    if design.exists():
        (ROOT/'docs/RW_PORT.md').write_text(design.read_text()+'\n'+
            (ROOT/a.output).read_text()+'\n'+(ROOT/'docs/RW_PORT_OFFSETS.md').read_text())
    if a.layouts:
        sys.path.insert(0,str(ROOT/'tools'))
        import linktest
        names=['Matrix','Frame','ObjectWithFrame','Image','Raster','Texture','TexDictionary','Material','Geometry','Atomic','Clump','Camera','Light','World','Animation','AnimInterpolator','HAnimHierarchy','Skin','UserDataArray','MatFX']
        probe=ROOT/'build/rw_layouts.cpp'
        probe.write_text('#include "'+str(RW/'rw.h')+'"\n'+'\n'.join('int size_'+n+' = sizeof(rw::'+n+');' for n in names)+'\n')
        r=subprocess.run(linktest.flags()+['-Xclang','-fdump-record-layouts','/Fo'+str(ROOT/'build/rw_layouts.obj'),'-c','--',str(probe)],cwd=ROOT/'build',text=True,capture_output=True)
        if r.returncode:raise RuntimeError(r.stderr)
        blocks=[b for b in r.stdout.split('*** Dumping AST Record Layout') if any('struct rw::'+n+'\n' in b or 'class rw::'+n+'\n' in b for n in names)]
        (ROOT/'docs/RW_PORT_LIBRW_LAYOUTS.txt').write_text('Compiler-derived Win32 librw layouts, using tools/linktest.py flags().\nCheckout: 18532c2e13efbc1aa43f0b00b4459831f9414de0\nNo original RW layout is implied by this file.\n'+''.join('*** Dumping AST Record Layout'+b for b in blocks))
        print('Win32 layouts compiled:',len(blocks),'records')
    if a.check_cpu_abi:
        abi=json.loads((ROOT/'export/abi.json').read_text())
        source=(ROOT/'src/boundary/rw/cpu.cpp').read_text();passed=0
        for addr,fn in re.findall(r'HOOK\((0x[\da-fA-F]+),\s*(\w+)',source):
            m=re.search(r'static\s+[^\n{};]+\s+(__cdecl|__stdcall)\s+'+fn+r'\(([^\n]*)\)\s*\{',source)
            if not m:raise RuntimeError((addr,fn,'definition not parsed'))
            conv,params=m.groups();count=len(params.split(',')) if params.strip() else 0
            original=abi[f'{int(addr,16):08x}']
            if count!=len(original['args']) or conv.removeprefix('__')!=original['cc']:
                raise RuntimeError((addr,fn,conv,count,original['cc'],len(original['args'])))
            passed+=1
        print('CPU export ABI conventions and argument counts:',passed,'PASS (not a return-type or behavioral check)')
    print(f"{len(inventory)} RW slots: {dict(c)}; {len(details)} offset matches")

if __name__=='__main__':main()
