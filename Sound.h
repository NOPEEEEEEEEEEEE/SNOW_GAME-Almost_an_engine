#pragma once
#include "AudioEngine.h"
#include <iostream>

class Sound
{
public:
    Sound() 
    {

    }
    ~Sound()
    {
        m_SourceVoice = nullptr;
    }
    HRESULT createSound(const wchar_t* strFileName)
    {
        HRESULT hr = S_OK;
        hr = AudioEngine::GetAudioEngine()
            ->OpenFile(
            strFileName,
            m_wfx,
            m_buffer);

        return hr;

    }
    
    void playSound(float Volume)
    {
        m_SourceVoice = AudioEngine::GetAudioEngine()->playSourceVoice(m_wfx, m_buffer, Volume);
    }
    HRESULT stopSound()
    {
      
        return AudioEngine::GetAudioEngine()->stopSourceVoice(m_SourceVoice);

    }
   
private:
	IXAudio2SourceVoice* m_SourceVoice;
    WAVEFORMATEXTENSIBLE m_wfx = { 0 };
    XAUDIO2_BUFFER m_buffer = { 0 };

};