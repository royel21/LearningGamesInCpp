local frametime = 120

function initPlayer(anim)

    anim:setTexture("player.png")

    anim:addSeq("stand-right", {0}, 0, true)
    anim:addSeq("stand-left", {12})
    anim:addSeq("stand-up", {6}, 0)
    anim:addSeq("stand-down", {18})

    anim:addSeq("run-right", {1, 2, 3, 4, 5}, frametime)
    anim:addSeq("run-left", {13, 14, 15, 16, 17}, frametime)
    anim:addSeq("run-up", {7, 8, 9, 10, 11}, frametime)
    anim:addSeq("run-down", {19, 20, 21, 22, 23}, frametime)
    print("initialize")
end

