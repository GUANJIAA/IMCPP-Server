aux_source_directory(. SRC_LIST)
aux_source_directory(../proto proto_LIST)
aux_source_directory(../mysqldb mysqldb_LIST)
aux_source_directory(../model model_LIST)
aux_source_directory(../redispb redispb_LIST)
add_executable(MsgServer ${SRC_LIST} ${proto_LIST} 
                ${mysqldb_LIST} ${model_LIST} ${redispb_LIST})
target_link_libraries(MsgServer 
                        rpc
                        protobuf
                        jsoncpp
                        hiredis
                        "/usr/lib64/mysql/libmysqlclient.so")