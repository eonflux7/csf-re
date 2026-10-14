# First-link diagnostic exe. Not ALL: `ninja -C build` remains the hooked DLL.
# Reuse linktest's private compile cache, NEVER csf_hook objects. This also keeps
# flags identical to the audited linktest.flags() MSVC x86 configuration.
find_program(CSF_PYTHON NAMES python PATHS "${CMAKE_SOURCE_DIR}/.venv/bin" NO_DEFAULT_PATH REQUIRED)
include("${CMAKE_SOURCE_DIR}/cmake/librw.cmake")
set(CSF_CUTOVER "${CMAKE_BINARY_DIR}/cutover")
file(MAKE_DIRECTORY "${CSF_CUTOVER}")
file(GLOB_RECURSE CSF_STANDALONE_SRC CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE CSF_STANDALONE_HEADERS CONFIGURE_DEPENDS
     "${CMAKE_SOURCE_DIR}/src/*.h" "${CMAKE_SOURCE_DIR}/hook/*.h"
     "${CMAKE_SOURCE_DIR}/src/*.inc" "${CMAKE_SOURCE_DIR}/hook/*.inc")
set(CSF_SLOT_OBJECTS)
foreach(src IN LISTS CSF_STANDALONE_SRC)
  file(RELATIVE_PATH rel "${CMAKE_SOURCE_DIR}" "${src}")
  string(REPLACE "/" "_" obj "${rel}")
  list(APPEND CSF_SLOT_OBJECTS "${CMAKE_SOURCE_DIR}/build/standalone/${obj}.obj")
endforeach()
# linktest.flags() intentionally reads the hooked build's compile_commands.json.
# Keep this target in that build directory; separate objects live in standalone/.
if(NOT CMAKE_BINARY_DIR STREQUAL "${CMAKE_SOURCE_DIR}/build")
  message(STATUS "csf_standalone uses linktest flags/cache from ${CMAKE_SOURCE_DIR}/build (configure it first)")
endif()
add_custom_command(OUTPUT "${CSF_CUTOVER}/base.json"
  COMMAND "${CSF_PYTHON}" "${CMAKE_SOURCE_DIR}/tools/linktest.py" -j 8 --out "${CSF_CUTOVER}/base.json"
  DEPENDS ${CSF_STANDALONE_SRC} ${CSF_STANDALONE_HEADERS}
          "${CMAKE_SOURCE_DIR}/standalone/game_plain.cpp" "${CMAKE_SOURCE_DIR}/standalone/adjustors.cpp"
          "${CMAKE_SOURCE_DIR}/tools/linktest.py" "${CMAKE_SOURCE_DIR}/db/functions.csv"
          "${CMAKE_SOURCE_DIR}/build/compile_commands.json"
  BYPRODUCTS ${CSF_SLOT_OBJECTS}
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  COMMENT "Compile standalone sources and inventory real slots (no traps)")
add_custom_target(csf_standalone_inventory DEPENDS "${CSF_CUTOVER}/base.json")

set(CSF_DATA "${CSF_CUTOVER}/data/data.cpp")
set(CSF_INITS "${CSF_CUTOVER}/initializers.cpp")
add_custom_command(OUTPUT "${CSF_DATA}" "${CSF_INITS}"
  COMMAND "${CSF_PYTHON}" "${CMAKE_SOURCE_DIR}/tools/datagen.py"
          --linktest "${CSF_CUTOVER}/base.json" --output "${CSF_CUTOVER}/data"
          --report "${CSF_CUTOVER}/data/datagen.json"
  COMMAND "${CSF_PYTHON}" "${CMAKE_SOURCE_DIR}/tools/initgen.py"
          --linktest "${CSF_CUTOVER}/base.json" --output "${CSF_CUTOVER}"
  DEPENDS "${CSF_CUTOVER}/base.json" "${CMAKE_SOURCE_DIR}/tools/datagen.py"
          "${CMAKE_SOURCE_DIR}/tools/initgen.py" "${CMAKE_SOURCE_DIR}/work/pc/CommXPC.exe"
          "${CMAKE_SOURCE_DIR}/export/dataptrs.json" "${CMAKE_SOURCE_DIR}/db/vtables.json"
          "${CMAKE_SOURCE_DIR}/db/datagen_text.yaml"
          "${CMAKE_SOURCE_DIR}/db/regions.yaml" "${CMAKE_SOURCE_DIR}/export/strings.json"
          "${CMAKE_SOURCE_DIR}/export/functions.jsonl"
  BYPRODUCTS "${CSF_CUTOVER}/initializers.json" "${CSF_CUTOVER}/data/manifest.json"
             "${CSF_CUTOVER}/initializers.tsv" "${CSF_CUTOVER}/startup-disassembly.txt"
             "${CSF_CUTOVER}/data/datagen.json" "${CSF_CUTOVER}/data/datagen-rejected.json"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  COMMENT "Generate validated data and original CRT initializer order (reject invalid bindings)")
