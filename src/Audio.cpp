#include "../header/Audio.h"

bool AudioBuffer::log = true;

AudioBuffer::AudioBuffer(std::string file) {
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

	alGenBuffers(1, &handle);
	alBufferData(handle, format, byteset.data(), (ALsizei)byteset.size(), (ALsizei)audiofile.getSampleRate());
}

AudioBuffer::~AudioBuffer() {
	if (log) std::cout << "DELETE Audio " << handle << "\n";
	alDeleteBuffers(1, &handle);
}

Ref<AudioBuffer> AudioBuffer::Create(std::string file) {
	return std::make_shared<AudioBuffer>(file);
}

// ============================================================================== //

std::vector<ALuint> AudioSource::sourceBuffers;
bool AudioSource::log = true;

ALuint AudioSource::Create() {
	ALuint source = UINT_MAX;
	alGenSources(1, &source);
	assert(source != UINT_MAX);
	sourceBuffers.push_back(source);
	if (log) std::cout << "CREATE AudioSource " << source << "\n";
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

void AudioSource::Play(ALuint& source, Ref<AudioBuffer>& buffer) {
	alSourcei(source, AL_BUFFER, buffer->handle);
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

// ============================================================================== //

bool Audio::log = true;

std::map<std::string, Ref<AudioBuffer>> Audio::AudioBuffers;
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
	SetListenerGain(1);
	SetListenerPitch(1);

	AudioBuffer::log = log;
	AudioSource::log = log;

	LoadSound("../audio/bounce.wav", "bounce");

	if (log)
		std::cout << "INIT Audio Version : " << alGetString(AL_VERSION) << "\n";
}

void Audio::ClearBuffers() {
	for (auto& audio : AudioBuffers) {
		audio.second.reset();
	}
	AudioBuffers.clear();
	for (auto& source : AudioSource::sourceBuffers) {
		if (log) std::cout << "DELETE AudioSource " << source << "\n";
		alDeleteSources(1, &source);
	}
	AudioSource::sourceBuffers.clear();
}

void Audio::Destroy() {
	if (!init) return;
	init = false;

	ClearBuffers();

	alcMakeContextCurrent(NULL);
	alcCloseDevice(device);
	alcDestroyContext(context);
	if (log) std::cout << "DELETE Audio\n";
}

void Audio::LoadSound(std::string file, std::string name) {
	if (name != "" && AudioBuffers.find(name) != AudioBuffers.end()) {
		AudioBuffers[name].reset();
	}
	else if (AudioBuffers.find(file) != AudioBuffers.end()) {
		AudioBuffers[file].reset();
	}

	Ref<AudioBuffer> buffer = AudioBuffer::Create(file);

	if (name != "") {
		Audio::AudioBuffers[name] = buffer;
		if (log) std::cout << "CREATE Audio " << buffer->handle << " " << name << "\n";
	}
	else {
		Audio::AudioBuffers[file] = buffer;
		if (log) std::cout << "CREATE Audio " << buffer->handle << " " << file << "\n";
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

float Audio::GetListenerGain() {
	ALfloat value;
	alGetListenerf(AL_GAIN, &value);
	return value;
}