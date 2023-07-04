#pragma once
#include "def.h"

enum class ServerDataType : int
{
	ChatMessege,
	PostionData,
	DamegeData,
	RigidbodyData,
};

template<typename T>
struct eTCP_Data
{
	ServerDataType type;

	char* Name;
	char Password[MAX_PASSWORD_SIZE];


	T* Data;

	// »ý¼º
	eTCP_Data() : type(ServerDataType::ChatMessege), Name(nullptr), Data(nullptr) {};
	eTCP_Data(ServerDataType _type, char* _name, T* _data) : type(_type), Name(_name), Data(_data) {};

	void SetType(ServerDataType inType) { type = inType; }
	void SetName(char* inName) { Name = inName; }
	void SetData(T* inData) { Data = inData; }
};