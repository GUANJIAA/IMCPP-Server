aux_source_directory(. SRC_LIST)
aux_source_directory(../proto proto_LIST)
aux_source_directory(../mysqldb mysqldb_LIST)
aux_source_directory(../model model_LIST)
add_executable(OfflineMsgServer ${SRC_LIST} ${proto_LIST} 
                ${mysqldb_LIST} ${model_LIST})
target_link_libraries(OfflineMsgServer 
                        rpc
                        protobuf
                        "/usr/lib64/mysql/libmysqlclient.so")