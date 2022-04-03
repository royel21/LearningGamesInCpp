local SPEED = 5.0;

print("player1 init")

local Player

function init() Player = scene:getEntity("Player") end

function update(dt)
    local trans = Player:getTransform()

    if input:onKeyDown("W") then trans.y = trans.y + SPEED end
    if input:onKeyDown("S") then trans.y = trans.y - SPEED end
    if input:onKeyDown("D") then trans.x = trans.x + SPEED end
    if input:onKeyDown("A") then trans.x = trans.x - SPEED end
end
