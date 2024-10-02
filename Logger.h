#pragma once

#include <string>

class Logger {

public: //静的メンバ関数

	/// <summary>
	/// ログ出力
	/// </summary>
	/// <param name="message">ログ</param>
	static void Log(const std::string& message);
};