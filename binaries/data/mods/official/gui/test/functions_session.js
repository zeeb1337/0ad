function initSession()
{
	initGroupPane();
	initResourcePool();
	initStatusOrb();
	initMapOrb();
	initTeamTray();
	initSubWindows();
}

// ====================================================================

function startLoadingScreen()
{
	// Setup loading screen.

	// Switch screens from main menu to loading screen.
	GUIObjectHide("pregame_gui");
	GUIObjectUnhide("loading_screen");
	console.write("Loading " + g_GameAttributes.mapFile + " (" + g_GameAttributes.numPlayers + " players) ...");

	getGUIObjectByName("loading_screen_titlebar-text").caption = "Loading Scenario ...";
	getGUIObjectByName("loading_screen_progress_bar_text").caption = "... Reticulating splines ...";
	getGUIObjectByName("loading_screen_progress_bar").caption = 80;
	getGUIObjectByName("loading_screen_text").caption = "LOADING " + g_GameAttributes.mapFile + " ...\nPlease wait ...\n(Yes, we know the progress bar doesn't do diddly squat right now)\nJust keep waiting ...\nIt'll get there ...\nAlmost done ...\nTrust me!";
	getGUIObjectByName("loading_screen_tip").caption = "Wise man once say ...\nHe who thinks slow, he act in haste, be rash and quick and foolish. But he that thinks too much, acts too slowly. The stupid always win, Commandersan. Remember that. You are tiny grasshopper.";

	// Begin game session.
	setTimeout( loadSession(), 0 );
}

// ====================================================================

function loadSession()
{
	startGame();
	GUIObjectHide("loading_screen");
	GUIObjectUnhide("session_gui");
	FlipGUI(GUIType);

	// Select session peace track.
	curr_session_playlist_1 = newRandomSound("music", "peace");
	// Fade out main theme and fade in session theme.
	CrossFade(curr_music, curr_session_playlist_1, 0.0001);
}

// ====================================================================

function setPortrait(objectName, portraitString) 
{
	// Use this function as a shortcut to change a portrait object to a different portrait image. 

	// Accepts an object and specifies its default, rollover (lit) and disabled (grey) sprites.
	// Sprite Format: "ui_portrait_"portraitString"_64"
	// Sprite Format: "ui_portrait_"portraitString"_64""-lit"
	// Sprite Format: "ui_portrait_"portraitString"_64""-grey"
	// Note: Make sure the file follows this naming convention or bad things could happen.

        // Get GUI object
        GUIObject = getGUIObjectByName(objectName);

	// Set the three portraits.
	GUIObject.sprite = "ui_portrait_" + portraitString + "_64";
	// Note we need to use a special syntax here (object["param"] instead of object.param because dashes aren't actually in JS's variable-naming conventions.
	GUIObject["sprite-over"] = GUIObject.sprite + "-lit";
	GUIObject["sprite-disabled"] = GUIObject.sprite + "-grey";
}

// ====================================================================

function getObjectInfo() 
{
	// Updated each tick to extract entity information from selected unit(s).

	// Don't process GUI when we're full-screen.
	if (GUIType != "none")
	{
		if (!selection.length) 	// If no entity selected,
		{
			// Hide Status Orb
			getGUIObjectByName("session_status_orb").hidden = true;

			// Hide Group Pane.
			getGUIObjectByName("session_group_pane").hidden = true;

			getGlobal().MultipleEntitiesSelected = 0;
		}
		else			// If at least one entity selected,
		{
			// Store globals for entity information.
//			strString = "" + selection[0].position;
//			EntityPos = strString.substring(20,strString.length-3);

			UpdateStatusOrb(); // (later, we need to base this on the selected unit's stats changing)

		        // Check if a group of entities selected
		        if (selection.length > 1) 
			{
				// If a group pane isn't already open, and we don't have the same set as last time,
				// NOTE: This "if" is an optimisation because the game crawls if this set of processing occurs every frame.
				// It's quite possible for the player to select another group of the same size and for it to not be recognised.
				// Best solution would be to base this off a "new entities selected" instead of an on-tick.
				if (
					// getGUIObjectByName("session_group_pane").hidden == true || 
					selection.length != getGlobal().MultipleEntitiesSelected)
				{
					UpdateGroupPane(); // (later, we need to base this on the selection changing)
			                getGlobal().MultipleEntitiesSelected = selection.length;
				}
		        } 
			else
			{
		                getGlobal().MultipleEntitiesSelected = 0;

				// Hide Group Pane.
				getGUIObjectByName("session_group_pane").hidden = true;
			}
	        }

		// Modify any resources given/taken (later, we need to base this on a resource-changing event).
		UpdateResourcePool();

		// Update Team Tray (later, we need to base this on the player creating a group).
		UpdateTeamTray();
	}
}

// ====================================================================

function MakeUnit(x, y, z, MakeUnitName)
{
	// Spawn an entity at the given coordinates.

	DudeSpawnPoint = new Vector3D(x, y, z);
	new Entity(getEntityTemplate(MakeUnitName), DudeSpawnPoint, 1.0);
	// writeConsole(MakeUnitName + " created at " + DudeSpawnPoint);
}

// ====================================================================

function selected()
{
	// Returns how many units selected.

	if( selection.length > 0 )
		return( selection[0] );
	return( null );
}
