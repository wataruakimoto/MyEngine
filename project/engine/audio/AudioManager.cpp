#include "AudioManager.h"
#include <cassert>
#include <fstream>

AudioManager* AudioManager::instance = nullptr;

AudioManager* AudioManager::GetInstance() {

	if (instance == nullptr) {
		instance = new AudioManager;
	}
	return instance;
}

void AudioManager::Initialize() {

	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

void AudioManager::Finalize() {
	xAudio2.Reset();
	delete instance;
	instance = nullptr;
}

void AudioManager::SoundLoadWave(const char* filename) {

	/// === ファイルオープン === ///

	// ファイル入力ストリームのインスタンス
	std::ifstream file;

	// .wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);

	// ファイルオープン失敗を検出する
	assert(file.is_open());

	/// === .wavデータ読み込み === ///

	// RiFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	//  Formatチャンクの読み込み
	FormatChunk format = {};

	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	// bextを検出した場合
	if (strncmp(data.id, "bext", 4) == 0) {

		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);

		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	// junkチャンクを検出した場合
	if (strncmp(data.id, "junk", 4) == 0) {

		// 読み取り位置をjunkチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);

		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	// Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Waveファイルを閉じる
	file.close();

	/// === 読み込んだ音声を返す === ///

	// returnする為の音声データ
	SoundData sound = {};

	sound.wfex = format.fmt;
	sound.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	sound.bufferSize = data.size;

	soundData = sound;
}

void AudioManager::SoundUnload() {

	delete[] soundData.pBuffer;

	soundData.pBuffer = 0;
	soundData.bufferSize = 0;
	soundData.wfex = {};
}

void AudioManager::SoundPlayWave() {

	HRESULT result;

	// 波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// 波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}