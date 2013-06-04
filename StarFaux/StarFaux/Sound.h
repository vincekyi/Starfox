#ifndef _SOUND_H_
#define _SOUND_H_

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include "GL/glut.h"

#include "AL/al.h" 
#include "AL/alc.h" 
#include "AL/alut.h"

class Sound {
public:
	Sound(int numSounds, const char* baseDir);
	void loadSound(const char* filename);
	void playSound(const char* filename, float gain, int delay);

	//-- UNOBJECT ORIENTED STYLE --
	bool checkLoopTimer(const char* filename, float gain);
	void setAudioLength(time_t len);
	//-- UNOBJECT ORIENTED STYLE --

private:
	struct cmp_str {
		bool operator()(const char* a, const char* b) {
			return strcmp(a, b) < 0;
		}
	};

	char* m_baseDir;
	ALuint* m_soundBuffers;
	ALuint* m_sourceBuffers;
	int m_currBuffer;
	int m_numBuffers;
	int m_currSource;
	int m_currTime;

	std::map<char*, ALuint, cmp_str> m_soundSrc;

	const static int MAXSOURCES = 30;

	//-- UNOBJECT ORIENTED STYLE --
	int m_loopTimer;
	time_t m_audioLength;
	//-- UNOBJECT ORIENTED STYLE --
};

#endif
