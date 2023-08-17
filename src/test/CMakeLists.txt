aux_source_directory(../proto proto_LIST)
add_executable(test ./test.cpp ${proto_LIST})

add_executable(test_1 ./test_1.cpp ${proto_LIST})

target_link_libraries(test 
                        rpc
                        jsoncpp
                        protobuf)

target_link_libraries(test_1 
                        rpc
                        jsoncpp
                        protobuf)