local curAnime = "flight-down"

local SPEED = 7

local rbody
local trans
local anim
local entToFollow
local player

print("enemy-ia")

function init()
    anim = entity:getAnimate()
    if anim then
        anim:play(curAnime)
    end
    rbody = entity:getRigidBody()
    trans = entity:getTransform()
end

function move(x, y)
    rbody:applyForce(x, y)
end

function update(dt)
    if player then
        local center = entity:getCenter()
        local playerCent = player:getCenter()
        local ent = castRay(center, playerCent, MASK.Walls | MASK.Player)

        if ent == player then
            local dis = playerCent - center

            if dis:length() > trans.w then
                local dir = dis:unit()
                dir = dir * SPEED

                move(dir.x, dir.y)

                local stateH
                local stateV

                if dir.y > 0 then
                    stateV = "up"
                elseif dir.y < 0 then
                    stateV = "down"
                end

                if dir.x > 0 then
                    stateH = "right"
                elseif dir.x < 0 then
                    stateH = "left"
                end

                if math.abs(dir.x) < math.abs(dir.y) then
                    direction = stateV
                else
                    direction = stateH
                end
            end
            anim:play("flight-" .. direction)
        end
    end
end

function collisionStart(entId, isSensor)
    if isSensor then
        player = scene:getEntity(entId)
    end
end

function collisionEnd(entId, isSensor)
    if isSensor then
        player = nil
    end
end

function destroy()
end

