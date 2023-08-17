-- chatmsg_redis.lua

local json = require("json")
local ops = require("redisOps")

local row = ops.rawRow()
local action = ops.rawAction()

if action == "insert" or action == "update"
then 
    local msgid = row["msgid"]
    local recvName = row["recvname"]
    local sendName = row["sendname"]
    local message = row["message"]

    local result = {}
    result["msgid"] = msgid
    result["recvName"] = recvName
    result["sendName"] = sendName
    result["message"] = message
    local value = json.encode(result)

    ops.SADD("chatMsg:"..recvName,value)
    ops.SADD("chatMsg:"..sendName,value)
else
    if action == "delete"
    then
        local msgid = row["msgid"]
        local recvName = row["recvname"]
        local sendName = row["sendname"]
        local message = row["message"]

        local result = {}
        result["msgid"] = msgid
        result["recvName"] = recvName
        result["sendName"] = sendName
        result["message"] = message
        local value = json.encode(result)

        ops.SREM("chatMsg:"..recvName,value)
        ops.SREM("chatMsg:"..sendName,value)
    end
end