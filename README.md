### Prototype Engine

This is a project I created using raylib, c++ and lua. It is not intended for professional use, as I created this just for fun.

## Functions for the lua embed:

* IsKeyDown(key) - Checks if the key is being pressed down.

* IsKeyPressed(key) - Checks if the key has been pressed.

* object() - Creates a new object with the properties: x, y, image (path), zIndex, visible (bool), opacity, id, width, height, and rotation.

* deleteObject(objectID) - Destroys the object using the object id. For example: deleteObject(object.id)

* clearObjects() - Clears all the objects that have been created.

* ui() - Creates a similar object to "object()", and has the same properties (with the addition of text, fontSize, and font color), but it is ALWAYS drawn on top of the objects, no matter the zIndex.

* deleteUI(uiID) - Exactly the same function as "deleteObject()" but for the UI.

* clearAllUI() - Exactly the same function as "clearObjects()" but for the UI.

## Globals:

Currently there is only one global that is useable, "player".

The player global has the properties of: x, y, zIndex, image, opacity, width, height, and rotation.

## Plans for the future:

* Clean everything up to make it look nicer.

* Add more control for lua.

* Map editor/creator

* 2D rigging for sprites.

* Shader Support