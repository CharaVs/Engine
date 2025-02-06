local rabbitPic = "resources/sayori.png"
local currentScene = "MainScene"
local clearedScene = true
local createdScene = false

local scenes = {}

local function clearScreen()
    clearObjects()
    clearUI()
    clearedScene = true
end

local function changeScene(sceneName)
    currentScene = sceneName
    clearScreen()
    createdScene = false
end


-- START OF MAIN MENU

local mainMenuButtons = {}
local function MainMenu()
    if not createdScene then
        -- Create all the required objects here
        local undertaleButton = ui()
        undertaleButton.image = "resources/undertaleButton.png"
        undertaleButton.width = 128;
        print(undertaleButton.widthCO)
        undertaleButton.height = 128;
        undertaleButton.x = 0
        undertaleButton.y = 0
        table.insert(mainMenuButtons, undertaleButton)
        table.insert(mainMenuButtons, "undertaleButton")
        createdScene = true
    end
    local undertaleButton = mainMenuButtons[1]
    local currentButton = mainMenuButtons[2]
    undertaleButton.fontColor = {r = 0, g = 255, b = 0, a = 255}
    undertaleButton.text = "H"
    if IsKeyDown("KEY_W") then
        undertaleButton.width = undertaleButton.width + 2
        print(undertaleButton.widthCO)
    end
    if IsKeyDown("KEY_S") then
        undertaleButton.width = undertaleButton.width - 2
        print(undertaleButton.widthCO)
    end
end
scenes["MainScene"] = MainMenu
-- END OF MAIN MENU

local velocity = 0

function update(deltaTime)
    if not clearedScene then
        clearScreen()
    end
    scenes[currentScene]()
end