-- allgroup_redis.lua

local json = require("json")
local ops = require("redisOps")

local row = ops.rawRow()
local action = ops.rawAction()

if action == "insert" or action == "update"
then
    local id = row["id"]
    local groupName = row["groupname"]
    local groupDesc = row["groupdesc"]

    local result = {}
    result["id"] = id
    result["groupName"] = groupName
    result["groupDesc"] = groupDesc

    local key = "allgroup"
    local field = groupName
    local value = json.encode(result)

    ops.HSET(key,field,value)
else
    if action == "delete"
    then
        local groupName = row["groupname"]
        
        local key = "allgroup"
        local field = groupName
        local value = ""

        ops.HSET(key,field,value)
    end
end
