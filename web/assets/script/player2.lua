local SPEED = 1.5
local curAnime = "stand-r"

local vel = 0;

local stand = {
    right = "stand-right",
    left = "stand-left",
    up = "stand-up",
    down = "stand-down"
}

local direction = "right";
local state = ""

function init()
    local anim = Player2:getAnimate()
    if anim then anim:play(curAnime) end

    print("player2 init")
end

local vel = {x = 0, y = 0}

function move()
    local rbody = Player2:getRigidBody()
    rbody:setVelocity(vel.x, vel.y)
end

function roll()
    local rbody = Player2:getRigidBody()
    vel.x = vel.x * 1.1;
    vel.y = vel.y * 1.1;
    rbody:applyForce(vel.x, vel.y)
end

function jump()
    local rbody = Player2:getRigidBody()
    rbody:applyImpulse(0, 2)
end

function update(dt)
    local anim = Player2:getAnimate()

    if not anim.loop then
        state = ""
        curAnime = stand[direction]
        anim:play(curAnime)
        vel = {x = 0, y = 0}
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

        if input:onKeyPressed("C") then jump() end

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
    if state == "jumping" or state == "running" then move() end
    -- Attack Animation
    if input:onKeyPressed("Z") and state ~= "jumping" then
        anim:play("attack-" .. direction)
        anim:setLoop(true)
        state = "     "
    end
end
