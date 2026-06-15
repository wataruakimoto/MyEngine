#include "AudioManager.h"

#include <cassert>
#include <fstream>

using namespace Engine;

void AudioManager::Initialize() {

	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

void AudioManager::Finalize() {

	SoundUnloadAll();

	// 再生中のBGMがあれば
	if (pBGMSourceVoice) {
		pBGMSourceVoice->Stop(); // 再生停止
		pBGMSourceVoice->DestroyVoice(); // SourceVoiceの破棄
		pBGMSourceVoice = nullptr; // ポインタをnullptrに設定
	}

	xAudio2.Reset();
	delete instance;
	instance = nullptr;
}

void AudioManager::SoundLoadWave(const std::string& fileName) {

	// すでに読み込まれていたら早期リターン
	if (soundDatas.find(fileName) != soundDatas.end()) return;

	/// === ファイルオープン === ///

	// ファイル入力ストリームのインスタンス
	std::ifstream file;

	// ベースディレクトリパスとファイル名を結合してフルパスを作成
	std::string fullPath = baseDirectoryPath + fileName;

	// .wavファイルをバイナリモードで開く
	file.open(fullPath, std::ios_base::binary);

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
	while (file.read((char*)&data, sizeof(data))) {
		// dataチャンク発見で終了
		if (strncmp(data.id, "data", 4) == 0) {
			break;
		}

		// その他のチャンク（bext, junk, LIST, INFO等）をスキップ
		file.seekg(data.size, std::ios_base::cur);

		// アラインメント対応（サイズが奇数の場合、パディングバイトが存在）
		if (data.size % 2 != 0) {
			file.seekg(1, std::ios_base::cur);
		}
	}

	// dataチャンク確認
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

	soundDatas[fileName] = sound;
}

void AudioManager::SoundUnload(const std::string& fileName) {

	auto it = soundDatas.find(fileName);
	if (it != soundDatas.end()) {
		delete[] it->second.pBuffer;
		soundDatas.erase(it);
	}
}

void AudioManager::SoundUnloadAll() {

	// 全ての音声データを解放
	for (auto& soundData : soundDatas) {
		delete[] soundData.second.pBuffer;
	}
}

void AudioManager::SoundPlayWave(const std::string& fileName, float volume, SoundType type) {

	// 音声データの検索
	auto it = soundDatas.find(fileName);
	if (it == soundDatas.end()) {
		assert(0);
	}

	// 音声データの取得
	const SoundData& soundData = it->second;

	HRESULT result;

	// 波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;

	// BGMの場合
	if (type == SoundType::BGM) {

		// すでにBGMが再生されている場合
		if (pBGMSourceVoice != nullptr) {
			pBGMSourceVoice->Stop(); // 再生停止
			pBGMSourceVoice->DestroyVoice(); // SourceVoiceの破棄
			pBGMSourceVoice = nullptr; // ポインタをnullptrに設定
		}

		// 新しいBGM用SourceVoiceの生成
		result = xAudio2->CreateSourceVoice(&pBGMSourceVoice, &soundData.wfex);
		assert(SUCCEEDED(result));
		pSourceVoice = pBGMSourceVoice;
	}
	// SEの場合
	else {

		// SE用のSourceVoiceの生成
		result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
		assert(SUCCEEDED(result));
	}

	// 音量の設定
	pSourceVoice->SetVolume(volume);

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	if (type == SoundType::BGM) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE; // BGMはループ再生
	}
	else {
		buf.LoopCount = 0; // SEはループなし
	}

	// 波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));

	result = pSourceVoice->Start();
	assert(SUCCEEDED(result));
}

AudioManager* AudioManager::instance = nullptr;

AudioManager* AudioManager::GetInstance() {

	if (instance == nullptr) {
		instance = new AudioManager;
	}
	return instance;
}