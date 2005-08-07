/*
	DESCRIPTION	: Utility functions that manipulate GUI objects.
	NOTES		: 
*/

// ====================================================================

// Hide GUI object.
function guiHide (objectName) 
{
        var guiObject = getGUIObjectByName (objectName);
	if (guiObject == "" || guiObject == null || guiObject == undefined || guiObject == "undefined")
		console.write ("GUI Object not found: " + objectName);
	guiObject.hidden = true;
}

// ====================================================================

// Reveal GUI object.
function guiUnHide (objectName)
{
        var guiObject = getGUIObjectByName (objectName);
	if (guiObject == "" || guiObject == null || guiObject == undefined || guiObject == "undefined")
		console.write ("GUI Object not found: " + objectName);
        guiObject.hidden = false;

}

// ====================================================================

// Toggle visibility of GUI object.
function guiToggle (objectName)
{
        // Get our GUI object
        var guiObject = getGUIObjectByName (objectName);

	if (guiObject == "" || guiObject == null || guiObject == undefined || guiObject == "undefined")
		console.write ("GUI Object not found: " + objectName);

        // Toggle it
        guiObject.hidden = !guiObject.hidden;
}

// ====================================================================

// Enable GUI object.
function guiEnable (objectName) 
{
        var guiObject = getGUIObjectByName (objectName);
	if (guiObject == "" || guiObject == null || guiObject == undefined || guiObject == "undefined")
		console.write ("GUI Object not found: " + objectName);
	guiObject.enabled = true;
}

// ====================================================================

// Disable GUI object.
function guiDisable (objectName) 
{
        var guiObject = getGUIObjectByName (objectName);
	if (guiObject == "" || guiObject == null || guiObject == undefined || guiObject == "undefined")
		console.write ("GUI Object not found: " + objectName);
	guiObject.enabled = false;
}

// ====================================================================

// Change caption of GUI object and then make it visible.
function guiRenameAndReveal (objectName, objectCaption)
{
        // Get our GUI object
        var guiObject = getGUIObjectByName (objectName);

	if (guiObject == "" || guiObject == null || guiObject == undefined || guiObject == "undefined")
		console.write ("GUI Object not found: " + objectName);

	// Rename it
	guiObject.caption = objectCaption;

        // Toggle it
        guiObject.hidden = false;
}

// ====================================================================

// Increment/decrement the numeric caption of a GUI object.
function guiModifyCaption (objectName, objectModifier, objectPlaces)
{
	// Adds a modifier to a GUI object's caption (eg increase a number by 1).
	// Obviously don't use this unless you're sure the caption contains a number.
	// objectPlaces specifies the number of decimal places to use for a floating point number.
	// If not specified, it defaults to zero (whole number).

        // Get our GUI object
        var guiObject = getGUIObjectByName (objectName);

	if (guiObject == "" || guiObject == null || guiObject == undefined || guiObject == "undefined")
		console.write ("GUI Object not found: " + objectName);

	if (!objectPlaces)
		objectPlaces = 0;
	objectPlaces = Math.pow (10, objectPlaces);

	guiObject.caption = 	(Math.round (objectPlaces * guiObject.caption)
				+ Math.round (objectPlaces * objectModifier) ) / objectPlaces;
}

// ====================================================================

// Set caption of a GUI object.
function guiSetCaption (objectName, objectCaption)
{
	// Sets an object's caption to the specified value.

        // Get our GUI object
        var guiObject = getGUIObjectByName (objectName);

	if (guiObject == "" || guiObject == null || guiObject == undefined || guiObject == "undefined")
		console.write ("GUI Object not found: " + objectName);

	guiObject.caption = objectCaption;
}

// ====================================================================

// Helper function that closes a given window (usually the current parent) and opens another one.
function guiSwitch (closeWindowName, openWindowName)
{
	guiHide		(closeWindowName);
	guiUnHide	(openWindowName);
}

// ====================================================================