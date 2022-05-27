local curAnime = "flight-down"

local SPEED = 7

local rbody
local entToFollow

print("enemy-ia")

function init()
    local anim = Bat:getAnimate()
    if anim then anim:play(curAnime) end
    rbody = Bat:getRigidBody()
end

function move(x, y) rbody:applyForce(x, y) end

function update(dt)

    if entToFollow ~= nil then
        local dir = entToFollow:getDirection(Bat);
        dir.x = dir.x * SPEED
        dir.y = dir.y * SPEED

        move(dir.x, dir.y)

        if dir.y > 0 then
            vel.y = SPEED
            direction = "up"
        end

        if dir.y < 0 then
            vel.y = -SPEED
            direction = "down"
        end

        if dir.x > 0 then
            vel.x = SPEED
            direction = "right"
        end
        if dir.y < 0 then
            vel.x = -SPEED
            direction = "left"
        end
    end
    Bat:getAnimate():play("flight-" .. direction)
end

function collisionStart(entId, isSensor)
    if isSensor then entToFollow = scene:getEntity(entId) end
end

function collisionEnd(entId, isSensor) if isSensor then entToFollow = nil end end

function destroy() end


