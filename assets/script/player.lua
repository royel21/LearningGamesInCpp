local rbody

function init()
    rbody = player:getRigidBody()
    rbody:setMaxVelocity(0.7)
end

function update(dt)
    local vel = {x = 0, y = 0}

    if input:onKeyDown("Right") then vel.x = 1.5 end

    if input:onKeyDown("Left") then vel.x = -1.5 end

    if input:onKeyPressed("X") then rbody:applyImpulse(0, 0.35) end

    if vel.x ~= 0 then rbody:applyForce(vel.x, vel.y) end
end

function destroy() end
