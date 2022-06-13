local timer = 1000
local acumulator = 0

local anim

function init()
    anim = entity:addAnimate()

    anim:setTexture("bombSheet.png")
    anim:addSeq("idle", {0}, 0, true)
    anim:addSeq("active", {0, 1}, 200)

    anim:setTexture("explosion.png")
    anim:addSeq("explosion", {0, 1, 2, 3, 4, 5}, 100)
    print("init", entity:getId())

end

function update(dt)

    acumulator = acumulator + dt

    if acumulator > 1 and acumulator < 3 then
        anim:play("active", true)
    end

    if acumulator > 3 then
        anim:play("explosion")
        anim.loop = false
    end
    print("Id", entity:getId(), acumulator)

end

function animationEnd(id)
    if id == "explosion" then
        print(id, entity:getId())
        scene:removeEntity(entity)
    end
end
