-- alldepart_redis.lua

local json = require("json")
local ops = require("redisOps")

local row = ops.rawRow()
local action = ops.rawAction()

if action == "insert" or action == "update"
then
    local id = row["id"]
    local departName = row["departname"]
    local departDesc = row["departdesc"]

    local result = {}
    result["id"] = id
    result["departName"] = departName
    result["departDesc"] = departDesc

    local key = "alldepart"
    local field = departName
    local value = json.encode(result)

    ops.HSET(key,field,value)
else
    if action == "delete"
    then
        local departName = row["departname"]

        local key = "alldepart"
        local field = departName
        local value = ""

        ops.HSET(key,field,value)
    end
end