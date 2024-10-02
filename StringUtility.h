#pragma once

#include <string>

class StringUtility {

public: // 静的メンバ関数

	/// <summary>
	/// stringをwstringに変換
	/// </summary>
	/// <param name="str">string型の文字列</param>
	/// <returns>wstring型の文字列</returns>
	static std::wstring ConvertString(const std::string& str);

	/// <summary>
	/// wstringをstringに変換
	/// </summary>
	/// <param name="str">wstring型の文字列</param>
	/// <returns>string型の文字列</returns>
	static std::string ConvertString(const std::wstring& str);
};