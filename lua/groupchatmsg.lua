-- groupchatmsg_redis.lua

local json = require("json")
local ops = require("redisOps")

local row = ops.rawRow()
local action = ops.rawAction()

if action == "insert" or action == "update"
then 
    local msgid = row["msgid"]
    local groupName = row["groupname"]
    local sendName = row["sendname"]
    local message = row["message"]

    local result = {}
    result["msgid"] = msgid
    result["groupname"] = groupName
    result["sendName"] = sendName
    result["message"] = message
    local value = json.encode(result)

    ops.SADD("groupChatMsg:"..groupName,value)
else
    if action == "delete"
    then
        local msgid = row["msgid"]
        local groupName = row["groupname"]
        local sendName = row["sendname"]
        local message = row["message"]

        local result = {}
        result["msgid"] = msgid
        result["groupname"] = groupName
        result["sendName"] = sendName
        result["message"] = message
        local value = json.encode(result)

        ops.SREM("groupChatMsg:"..groupName,value)
    end
end