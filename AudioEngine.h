#pragma once
#include "xaudio2.h"
#include <wrl/client.h>


class AudioEngine
{

public:

	static AudioEngine* GetAudioEngine();

	static void create();
	
	static void release();

private:
	AudioEngine();
	~AudioEngine();
    
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);

	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);

	IXAudio2SourceVoice* playSourceVoice(WAVEFORMATEXTENSIBLE wfx, XAUDIO2_BUFFER buffer, float Volume);
	HRESULT stopSourceVoice(IXAudio2SourceVoice* m_SourceVoice);


    HRESULT OpenFile(const wchar_t* strFileName, WAVEFORMATEXTENSIBLE& wfx, XAUDIO2_BUFFER& buffer);

    static AudioEngine* m_audio_engine;

	Microsoft::WRL::ComPtr<IXAudio2>m_IXAudio2 = nullptr;

	IXAudio2MasteringVoice* m_MasterVoice = nullptr;

	friend class Sound;
};

