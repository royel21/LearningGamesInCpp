local SPEED = 10.0
local curAnime = "stand-r"

local stand = {
    right = "stand-right",
    left = "stand-left",
    up = "stand-up",
    down = "stand-down"
}

local sound = assetManager:addSound("bg", "assets/sounds/shotgun.wav")

local direction = "right";
local state = ""

local rbody
local anim

local size = camera:getVPSize();

local vec = Vec2f.new(-size.x / 2 + 32, -size.y / 2 + 32);

print(size.x, size.y)

camera:setBounds(Vec4f.new(0, 0, 1280 - size.x, 768 - size.y));

function init()
    anim = entity:getAnimate()
    rbody = entity:getRigidBody()
    initPlayer(anim)

    if anim then
        anim:play(curAnime)
    end

    camera:setTarget(entity, vec)
    local trans = entity:getTransform()
    print("player2 init", trans.layer)
end

function destroy()
    assetManager:removeSound("bg")
end

local vel = Vec2f.new(0, 0);

function move()
    vel = vel:unit() * SPEED
    rbody:applyForce(vel.x, vel.y)
end

function roll()
    vel = vel * 1.1;
    rbody:applyForce(vel.x, vel.y)
end

function jump()
    rbody:applyImpulse(0, 0.3)
end

function update(dt)

    if not anim.loop then
        state = ""
        curAnime = stand[direction]
        anim:play(curAnime)
        vel = Vec2f.new(0, 0);
    end

    if input:onKeyDown("A") then
        local pos = entity:getCenter()

        local ents = queryWorld(pos.x, pos.y, 25, MASK.Enemy, 10, direction)
        if #ents > 0 then
            -- print("found:", ents[1]:getCenter())
        end
    end

    -- if (input:onKeyDown("X")) then
    -- sound:play(false)
    --   print("play sound")
    -- end

    if input:onKeyPressed("T") then
        local dir = getDirection(entity:getCenter(), direction, 16)
        spawnBomb(dir.x, dir.y)
    end

    if input:onKeyDown("V") then
        SPEED = 20
    else
        SPEED = 10
    end

    if state ~= "attacking" and state ~= "jumping" then
        -- Move Up - Down
        if input:onKeyDown("Up") then
            direction = "up"
            state = "running"
            vel.y = SPEED
        elseif input:onKeyDown("Down") then
            direction = "down"
            state = "running"
            vel.y = -SPEED
        end
        -- Move Right - Left
        if input:onKeyDown("Right") then
            direction = "right"
            state = "running"
            vel.x = SPEED
        elseif input:onKeyDown("Left") then
            direction = "left"
            state = "running"
            vel.x = -SPEED
        end

        if input:onKeyPressed("C") then
            jump()
        end

        -- Jump animation
        if input:onKeyPressed("X") and state == "running" then
            state = "jumping"
            anim:play("roll-" .. direction)
            anim:setLoop(true)
            roll()
        elseif not anim.loop and state == "running" then
            -- Running animation
            anim:play("run-" .. direction)
        end
    end
    -- Jumping
    if state == "jumping" or state == "running" then
        move()
    end
    -- Attack Animation
    if input:onKeyPressed("Z") and state ~= "jumping" then
        anim:play("attack-" .. direction)
        anim:setLoop(true)
        state = "     "
    end
end

function animationEnd(id)
    print(id)
end
