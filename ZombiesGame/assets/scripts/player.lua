local SPEED = 4

local rbody

local size = camera:getVPSize();
local direction = "right"

local currentAnim = "stand-right"
local state = "standing-"

local vel = {
    x = 0,
    y = 0
}

camera:setBounds(Vec4f.new(0, 0, 7680 - size.x, 5120 - size.y));

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
        vel.x = 0
        vel.y = 0
        state = "stand-"
    end

    if state ~= "attacking" and state ~= "jumping" then
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

        if vel.x ~= 0 or vel.y ~= 0 then
            state = "run-"
        end

        if state == "run-" then
            if input:onKeyPressed("X") then

            end
        end
    end

    if input:onKeyPressed("Z") and state ~= "jumping" then
        anim:play("attack-" .. direction)
        anim.loop = true
        local ents = queryWorld(trans.x, trans.y, 30, MASK.Enemy, 20, direction)
        print("found:", #ents)
    elseif anim.loop == false then
        local trans = entity:getTransform()
        trans.x = trans.x + vel.x
        trans.y = trans.y + vel.y
        anim:play(state .. direction)
    end
end




