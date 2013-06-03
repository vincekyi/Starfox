#include "Sound.h"

Sound::Sound(int numSounds, const char* baseDir) {
	if (baseDir[strlen(baseDir)-1] != '/') {
		m_baseDir = (char*)malloc(sizeof(char) * strlen(baseDir) + 2 * sizeof(char));
		strcpy(m_baseDir, baseDir);
		strcat(m_baseDir, "/");
	} else {
		m_baseDir = (char*)malloc(sizeof(char) * strlen(baseDir) + sizeof(char));
		strcpy(m_baseDir, baseDir);
	}

	//-- UNOBJECT ORIENTED STYLE --
	m_loopTimer = time(NULL);
	m_audioLength = 0;
	//-- UNOBJECT ORIENTED STYLE --

	m_currBuffer = 0;
	m_currSource = 0;
	m_numBuffers = numSounds;
	m_currTime = 0;
	m_soundBuffers = (ALuint*)malloc(sizeof(ALuint) * m_numBuffers);
	m_sourceBuffers = (ALuint*)malloc(sizeof(ALuint) * MAXSOURCES);

	alutInit(0, NULL);
	alGetError();

	alGenBuffers(m_numBuffers, m_soundBuffers);
	alGenSources(MAXSOURCES, m_sourceBuffers);

	ALfloat listenerPos[3] = {0.0, 0.0, 0.0};
	ALfloat listenerVel[3] = {0.0, 0.0, 0.0};
	ALfloat listenerOri[3] = {0.0, 0.0, 0.0};
	alListenerfv(AL_POSITION,listenerPos);
	alListenerfv(AL_VELOCITY,listenerVel);
	alListenerfv(AL_ORIENTATION,listenerOri);
}

void Sound::loadSound(const char* filename) {
	char* fn = (char*)malloc(sizeof(char) * strlen(filename) + sizeof(char) * strlen(m_baseDir) + sizeof(char));
	strcpy(fn, m_baseDir);
	strcat(fn, filename);

	ALenum     format;
	ALsizei    size;
	ALsizei    freq;
	ALboolean  loop;
	ALvoid*    data;
	alutLoadWAVFile(fn, &format, &data, &size, &freq, &loop);
	alBufferData(m_soundBuffers[m_currBuffer],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);

	m_soundSrc[fn] = m_currBuffer;

	m_currBuffer++;
}

void Sound::playSound(const char* filename, float gain, int delay) {
	if (m_currTime + delay > time(NULL)) {
		return;
	}

	char* fn = (char*)malloc(sizeof(char) * strlen(filename) + sizeof(char) * strlen(m_baseDir) + sizeof(char));
	strcpy(fn, m_baseDir);
	strcat(fn, filename);

	ALuint id = m_soundSrc.at(fn);

	ALfloat sourcePos[3] = {0.0, 0.0, 0.0};
	ALfloat sourceVel[3] = {0.0, 0.0, 0.0};
	ALfloat sourceOri[3] = {0.0, 0.0, 0.0};
	alSourcefv (m_sourceBuffers[m_currSource], AL_POSITION, sourcePos);
	alSourcefv (m_sourceBuffers[m_currSource], AL_VELOCITY, sourceVel);
	alSourcefv (m_sourceBuffers[m_currSource], AL_DIRECTION, sourceOri);

	alSourcef(m_sourceBuffers[m_currSource], AL_GAIN, gain);

	alSourcei(m_sourceBuffers[m_currSource], AL_BUFFER, m_soundBuffers[id]);

	alSourcePlay(m_sourceBuffers[m_currSource]);
	m_currSource++;
	m_currSource = (m_currSource % MAXSOURCES == 0) ? 0 : m_currSource;
	m_currTime = time(NULL);
}

//-- UNOBJECT ORIENTED STYLE --
bool Sound::checkLoopTimer(const char* filename, float gain) {
	time_t currTime = time(NULL);
	if (m_loopTimer + m_audioLength < currTime) {
		playSound(filename, gain, 1);
		m_loopTimer = currTime;

		return true;
	}
	return false;
}

void Sound::setAudioLength(time_t len) {
	m_audioLength = len;
}
//-- UNOBJECT ORIENTED STYLE --
