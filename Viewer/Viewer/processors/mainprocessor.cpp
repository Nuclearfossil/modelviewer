
#include "mainprocessor.h"

#include "..\\fbxLoader.h"
#include "..\\utils\fileutils.h"

void MainProcessor(AppState& appState)
{
	while (appState.ShouldExit == false)
	{
		if (appState.OpenFile)
		{
			// open the file
			char filename[1024];
			if (OSOpenFile(filename))
			{
				OpenSceneFile(filename);
				appState.OpenFile = false;
				appState.Waiting = false;
			}
		}
	}
}
