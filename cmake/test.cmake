file(GLOB test_pario_list
	"${PROJECT_SOURCE_DIR}/test/pario/*.cpp"
	"${PROJECT_SOURCE_DIR}/test/pario/*.cc"
	)

foreach(test_src ${test_pario_list})
	get_filename_component(tgt_name ${test_src} NAME_WE)
	add_executable(${tgt_name} ${test_src})
    target_link_libraries(${tgt_name} lsol_pario lsol_util)
	SET_PROPERTY(TARGET ${tgt_name} PROPERTY FOLDER "test/pario")
	list(APPEND test_targets ${tgt_name})
endforeach()

install(TARGETS ${test_targets}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION bin
	ARCHIVE DESTINATION lib
	)

install(DIRECTORY data
    DESTINATION bin
    )