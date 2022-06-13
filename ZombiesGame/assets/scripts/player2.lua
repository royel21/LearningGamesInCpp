local SPEED = 10
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

local bounds = Vec4f.new(0, 0, 1280 - size.x, 768 - size.y)
camera:setBounds(bounds);

function init()
    anim = entity:getAnimate()
    rbody = entity:getRigidBody()
    initPlayer(anim)

    if anim then
        anim:play(curAnime)
    end

    camera:setTarget(entity, vec)

    print("player2 init")
end

function destroy()
    assetManager:removeSound("bg")
end

local vel = {
    x = 0,
    y = 0
}

function move()
    rbody:applyForce(vel.x, vel.y)
    -- local trans = Player2:getTransform()
    -- print("move")
    -- trans.x = trans.x + vel.x
    -- trans.y = trans.y + vel.y
    -- print("move")
end

function roll()
    vel.x = vel.x * 1.1;
    vel.y = vel.y * 1.1;
    rbody:applyForce(vel.x, vel.y)
    -- rbody:setVelocity(vel.x, vel.y)
end

function jump()
    rbody:applyImpulse(0, 0.3)
end

function createEnt()
    local ent = scene:createEntity("bomb", true)

    local trans = ent:addTransform(5.0, 5.0, 32, 32)
    trans.layer = 5

    ent:addSprite()

    local script = ent:addScript("bomb.lua")
    script:setScript("bomb.lua")
    local id = ent:getId()

    initScript(id)

    print("create", anim, type(id))
end

function update(dt)
    -- local v = rbody:getVelocity()
    -- print(v.x)

    if not anim.loop then
        state = ""
        curAnime = stand[direction]
        anim:play(curAnime)
        vel = {
            x = 0,
            y = 0
        }
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
        createEnt()
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
            anim:play("jump-" .. direction)
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
