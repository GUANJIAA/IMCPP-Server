aux_source_directory(../proto proto_LIST)
aux_source_directory(../mysqldb mysqldb_LIST)
aux_source_directory(../model model_LIST)
aux_source_directory(../redisdb redisdb_LIST)
add_executable(LoginServer ./main.cpp ./LoginService.cpp ./LoginService.h ${proto_LIST} 
                ${mysqldb_LIST} ${model_LIST} ${redisdb_LIST})
add_executable(LoginStartServer ./LoginServer ${proto_LIST})

target_link_libraries(LoginServer 
                        rpc
                        protobuf
                        "/usr/local/lib64/libjsoncpp.so"
                        hiredis
                        "/usr/lib64/mysql/libmysqlclient.so")

target_link_libraries(LoginStartServer 
                        rpc
                        protobuf
                        jsoncpp)