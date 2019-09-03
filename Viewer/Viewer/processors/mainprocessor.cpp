
#include "mainprocessor.h"

#include "..\fbxLoader.h"
#include "..\utils\fileutils.h"
#include "..\gui\applog.h"

void MainProcessor(AppState& appState)
{
	gAppLog.Add("Main Processor Thread Started\n");
	while (appState.ShouldExit == false)
	{
		if (appState.OpenFile)
		{
			// open the file
			char filename[1024];
			if (OSOpenFile(filename))
			{
				gAppLog.Add("Opening File: %s\n", filename);
				OpenSceneFile(filename);
				appState.OpenFile = false;
				appState.Waiting = false;
				gAppLog.Add("File %s Open\n", filename);
			}
		}
	}
}
