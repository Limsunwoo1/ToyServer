#pragma once

#define SINGLE(type)						\
public:										\
	static type* GetInstance()				\
	{										\
		static type* mInstance;				\
		if (mInstance == nullptr)			\
		{									\
			mInstance = new type();			\
			return mInstance;				\
		}									\
		return mInstance;					\
	}										\
											\
	static void DestroyInstance()			\
	{										\
	if (GetInstance())						\
		delete GetInstance();				\
	}										\
											\
private:									\
	type();									\
	~type();								\

#define GETSINGLE(type)	\
type::GetInstance()		\

// 반환형, 타겟, 생성되는 메소드명
#define GETSET(__TYPE__, __TARGET__, __METHODNAME__) \
__TYPE__ Get##__METHODNAME__()	const				 \
{													 \
	return __TARGET__;								 \
}													 \
													 \
void Set##__METHODNAME__(__TYPE__ value)			 \
{													 \
	__TARGET__ = value;								 \
}													 \


#define MAX_DATA_SIZE		1024
#define MAX_NAME_SIZE		20
#define MAX_PASSWORD_SIZE	40
