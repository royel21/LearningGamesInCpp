MASK = {
    Walls = 0x01, -- 1
    --  10000000
    --  00000000
    Sensor = 0x02, -- 2
    --  01000000
    --  00000000
    Enemy = 0x80, -- 128
    --  00000001
    --  00000000
    Player = 0x100 -- 256
    --  00000000
    --  10000000
}

local frametime = 120

function initPlayer(anim)

    anim:setTexture("player.png")

    anim:addSeq("stand-right", {0})
    anim:addSeq("stand-left", {12})
    anim:addSeq("stand-up", {6})
    anim:addSeq("stand-down", {18})

    anim:addSeq("run-right", {1, 2, 3, 4, 5}, frametime)
    anim:addSeq("run-left", {13, 14, 15, 16, 17}, frametime)
    anim:addSeq("run-up", {7, 8, 9, 10, 11}, frametime)
    anim:addSeq("run-down", {19, 20, 21, 22, 23}, frametime)

    anim:addSeq("attack-right", {24, 25, 26, 27}, frametime)
    anim:addSeq("attack-left", {32, 33, 34, 35}, frametime)
    anim:addSeq("attack-up", {28, 29, 30, 31}, frametime)
    anim:addSeq("attack-down", {36, 37, 38, 39}, frametime)

    anim:addSeq("roll-right", {40, 41, 42, 43, 44}, frametime)
    anim:addSeq("roll-left", {50, 51, 52, 53, 54}, frametime)
    anim:addSeq("roll-up", {45, 46, 47, 48, 49}, frametime)
    anim:addSeq("roll-down", {55, 56, 57, 58, 59}, frametime)
end

