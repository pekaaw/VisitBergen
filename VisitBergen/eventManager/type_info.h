#ifndef TYPEINFO_H
#define TYPEINFO_H

#include <typeinfo.h>
#include <cstdio>

class TypeInfo
{
public:
	explicit TypeInfo(const type_info& info) : typeInfo(info)
	{};

	bool operator < (const TypeInfo& rhs) const
	{
		return this->typeInfo.before(rhs.typeInfo) != 0;
	}

	const char* getName() const
	{
		return this->typeInfo.name();
	}

private:
	const type_info& typeInfo;
};

#endif