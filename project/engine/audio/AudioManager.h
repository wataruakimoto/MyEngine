#pragma once

#include <xaudio2.h>
#include <wrl.h>
#include <stdint.h>
#include <fstream>
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

/// === オーディオ管理 === ///
class AudioManager {

///-------------------------------------------/// 
/// 構造体
///-------------------------------------------///
public:

	// チャンクヘッダー
	struct ChunkHeader {
		char id[4];   // チャンク毎のID
		int32_t size; // チャンクサイズ
	};

	// RIFFヘッダーチャンク
	struct RiffHeader {
		ChunkHeader chunk; // "RIFF"
		char type[4];	   // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk {
		ChunkHeader chunk; // "fmt"
		WAVEFORMATEX fmt;  // 波形フォーマット
	};

	// 音声データ
	struct SoundData {
		WAVEFORMATEX wfex;		 // 波形フォーマット
		BYTE* pBuffer;			 // バッファの先頭アドレス
		unsigned int bufferSize; // バッファのサイズ
	};

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	Microsoft::WRL::ComPtr <IXAudio2> xAudio2;

	IXAudio2MasteringVoice* masterVoice;

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static AudioManager* instance;
	// コンストラクタの隠蔽
	AudioManager() = default;
	// デストラクタの隠蔽
	~AudioManager() = default;
	// コピーコンストラクタの封印
	AudioManager(AudioManager&) = delete;
	// コピー代入演算子の封印
	AudioManager& operator=(AudioManager&) = delete;

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 音声データ読み込み
	/// </summary>
	/// <param name="filename">ファイル名</param>
	/// <returns>SoundData</returns>
	SoundData LoadWave(const char* filename);

	/// <summary>
	/// 音声データ解放
	/// </summary>
	/// <param name="soundData">音声データ</param>
	void SoundUnload(SoundData* soundData);

	/// <summary>
	/// 音声再生
	/// </summary>
	/// <param name="xAudio2"></param>
	/// <param name="soundData">音声データ</param>
	void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData);
};