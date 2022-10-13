#include "Audio.h"

ALuint AudioSource::Create() {
	ALuint source = UINT_MAX;
	alGenSources(1, &source);
	assert(source != UINT_MAX);
	return source;
}

void AudioSource::SetLooping(ALuint& source, bool loop) {
	alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

void AudioSource::SetGain(ALuint& source, float gain) {
	alSourcef(source, AL_GAIN, gain);
}

void AudioSource::SetPitch(ALuint& source, float pitch) {
	alSourcef(source, AL_PITCH, pitch);
}

void AudioSource::SetPosition(ALuint& source, glm::vec3 pos) {
	alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
}

void AudioSource::SetVelocity(ALuint& source, glm::vec3 vel) {
	alSource3f(source, AL_VELOCITY, vel.x, vel.y, vel.z);
}

void AudioSource::SetRollOffFactor(ALuint& source, float factor) {
	alSourcef(source, AL_ROLLOFF_FACTOR, factor);
}

void AudioSource::SetAttenuationBeginDistance(ALuint& source, float distance) {
	alSourcef(source, AL_REFERENCE_DISTANCE, distance);
}

void AudioSource::SetMaxAttenuationDistance(ALuint& source, float distance) {
	alSourcef(source, AL_MAX_DISTANCE, distance);
}

void AudioSource::Play(ALuint& source, ALuint& buffer) {
	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(source);
}

void AudioSource::Pause(ALuint& source) {
	alSourcePause(source);
}

void AudioSource::Resume(ALuint& source) {
	alSourcePause(source);
}

void AudioSource::Stop(ALuint& source) {
	alSourceStop(source);
}

// =========================================== //

std::map<std::string, ALuint> Audio::AudioBuffers;
bool Audio::init = false;
ALCdevice* Audio::device;
ALCcontext* Audio::context;

void Audio::Init() {
	if (init) return;
	init = true;

	device = alcOpenDevice(NULL);
	assert(device);
	context = alcCreateContext(device, NULL);
	assert(context);
	alcMakeContextCurrent(context);

	alDistanceModel(AL_EXPONENT_DISTANCE_CLAMPED);

	ALfloat forwardAndUp[] = {
		1, 0, 0,
		0, 1, 0
	};
	alListenerfv(AL_ORIENTATION, forwardAndUp);
	SetListenerPosition({ 0, 0, 0 });
	SetListenerVelocity({ 0, 0, 0 });
	SetListenerGain(10);
	SetListenerPitch(1);

	std::cout << "INIT Audio Version : ";
	std::cout << alGetString(AL_VERSION) << "\n";
}

void Audio::Destroy() {
	if (!init) return;
	init = false;

	for (auto sound : AudioBuffers) {
		std::cout << "DELETE Audio " << sound.second << " " << sound.first << "\n";
		alDeleteBuffers(1, &sound.second);
	}

	alcMakeContextCurrent(NULL);
	alcCloseDevice(device);
	alcDestroyContext(context);
	std::cout << "DELETE Audio\n";
}

void Audio::LoadSound(std::string file, std::string name) {
	
	ALenum format = AL_FORMAT_MONO16;
	AudioFile<float> audiofile;
	audiofile.load(file);
	std::vector<uint8_t> byteset;
	if (!audiofile.writePCMToBuffer(byteset))
		throw std::exception("Invalid or unsupported audio file");

	int channel = audiofile.getNumChannels();
	int bitDepth = audiofile.getBitDepth();

	if (channel == 1) {
		if (bitDepth == 8) format = AL_FORMAT_MONO8;
		else if (bitDepth == 16) format = AL_FORMAT_MONO16;
		else throw std::exception((std::to_string(bitDepth) + " bits audio is not supported").c_str());
	}
	else if (channel == 2) {
		if (bitDepth == 8) format = AL_FORMAT_STEREO8;
		else if (bitDepth == 16) format = AL_FORMAT_STEREO16;
		else throw std::exception((std::to_string(bitDepth) + " bits audio is not supported").c_str());
	}
	else {
		throw std::exception((std::to_string(channel) + " audio channels are not supported").c_str());
	}

	ALuint buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, byteset.data(), (ALsizei)byteset.size(), (ALsizei)audiofile.getSampleRate());
	
	if (AudioBuffers.find(file) != AudioBuffers.end()) {
		alDeleteBuffers(1, &AudioBuffers[file]);
	}

	if (name != "") {
		Audio::AudioBuffers[name] = buffer;
		std::cout << "CREATE Audio " << buffer << " " << name << "\n";
	}
	else {
		Audio::AudioBuffers[file] = buffer;
		std::cout << "CREATE Audio " << buffer << " " << file << "\n";
	}
}

void Audio::SetListenerPosition(glm::vec3 pos) {
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}

void Audio::SetListenerVelocity(glm::vec3 vel) {
	alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
}

void Audio::SetListenerGain(float gain) {
	alListenerf(AL_GAIN, gain);
}

void Audio::SetListenerPitch(float pitch) {
	alListenerf(AL_PITCH, pitch);
}