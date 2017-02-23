
#include "MusicHandler.h"

MusicHandler::MusicHandler()
{
	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	{
		// ERROR
	}
	Mix_AllocateChannels(16);

	popcorn = Mix_LoadMUS("sounds/popcorn.mid");
	//popcorn = Mix_LoadMUS("sounds/aria.mp3");
	if (popcorn==NULL) {
	 	printf( "UPS\n");
	}

}


void MusicHandler::play(const char* musicName) {
	Mix_PlayMusic(popcorn,-1);
}