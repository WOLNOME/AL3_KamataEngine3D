#pragma once
#include "Function.h"
#include <json.hpp>
#include <map>
#include <string>
#include <variant>

class GlobalVariables {
private:
	//using
	using json = nlohmann::json;

public:
	static GlobalVariables* GetInstance();

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	const GlobalVariables& operator=(const GlobalVariables&) = delete;

	void LoadFile(const std::string& groupName);
public:
	void Update();

public:
	void CreateGroup(const std::string& groupName);
	void SaveFile(const std::string& groupName);
	void LoadFiles();

	//値のセット
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	void SetValue(const std::string& groupName, const std::string& key, float value);
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	//値の取得
	int32_t GetIntValue(const std::string& groupName, const std::string& key) const;
	float GetFloatValue(const std::string& groupName, const std::string& key) const;
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key) const;

	//項目の追加
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	void AddItem(const std::string& groupName, const std::string& key, float value);
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);


private:
	// 項目
	using Item = std::variant<int32_t,float ,Vector3>;
	// グループ
	using Group=std::map<std::string,Item>;

	// 全データ
	std::map<std::string, Group> datas_;
	//グローバル変数の保存先ファイルパス
	const std::string kDirectryPath = "Resources/GlobalVariables/";

};
