local curAnime = "flight-down"

local SPEED = 7

local rbody
local trans
local entToFollow

print("enemy-ia")

function init()
    local anim = Bat:getAnimate()
    if anim then anim:play(curAnime) end
    rbody = Bat:getRigidBody()
	trans=Bat:getTransform()
end

function move(x, y) rbody:applyForce(x, y) end

function update(dt)

    if entToFollow ~= nil and entToFollow:getDistance(Bat) > trans.w then
        local dir = entToFollow:getDirection(Bat)
        dir.x = dir.x * SPEED
        dir.y = dir.y * SPEED
		
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
    Bat:getAnimate():play("flight-" .. direction)
end

function collisionStart(entId, isSensor)
   if isSensor then entToFollow = scene:getEntity(entId) end
end

function collisionEnd(entId, isSensor) 
	if isSensor then
		print(scene:getEntity(entId):getName(), isSensor) 
		entToFollow = nil 
	end 
end

function destroy() end

















