
print("bat init")

local SPEED = 5

local curAnime = "down"

function update(dt)
    local trans = Bat:getTransform();
    local anim = Bat:getAnimate();

    if input:onKeyDown("Up") then
        curAnime = "up";

        if trans.y > 0 then
            trans.y = trans.y - SPEED
        end
    end

    if input:onKeyDown("Down") then
        curAnime = "down";
        if trans.y < ScreenHeight-trans.h then
            trans.y = trans.y + SPEED
        end
    end

    if input:onKeyDown("Right") then
        curAnime = "right";
        if trans.x < ScreenWidth-trans.w then
            trans.x = trans.x + SPEED
        end
    end

    if input:onKeyDown("Left") then
        curAnime = "left";
        if trans.x > 0 == true then
            trans.x = trans.x - SPEED
        end
    end
    
    anim:play(curAnime)

end
