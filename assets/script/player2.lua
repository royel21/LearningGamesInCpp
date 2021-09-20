
print("player2 init")

local SPEED = 4

local curAnime = "stand-r"

local vel = 0;

local stand = {}

stand["right"] = "stand-r"
stand["left"] = "stand-l"
stand["up"] = "stand-u"
stand["down"] = "stand-d"

local direction = "right";
local state = ""
local loop = false;

function update(dt)
    local trans = Player2:getTransform();
    local anim = Player2:getAnimate();
    
    
    if not anim.loop then
        state = ""
        curAnime = stand[direction]
        anim:play(curAnime)
    end

    if state ~= "attacking" then
        if input:onKeyDown("Up") then
            direction = "up"
            state = "running"
            if trans.y > -32 then
                trans.y = trans.y - SPEED
            end
        elseif input:onKeyDown("Down") then
            direction = "down"
            state = "running"
            if trans.y < ScreenHeight-96 then
                trans.y = trans.y + SPEED
            end
        end

        if input:onKeyDown("Right") then
            direction = "right"
            state = "running"
            if trans.x < ScreenWidth-85 then
                trans.x = trans.x + SPEED
            end
        elseif input:onKeyDown("Left") then
            direction = "left"
            state = "running"
            if trans.x > -32 then
                trans.x = trans.x - SPEED
            end
        end

        if input:onKeyPressed("X") then
            if direction == "right" then
                curAnime = "jump-right"
            elseif direction == "left" then
                curAnime = "jump-left"
            elseif direction == "up" then
                curAnime = "jump-up"
            else
                curAnime = "jump-down"
            end
            state = "jumping"
            anim:play(curAnime);
            anim:setLoop(true);
        elseif not anim.loop and state == "running" then
            if direction == "right" then
                curAnime = "run-right"
            elseif direction == "left" then
                curAnime = "run-left"
            elseif direction == "up" then
                curAnime = "run-up"
            elseif direction == "down" then
                curAnime = "run-down"
            end
            anim:play(curAnime)
        end
        
    end

    if input:onKeyPressed("Z") and state ~= "jumping" then
        if direction == "right" then
            curAnime = "attack-right"
        elseif direction == "left" then
            curAnime = "attack-left"
        elseif direction == "up" then
            curAnime = "attack-up"
        else
            curAnime = "attack-down"
        end
        anim:play(curAnime);
        anim:setLoop(true);
        state = "attacking"
    end

    print(state, curAnime)
end
