local SPEED = 4

local curAnime = "stand-r"

local vel = 0;

local Player2

local stand = {
    right = "stand-right",
    left = "stand-left",
    up = "stand-up",
    down = "stand-down"
}

local direction = "right";
local state = ""

function init()
    Player2 = scene:getEntity("Player2")
    local anim = Player2:getAnimate();

    if anim then
        -- -- Standing
        -- local seq = Sequence.new({0}, 0, 0)
        anim:addSeq("stand-right", {0}, 0, 0)
        anim:addSeq("stand-up", {6}, 0, 0)
        anim:addSeq("stand-left", {12}, 0, 0)
        anim:addSeq("stand-down", {18}, 0, 0)
        -- -- Running
        anim:addSeq("run-right", {1, 2, 3, 4, 5}, 0, 100)
        anim:addSeq("run-up", {7, 8, 9, 10, 11}, 0, 100)
        anim:addSeq("run-left", {13, 14, 15, 16, 17}, 0, 100)
        anim:addSeq("run-down", {19, 20, 21, 22, 23}, 0, 100)
        -- Attacking
        anim:addSeq("attack-right", {24, 25, 26, 27}, 0, 65)
        anim:addSeq("attack-up", {28, 29, 30, 31}, 0, 65)
        anim:addSeq("attack-left", {32, 33, 34, 35}, 0, 65)
        anim:addSeq("attack-down", {36, 37, 38, 39}, 0, 65)
        -- Jumpin
        anim:addSeq("jump-up", {45, 46, 47, 48, 49}, 0, 90)
        anim:addSeq("jump-down", {55, 56, 57, 58, 59}, 0, 90)
        anim:addSeq("jump-right", {40, 41, 42, 43, 44}, 0, 90)
        anim:addSeq("jump-left", {50, 51, 52, 53, 54}, 0, 90)
        -- print("seq", seq)
    end
    anim:play(curAnime)
end

local vel = {x = 0, y = 0}

function move()
    local trans = Player2:getTransform();
    trans.x = trans.x + vel.x
    trans.y = trans.y + vel.y
end

function update(dt)
    local anim = Player2:getAnimate();

    if not anim.loop then
        state = ""
        curAnime = stand[direction]
        anim:play(curAnime)
        vel = {x = 0, y = 0}
    end

    if state ~= "attacking" and state ~= "jumping" then
        -- Move Up - Down
        if input:onKeyDown("Up") then
            direction = "up"
            state = "running"
            vel.y = SPEED
        elseif input:onKeyDown("Down") then
            direction = "down"
            state = "running"
            vel.y = -SPEED
        end
        -- Move Right - Left
        if input:onKeyDown("Right") then
            direction = "right"
            state = "running"
            vel.x = SPEED
        elseif input:onKeyDown("Left") then
            direction = "left"
            state = "running"
            vel.x = -SPEED
        end
        -- Jump animation
        if input:onKeyPressed("X") and state == "running" then
            state = "jumping"
            vel.x = vel.x * 1.8;
            vel.y = vel.y * 1.8;
            anim:play("jump-" .. direction);
            anim:setLoop(true);
        elseif not anim.loop and state == "running" then
            -- Running animation
            anim:play("run-" .. direction)
        end
    end
    -- Jumping
    if state == "jumping" or state == "running" then move() end
    -- Attack Animation
    if input:onKeyPressed("Z") and state ~= "jumping" then
        anim:play("attack-" .. direction);
        anim:setLoop(true);
        state = "attacking"
    end
end
