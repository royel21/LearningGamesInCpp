local SPEED = 32

local rbody

local size = camera:getVPSize();
camera:setBounds( Vec4f.new(0, 0, 4096 - size.x, 2048 - size.y));

function init()
	local size = camera:getVPSize();
	camera:setTarget(Player, Vec2f.new(-size.x / 2 + 32, -size.y / 2 + 32))
    rbody = Player:getRigidBody()
 	rbody:setMaxVelocity(1, 2)
	print("Player")
end

function update(dt)
    local vel = {x = 0, y = 0}

    if input:onKeyDown("Up") then
        vel.y = SPEED
    elseif input:onKeyDown("Down") then
        vel.y = -SPEED
    end
    -- Move Right - Left
    if input:onKeyDown("Right") then
        vel.x = SPEED
    elseif input:onKeyDown("Left") then
        vel.x = -SPEED
    end

    rbody:applyForce(vel.x, vel.y)
end

function destroy() end























