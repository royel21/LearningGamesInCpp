local SPEED = 1

local rbody

local size = camera:getVPSize();
local direction = "right"

local currentAnim = "stand-right"
local state = "standing-"

local vel = {
    x = 0,
    y = 0
}

camera:setBounds(Vec4f.new(0, 0, 4096 - size.x, 2048 - size.y));

local anim
local frametime = 120

function init()
    local size = camera:getVPSize();
    camera:setTarget(entity, Vec2f.new(-size.x / 2 + 32, -size.y / 2 + 32))
    anim = entity:getAnimate();
    initPlayer(anim);
end

function update(dt)

    if not anim.loop then
        vel = {
            x = 0,
            y = 0
        }
        state = "stand-"
    end

    if input:onKeyDown("Up") then
        direction = "up"
        vel.y = SPEED
    elseif input:onKeyDown("Down") then
        direction = "down"
        vel.y = -SPEED
    end
    -- Move Right - Left
    if input:onKeyDown("Right") then
        direction = "right"
        vel.x = SPEED
    elseif input:onKeyDown("Left") then
        direction = "left"
        vel.x = -SPEED
    end

    local trans = entity:getTransform()
    trans.x = trans.x + vel.x
    trans.y = trans.y + vel.y

    if vel.x ~= 0 or vel.y ~= 0 then
        state = "run-"
    end
    -- print(state .. direction)
    anim:play(state .. direction)
end

function destroy()
end

