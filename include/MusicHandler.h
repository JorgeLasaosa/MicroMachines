
// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>


#include <map>
#include <string>

class MusicHandler {
	public:
        static std::map<std::string, Mix_Music*> sounds;
		static void init ();
		static void play(std::string musicName, int times=-1);
		static void loadSound(const char* file, std::string name);
		static bool isPlaying();
};