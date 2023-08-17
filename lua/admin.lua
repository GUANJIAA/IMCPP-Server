-- admin_redis.lua

local json = require("json")
local ops = require("redisOps")

local row = ops.rawRow()
local action = ops.rawAction()

if action == "insert" or action == "update"
then
    local id = row["id"]
    local userName = row["name"]
    local password = row["pwd"]
    local status = row["status"]
    local email = row["email"]
    local phone = row["phone"]
    local desc = row["desc"]
    local depart = row["depart"]

    local result = {}
    result["id"] = id
    result["name"] = userName
    result["pwd"] = password
    result["status"] = status
    result["email"] = email
    result["phone"] = phone
    result["desc"] = desc
    result["depart"] = depart

    local key = "admin"
    local field = userName
    local value = json.encode(result)

    ops.HSET(key,field,value)
else
    if action == "delete"
    then
        local userName = row["name"]
        local key = "admin"
        local field = userName
        local value = ""
        ops.HSET(key,field,value)
    end
end