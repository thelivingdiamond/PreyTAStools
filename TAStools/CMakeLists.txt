set(MOD_NAME TAStools) # CHANGE ME
set(MOD_PREFIX TAS_TOOLS) # CHANGE ME

set(SOURCE_FILES
		CMakeLists.txt
		pch.h
		#Mod Main
		ModMain.cpp ModMain.h
		InputRecorder.cpp InputRecorder.h
		InputNameMap.h
		TAS/TASInfo.h TAS/TASInfo.cpp
		TAS/TASFile.cpp TAS/TASFile.h
		TAS/TASActions.cpp TAS/TASActions.h
		TAS/Actions.cpp TAS/Actions.h
		TAS/Frame.cpp TAS/Frame.h
		TAS/TASReplayer.cpp TAS/TASReplayer.h
		)

add_library(${MOD_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${MOD_NAME}
	CommonMod
)

target_precompile_headers(${MOD_NAME} PRIVATE pch.h)
mod_output_dir(${MOD_NAME} ${MOD_PREFIX})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})
