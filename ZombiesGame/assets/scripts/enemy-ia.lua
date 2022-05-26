local curAnime = "flight-down"

local SPEED = 7.5

local rbody

function init()
    local anim = Bat:getAnimate()
    if anim then anim:play(curAnime) end
    rbody = Bat:getRigidBody()
end

function move(vel) rbody:applyForce(vel.x, vel.y) end

function update(dt)
    local vel = {x = 0, y = 0}

    if input:onKeyDown("Up") then
        vel.y = SPEED
        direction = "up"
    end
    if input:onKeyDown("Down") then
        vel.y = -SPEED
        direction = "down"
    end
    if input:onKeyDown("Right") then
        vel.x = SPEED
        direction = "right"
    end
    if input:onKeyDown("Left") then
        vel.x = -SPEED
        direction = "left"
    end

    move(vel)
    Bat:getAnimate():play("flight-" .. direction)
end

function destroy() end

