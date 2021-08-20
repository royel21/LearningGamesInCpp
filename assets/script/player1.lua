
local SPEED = 32;

function update(dt)
    local trans = Player:getTransform();
    
    if input:onKeyPressed("W") and trans.y > 0 == true then
        trans.y = trans.y - SPEED;
    end
    if input:onKeyPressed("S") and trans.y < SceenHeight-32 == true then
        trans.y = trans.y + SPEED;
    end
    if input:onKeyPressed("D") and trans.x < SceenWidth-32 == true then
        trans.x = trans.x + SPEED;
    end
    if input:onKeyPressed("A") and trans.x > 0 == true then
        trans.x = trans.x - SPEED;
    end
end
