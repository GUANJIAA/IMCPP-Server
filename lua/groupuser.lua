-- groupuser_redis.lua

local json = require("json")
local ops = require("redisOps")

local row = ops.rawRow()
local action = ops.rawAction()

if action == "insert" or action == "update"
then
    local groupName = row["groupname"]
    local userName = row["username"]
    local userRole = row["userrole"]

    ops.SADD("groups_"..userName,groupName)

    local result = {}
    result["userName"] = userName
    result["userRole"] = userRole
    local value = json.encode(result)
    ops.SADD("groupUsers_"..groupName,value)
else
    if action == "delete"
    then
        local groupName = row["groupname"]
        local userName = row["username"]
        local userRole = row["userrole"]

        ops.SREM("groups_"..userName,groupName)

        local result = {}
        result["userName"] = userName
        result["userRole"] = userRole
        local value = json.encode(result)
        ops.SREM("groupUsers_"..groupName,value)
    end
end