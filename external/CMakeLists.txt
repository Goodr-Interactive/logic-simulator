if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/catch)
    set(CATCH_FOUND ON PARENT_SCOPE)

    add_subdirectory(catch)
else()
    set(CATCH_FOUND OFF PARENT_SCOPE)
endif()
