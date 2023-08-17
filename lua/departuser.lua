-- departuser_redis.lua

local json = require("json")
local ops = require("redisOps")

local row = ops.rawRow()
local action = ops.rawAction()

if action == "insert" or action == "update"
then
    local departName = row["departname"]
    local userName = row["username"]
    local userRole = row["userrole"]

    local key = "departUsers_"..departName
    local result = {}
    result["userName"] = userName
    result["userRole"] = userRole
    local value = json.encode(result)
    ops.SADD(key,value)
else
    if action == "delete"
    then
        local departName = row["departname"]
        local userName = row["username"]
        local userRole = row["userrole"]

        local key = "departUsers_"..departName
        local result = {}
        result["userName"] = userName
        result["userRole"] = userRole
        local value = json.encode(result)
        ops.SREM(key,value)
    end
end