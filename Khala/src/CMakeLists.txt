AUX_SOURCE_DIRECTORY(. KHALA_SRCS)
MESSAGE(STATUS "Khala/src:" ${KHALA_SRCS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Khala/include)
#set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
add_library(Khala ${KHALA_SRCS})
#target_link_libraries(libKhala pthread muduo_net muduo_base json)

