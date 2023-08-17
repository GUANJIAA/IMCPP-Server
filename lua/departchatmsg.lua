-- departchatmsg_redis.lua

local json = require("json")
local ops = require("redisOps")

local row = ops.rawRow()
local action = ops.rawAction()

if action == "insert" or action == "update"
then 
    local msgid = row["msgid"]
    local departName = row["departname"]
    local sendName = row["sendname"]
    local message = row["message"]

    local result = {}
    result["msgid"] = msgid
    result["departname"] = departName
    result["sendName"] = sendName
    result["message"] = message
    local value = json.encode(result)

    ops.SADD("depatChatMsg:"..departName,value)
else
    if action == "delete"
    then
        local msgid = row["msgid"]
        local departName = row["departname"]
        local sendName = row["sendname"]
        local message = row["message"]

        local result = {}
        result["msgid"] = msgid
        result["departname"] = departName
        result["sendName"] = sendName
        result["message"] = message
        local value = json.encode(result)

        ops.SREM("depatChatMsg:"..departName,value)
    end
end