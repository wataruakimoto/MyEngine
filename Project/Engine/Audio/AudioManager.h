#pragma once

#include <xaudio2.h>
#include <wrl.h>
#include <stdint.h>
#include <unordered_map>
#include <string>

#pragma comment(lib,"xaudio2.lib")

namespace Engine {

	/// === オーディオ管理 === ///
	class AudioManager {

		/// ================================================== ///
		/// シングルトン
		/// ================================================== ///
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

		/// ================================================== ///
		/// 構造体
		/// ================================================== ///
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

		// サウンドタイプ
		enum class SoundType {
			BGM,	// 背景音楽
			SE		// 効果音
		};

		/// ================================================== ///
		/// メンバ関数
		/// ================================================== ///
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

		/// <summary>
		/// 音声データ読み込み
		/// </summary>
		/// <param name="fileName">ファイル名</param>
		/// <returns>SoundData</returns>
		void SoundLoadWave(const std::string& fileName);

		/// <summary>
		/// 音声データ解放
		/// </summary>
		void SoundUnload(const std::string& fileName);

		/// <summary>
		/// 全ての音声データ解放
		/// </summary>
		void SoundUnloadAll();

		/// <summary>
		/// 音声再生
		/// </summary>
		/// <param name="soundName">サウンド名</param>
		/// <param name="volume">音量（0.0f～1.0f）</param>
		/// <param name="type">サウンドタイプ</param>
		void SoundPlayWave(const std::string& fileName, float volume = 1.0f, SoundType type = SoundType::SE);

		/// ================================================== ///
		/// ゲッター
		/// ================================================== ///
	public:

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static AudioManager* GetInstance();

		/// ================================================== ///
		/// セッター
		/// ================================================== ///
	public:

		/// ================================================== ///
		/// メンバ変数
		/// ================================================== ///
	private:

		Microsoft::WRL::ComPtr <IXAudio2> xAudio2;

		IXAudio2MasteringVoice* masterVoice;

		// BGM用のSourceVoice
		IXAudio2SourceVoice* pBGMSourceVoice = nullptr;

		// 音声データのマップ キーはサウンド名
		std::unordered_map<std::string, SoundData> soundDatas;

		// ベースディレクトリパス
		const std::string baseDirectoryPath = "Resources/Sounds/";
	};
}