add_library(csf_standalone_startup OBJECT EXCLUDE_FROM_ALL
            standalone/main.cpp standalone/game_plain.cpp standalone/adjustors.cpp "${CSF_DATA}" "${CSF_INITS}")
add_dependencies(csf_standalone_startup csf_standalone_inventory)
target_compile_definitions(csf_standalone_startup PRIVATE CSF_STANDALONE)
target_include_directories(csf_standalone_startup PRIVATE src hook standalone)
target_compile_options(csf_standalone_startup PRIVATE /W3 /EHsc /Zc:threadSafeInit-
                       -Wno-microsoft-cast -Wno-invalid-offsetof)

# An exact linker-input manifest permits stubbing data-only/vtable-only slots,
# unknown initializer thunks, and the parked WinMain, not just CSF_FUNC users.
string(JOIN "\n" CSF_OBJECT_LINES ${CSF_SLOT_OBJECTS})
file(GENERATE OUTPUT "${CSF_CUTOVER}/objects.txt"
     CONTENT "${CSF_OBJECT_LINES}\n$<JOIN:$<TARGET_OBJECTS:csf_standalone_startup>,\n>\n")
add_custom_command(OUTPUT "${CSF_CUTOVER}/stubs.cpp" "${CSF_CUTOVER}/linktest.json"
  COMMAND "${CSF_PYTHON}" "${CMAKE_SOURCE_DIR}/tools/linktest.py" -j 8
          --objects "${CSF_CUTOVER}/objects.txt" --out "${CSF_CUTOVER}/linktest.json"
  COMMAND "${CSF_PYTHON}" "${CMAKE_SOURCE_DIR}/tools/stubgen.py"
          --linktest "${CSF_CUTOVER}/linktest.json" --objects "${CSF_CUTOVER}/objects.txt"
          --output "${CSF_CUTOVER}"
  DEPENDS csf_standalone_startup $<TARGET_OBJECTS:csf_standalone_startup>
          "${CSF_CUTOVER}/base.json" "${CSF_CUTOVER}/objects.txt"
          "${CMAKE_SOURCE_DIR}/tools/stubgen.py" "${CMAKE_SOURCE_DIR}/db/functions.csv"
  BYPRODUCTS "${CSF_CUTOVER}/stubs.json"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  COMMENT "Inventory all real link inputs, verify and emit only unresolved fatal traps")
set_source_files_properties(${CSF_SLOT_OBJECTS} PROPERTIES EXTERNAL_OBJECT TRUE GENERATED TRUE)
add_executable(csf_standalone WIN32 EXCLUDE_FROM_ALL "${CSF_CUTOVER}/stubs.cpp"
               $<TARGET_OBJECTS:csf_standalone_startup> ${CSF_SLOT_OBJECTS})
add_dependencies(csf_standalone csf_standalone_inventory)
target_compile_definitions(csf_standalone PRIVATE CSF_STANDALONE)
target_include_directories(csf_standalone PRIVATE src hook standalone)
target_compile_options(csf_standalone PRIVATE /W3 /EHsc /Zc:threadSafeInit-)
# The SDK's dinput8.lib by path (its c_dfDIKeyboard/c_dfDIMouse; their GUIDs are in dxguid): plain `dinput8` finds the hook DLL's import library
# build/dinput8.lib first.
target_link_libraries(csf_standalone PRIVATE csf_rw_d3d9_backend kernel32 user32 ole32 ${XWIN}/sdk/lib/um/x86/dinput8.lib dxguid)
target_link_options(csf_standalone PRIVATE "/MAP:${CSF_CUTOVER}/csf_standalone.map" /ERRORLIMIT:0)
