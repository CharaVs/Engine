### Prototype engine

This is a project I created using raylib, c++ and lua. It is not intended for professional use, as I created this just for fun.

## Functions for the lua embed:

IsKeyDown(key) - Checks if the key is being pressed down. 
IsKeyPressed(key) - Checks if the key has been pressed.
object() - Creates a new object with the properties: x, y, image (path), zIndex, visible (bool), opacity, and id.
deleteObject(objectID) - Destroys the object using the object id. For example: deleteObject(object.id)
clearObjects() - Clears all the objects that have been created.
ui() - Creates a similar object to "object()", and has the same properties, but it is ALWAYS drawn on top of the objects, no matter the zIndex.
deleteUI(uiID) - Exactly the same function as "deleteObject()" but for the UI.
clearAllUI() - Exactly the same function as "clearObjects()" but for the UI.
