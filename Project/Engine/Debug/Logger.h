#pragma once

#include <string>

namespace Engine {

	namespace Logger {

		/// <summary>
		/// ログ出力
		/// </summary>
		/// <param name="message">ログ</param>
		void Log(const std::string& message);
	};
}