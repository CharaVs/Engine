local rabbitPic = "resources/sayori.png"
local currentScene = "MainScene"
local newObj = object()
newObj.image = rabbitPic
newObj.visible = true
local newObjID = newObj.id

local velocity = 0

function update(deltaTime)
    velocity = velocity + 0.1
    player.y = player.y + velocity
    if player.y > 10 then
        player.y = 10
    end
    if IsKeyPressed("KEY_S") then
        delete_object(newObjID)
    end
end