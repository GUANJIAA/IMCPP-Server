-- friend_redis.lua

local json = require("json")
local ops = require("redisOps")

local row = ops.rawRow()
local action = ops.rawAction()

if action == "insert" or action == "update"
then
    local userName = row["username"]
    local friendName = row["friendname"]

    ops.SADD("friend:"..userName,friendName)
    ops.SADD("friend:"..friendName,userName)
else
    if action == "delete"
    then
        local userName = row["username"]
        local friendName = row["friendname"]

        ops.SREM("friend:"..userName,friendName)
        ops.SREM("friend:"..friendName,userName)
    end
end