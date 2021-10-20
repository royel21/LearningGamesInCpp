local SPEED = 5.0;

print("player1 init")

local Player

function init() Player = scene:getEntity("Player") end

function update(dt)
    local trans = Player:getTransform();

    if input:onKeyDown("W") and trans.y < ScreenHeight - trans.h then
        trans.y = trans.y + SPEED;
    end
    if input:onKeyDown("S") and trans.y > 0 then trans.y = trans.y - SPEED; end
    if input:onKeyDown("D") and trans.x < ScreenWidth - trans.w then
        trans.x = trans.x + SPEED;
    end
    if input:onKeyDown("A") and trans.x > 0 then trans.x = trans.x - SPEED; end
end
