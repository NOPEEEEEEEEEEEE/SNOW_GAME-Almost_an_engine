#include "AudioEngine.h"
#include "xaudio2.h"
#include <exception>


#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif




AudioEngine* AudioEngine::m_audio_engine = nullptr;

AudioEngine::AudioEngine()
{

    if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
        throw std::exception("Audio Engine was not created successfully");

    if (FAILED( XAudio2Create(&m_IXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
        throw std::exception("Audio Engine was not created successfully");
    
    if (FAILED(m_IXAudio2->CreateMasteringVoice(&m_MasterVoice)))
        throw std::exception("Audio Engine was not created successfully");

    
}

AudioEngine::~AudioEngine()
{   
    AudioEngine::m_audio_engine = nullptr;
    m_MasterVoice = nullptr;
    m_IXAudio2 = nullptr;
}

HRESULT AudioEngine::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD bytesRead = 0;
    DWORD dwOffset = 0;

    while (hr == S_OK)
    {
        DWORD dwRead;
        if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        switch (dwChunkType)
        {
        case fourccRIFF:
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
                hr = HRESULT_FROM_WIN32(GetLastError());
            break;

        default:
            if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
                return HRESULT_FROM_WIN32(GetLastError());
        }

        dwOffset += sizeof(DWORD) * 2;

        if (dwChunkType == fourcc)
        {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;

        if (bytesRead >= dwRIFFDataSize) return S_FALSE;

    }

    return S_OK;
}

HRESULT AudioEngine::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());
    DWORD dwRead;
    if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
        hr = HRESULT_FROM_WIN32(GetLastError());
    return hr;
}

HRESULT AudioEngine::OpenFile(const wchar_t* strFileName, WAVEFORMATEXTENSIBLE& wfx, XAUDIO2_BUFFER& buffer )
{

    HANDLE hFile = CreateFile(
        strFileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);
   
    if (INVALID_HANDLE_VALUE == hFile)
        return HRESULT_FROM_WIN32(GetLastError());

    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    DWORD dwChunkSize;
    DWORD dwChunkPosition;
    //check the file type, should be fourccWAVE or 'XWMA'
    FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
    DWORD filetype;
    ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
    if (filetype != fourccWAVE)
        return S_FALSE;

    FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
    ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

    FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
    BYTE* pDataBuffer = new BYTE[dwChunkSize];
    ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);
    buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
    buffer.pAudioData = pDataBuffer;  //buffer containing audio data
    buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

    return S_OK;
}

IXAudio2SourceVoice* AudioEngine::playSourceVoice( WAVEFORMATEXTENSIBLE wfx, XAUDIO2_BUFFER buffer, float Volume)
{
   
    IXAudio2SourceVoice* Source;
    m_IXAudio2->CreateSourceVoice(&Source, (WAVEFORMATEX*)&wfx);
        
    Source->SubmitSourceBuffer(&buffer);
       
    Source->SetVolume(Volume);
    Source->Start(0);
    return Source;
}

HRESULT AudioEngine::stopSourceVoice(IXAudio2SourceVoice* SourceVoice)
{
    return SourceVoice->Stop();
}

AudioEngine* AudioEngine::GetAudioEngine()
{
    return m_audio_engine;
}

void AudioEngine::create()
{
    
    if (AudioEngine::m_audio_engine)
         throw std::exception("Audio Engine already created");
    AudioEngine::m_audio_engine = new AudioEngine();
    
}

void AudioEngine::release()
{
     if (!AudioEngine::m_audio_engine)
       return;
     delete AudioEngine::m_audio_engine;
    
}
