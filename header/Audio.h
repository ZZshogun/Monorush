#ifndef AUDIO_CLASS_H
#define AUDIO_CLASS_H

#include "Core.h"

class AudioBuffer {
public:
	static bool log;

	ALuint handle;
	AudioBuffer(std::string file);
	~AudioBuffer();
	static Ref<AudioBuffer> Create(std::string file);
};

class AudioSource {
	AudioSource();

	static std::vector<ALuint> sourceBuffers;
	friend class Audio;
public:
	static bool log;
	static ALuint Create();

	static void SetLooping(ALuint& source, bool loop);
	static void SetGain(ALuint& source, float gain);
	static void SetPitch(ALuint& source, float pitch);
	static void SetPosition(ALuint& source, glm::vec3 pos);
	static void SetVelocity(ALuint& source, glm::vec3 vel);

	static void SetRollOffFactor(ALuint& source, float factor);
	static void SetAttenuationBeginDistance(ALuint& source, float distance);
	static void SetMaxAttenuationDistance(ALuint& source, float distance);

	static void Play(ALuint& source, Ref<AudioBuffer>& buffer);
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
	static std::map<std::string, Ref<AudioBuffer>> AudioBuffers;
	static bool log;

	static void Init();
	static void ClearBuffers();
	static void Destroy();

	static void LoadSound(std::string file, std::string name = "");
	static void SetListenerPosition(glm::vec3 pos);
	static void SetListenerVelocity(glm::vec3 vel);
	static void SetListenerGain(float gain);
	static void SetListenerPitch(float pitch);

	static float GetListenerGain();
};

#endif