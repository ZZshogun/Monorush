#ifndef AUDIO_CLASS_H
#define AUDIO_CLASS_H

#include "Core.h"

class AudioSource {
	AudioSource();

public:
	static ALuint Create();

	static void SetLooping(ALuint& source, bool loop);
	static void SetGain(ALuint& source, float gain);
	static void SetPitch(ALuint& source, float pitch);
	static void SetPosition(ALuint& source, glm::vec3 pos);
	static void SetVelocity(ALuint& source, glm::vec3 vel);

	static void SetRollOffFactor(ALuint& source, float factor);
	static void SetAttenuationBeginDistance(ALuint& source, float distance);
	static void SetMaxAttenuationDistance(ALuint& source, float distance);

	static void Play(ALuint& source, ALuint& buffer);
	static void Pause(ALuint& source);
	static void Resume(ALuint& source);
	static void Stop(ALuint& source);
};

class Audio {

	static bool init;
	static ALCdevice* device;
	static ALCcontext* context;

	Audio();
public:

	static std::map<std::string, ALuint> AudioBuffers;

	static void Init();
	static void ClearBuffers();
	static void Destroy();

	static void LoadSound(std::string file, std::string name = "");
	static void SetListenerPosition(glm::vec3 pos);
	static void SetListenerVelocity(glm::vec3 vel);
	static void SetListenerGain(float gain);
	static void SetListenerPitch(float pitch);

};

#endif