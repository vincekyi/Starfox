/*
#ifndef _SOUND_H_
#define _SOUND_H_

#include <stdlib.h>
#include <string.h>
#include <map>

#include "AL/al.h" 
#include "AL/alc.h" 
#include "AL/alut.h"

class Sound {
public:
	Sound(int numSounds, const char* baseDir);
	void loadSound(const char* filename);
	void playSound(const char* filename);
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

	std::map<char*, ALuint, cmp_str> m_soundSrc;

	const static int MAXSOURCES = 20;
};

#endif
*/
