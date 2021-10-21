print("bat init", scene)

local SPEED = 5

local curAnime = "down"
local Player
local Bat

function init()
    Bat = scene:getEntity("Bat")
    player = scene:getEntity("Player")
    local anim = Bat:getAnimate();

    if anim then
        anim:setTexture("bats");
        -- -- Standing
        anim:addSeq("up", {0, 1, 2}, 120)
        anim:addSeq("left", {3, 4, 5}, 120)
        anim:addSeq("down", {6, 7, 8}, 120)
        anim:addSeq("right", {9, 10, 11}, 120)
    end
    anim:play(curAnime)
end

function update(dt)
    local playerPos = player:getTransform()

    local trans = Bat:getTransform()
    trans.x = playerPos.x - 64.0
    trans.y = playerPos.y + 64.0
    local anim = Bat:getAnimate()

    if input:onKeyDown("Up") then anim:play("up") end
    if input:onKeyDown("Down") then anim:play("down") end
    if input:onKeyDown("Right") then anim:play("right") end
    if input:onKeyDown("Left") then anim:play("left") end
end
