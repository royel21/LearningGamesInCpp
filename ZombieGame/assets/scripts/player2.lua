local SPEED = 0.3
local curAnime = "stand-right"

local stand = {
    right = "stand-right",
    left = "stand-left",
    up = "stand-up",
    down = "stand-down"
}

 print("player2 loaded")

local sound = assetManager:addSound("shotgun", "assets/sounds/shotgun.wav")
 print("sound loaded")

local direction = "right";
local state = ""

local rbody

function init()
    local anim = Player2:getAnimate()
    if anim then anim:play(curAnime) end
    rbody = Player2:getRigidBody()
    rbody:setMaxVelocity(1,2)

    print("player2 init")
end

function destroy() assetManager:removeSound("bg") end

local vel = {x = 0, y = 0}

function move() rbody:applyForce(vel.x, vel.y) end

function roll()
    vel.x = vel.x * 1.1;
    vel.y = vel.y * 1.1;
    rbody:applyForce(vel.x, vel.y)
end

function jump() rbody:applyImpulse(0, 0.3) end

function update(dt)
    local anim = Player2:getAnimate()
    -- local v = rbody:getVelocity()
    -- print(v.x)

    if not anim.loop then
        state = ""
        curAnime = stand[direction]
        anim:play(curAnime)
        vel = {x = 0, y = 0}
    end

     if (input:onKeyDown("X")) then
		print("play sound")
     	sound:play(false)
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























