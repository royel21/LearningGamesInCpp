print("bat init")

local SPEED = 5

local curAnime = "down"

function init()
    local anim = Bat:getAnimate();

    if anim then
        -- -- Standing
        anim:addSeq("up", {0, 1, 2}, 0, 120)
        anim:addSeq("left", {3, 4, 5}, 0, 120)
        anim:addSeq("down", {6, 7, 8}, 0, 120)
        anim:addSeq("right", {9, 10, 11}, 0, 120)
    end
    anim:play(curAnime)
end

player = scene:getEntity("Player")

-- print("scene", );

function update(dt)
    local playerPos = player:getTransform();
    -- print("pos: ", playerPos.x, playerPos.y)
    local trans = Bat:getTransform();
    trans.x = playerPos.x - 64.0
    trans.y = playerPos.y + 64.0
    -- local anim = Bat:getAnimate();

    -- if input:onKeyDown("Up") then
    --     anim:play("up")
    --     if trans.y < ScreenHeight - trans.h then
    --         trans.y = trans.y + SPEED
    --     end
    -- end

    -- if input:onKeyDown("Down") then
    --     anim:play("down")
    --     if trans.y > 0 then trans.y = trans.y - SPEED end
    -- end

    -- if input:onKeyDown("Right") then
    --     anim:play("right")
    --     if trans.x < ScreenWidth - trans.w then trans.x = trans.x + SPEED end
    -- end

    -- if input:onKeyDown("Left") then
    --     anim:play("left")
    --     if trans.x > 0 == true then trans.x = trans.x - SPEED end
    -- end

end
