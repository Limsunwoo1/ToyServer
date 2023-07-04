#pragma once

enum class ServerDataType : char
{
	ChatMessege,
	PostionData,
	DamegeData,
	RegidbodyData,
};

template<typename T>
struct eTCP_Data
{
	ServerDataType type;

	char* Name;
	T* Data;

	// »ý¼º
	TCP_Data() : type(ChatMessege), Name(nullptr), Data(nullptr) {};
	TCP_Data(ServerDataType _type, char* _name, T* _data) : type(_type), Name(_name), Data(data) {};

	void SetType(ServerDataType inType) { type = inType; }
	void SetName(char* inName) { Name = InName; }
	void SetName(T* inData) { Data = inData; }
};