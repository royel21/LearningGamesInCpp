local SPEED = 32;

print("player1 init")

function update(dt)
    local trans = Player:getTransform();

    if input:onKeyPressed("W") and trans.y < ScreenHeight - trans.h then
        trans.y = trans.y + SPEED;
    end
    if input:onKeyPressed("S") and trans.y > 0 then
        trans.y = trans.y - SPEED;
    end
    if input:onKeyPressed("D") and trans.x < ScreenWidth - trans.w then
        trans.x = trans.x + SPEED;
    end
    if input:onKeyPressed("A") and trans.x > 0 then
        trans.x = trans.x - SPEED;
    end
end
