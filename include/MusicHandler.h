
// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
// #include <SDL/SDL.h>
// #include <SDL/SDL_image.h>
// #include <SDL/SDL_mixer.h>
// #include <stdio.h>
// #include <string>


#include <map>
#include <string>

class MusicHandler {
	private:
		// //The music that will be played
		Mix_Music *popcorn = NULL;
		// //The sound effects that will be used
		// Mix_Chunk *gScratch = NULL;
		// Mix_Chunk *gHigh = NULL;
		// Mix_Chunk *gMedium = NULL;
		// Mix_Chunk *gLow = NULL;
	public:
        static std::map<std::string, Mix_Music*> shaders;
		MusicHandler ();
		void play(const char* musicName);
};