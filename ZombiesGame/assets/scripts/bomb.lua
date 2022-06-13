local acumulator = 0

function init()
    local anim = entity:addAnimate()

    anim:setTexture("bombSheet.png")
    anim:addSeq("idle", {0}, 0, true)
    anim:addSeq("active", {0, 1}, 200)

    anim:setTexture("explosion.png")
    anim:addSeq("explosion", {0, 1, 2, 3, 4, 5}, 100)
end

function update(dt)
    local anim = entity:getAnimate()

    acumulator = acumulator + dt

    if acumulator > 1 and acumulator < 3 then
        anim:play("active")
    end

    if acumulator > 3 then
        local trans = entity:getTransform()
        if trans.layer ~= 99 then
            trans.layer = 99
            trans.x = trans.x - 16
            trans.y = trans.y - 16
            trans.w = 48
            trans.h = 48
        end

        anim:play("explosion")
    end
end

function animationEnd(id)
    if id == "explosion" then
        local center = entity:getCenter()

        local ents = queryWorld(center.x, center.y, 48, MASK.Enemy | MASK.Player)

        for i = 1, #ents do
            local rbody = ents[i]:getRigidBody()

            if rbody then
                local eCenter = ents[i]:getCenter()
                local dir = eCenter - center
                dir = dir:unit() * 3.0
                rbody:applyImpulse(dir.x, dir.y)
            end
        end
        scene:removeEntity(entity)
    end
end
