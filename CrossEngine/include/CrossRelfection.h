/****************************************************************************
Copyright (c) 2015 LiangYue.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#pragma once


#include "CrossEngine.h"


#define __OFFSET__(C, M) ((unsigned long)(&((const C *)1024)->M) - 1024)


#define CLASS(NAME, BASE_NAME)                                                                     \
	private:                                                                                       \
		typedef NAME ClassType;                                                                    \
		struct __CLASS_##NAME##__                                                                  \
		{                                                                                          \
			__CLASS_##NAME##__(void)                                                               \
			{                                                                                      \
				RegistRelfectionClass(#NAME, GetClass());                                          \
			}                                                                                      \
		} __CLASS_##NAME##__;                                                                      \
	public:                                                                                        \
		static Class* GetClass(void)                                                               \
		{                                                                                          \
			static Class _class(#NAME, BASE_NAME::GetClass());                                     \
			return &_class;                                                                        \
		}

#define PROPERTY(TYPE, NAME, DESCRIBTION)                                                          \
	private:                                                                                       \
		struct __PROPERTY_##NAME##__                                                               \
		{                                                                                          \
			__PROPERTY_##NAME##__(void)                                                            \
			{                                                                                      \
				GetClass()->AddProperty(                                                           \
					#NAME,                                                                         \
					new Property(GetClass(), __OFFSET__(ClassType, NAME), #TYPE, #NAME, DESCRIBTION, typeid(TYPE)));\
			}                                                                                      \
		} __PROPERTY_##NAME##__;                                                                   \
	public:                                                                                        \
		TYPE NAME

#define FUNCTION(RETURN, NAME, PARAMS, DESCRIBTION)                                                \
	private:                                                                                       \
		struct __FUNCTION_##NAME##__                                                               \
		{                                                                                          \
			__FUNCTION_##NAME##__(void)                                                            \
			{                                                                                      \
				GetClass()->AddFunction(                                                           \
					#NAME,                                                                         \
					new Function(GetClass(), __create_function__(&ClassType::NAME), #RETURN, #NAME, #PARAMS, DESCRIBTION)); \
			}                                                                                      \
		} __FUNCTION_##NAME##__;                                                                   \
	public:                                                                                        \
		RETURN NAME PARAMS

#define VIRTUAL_FUNCTION(RETURN, NAME, PARAMS, DESCRIBTION)                                        \
	private:                                                                                       \
		struct __FUNCTION_##NAME##__                                                               \
		{                                                                                          \
			__FUNCTION_##NAME##__(void)                                                            \
			{                                                                                      \
				GetClass()->AddFunction(                                                           \
					#NAME,                                                                         \
					new Function(GetClass(), __create_function__(&ClassType::NAME), #RETURN, #NAME, #PARAMS, DESCRIBTION)); \
			}                                                                                      \
		} __FUNCTION_##NAME##__;                                                                   \
	public:                                                                                        \
		virtual RETURN NAME PARAMS


struct __function_base__
{
	virtual ~__function_base__(void)
	{
	}
};

template<class R, class C, class... Types> struct __function__ : __function_base__
{
	typedef R(C::*Method)(Types...);
	Method pMethod;

	__function__(Method m)
	{
		pMethod = m;
	}

	void Invoke(R &result, C *object, Types... params)
	{
		result = (object->*pMethod)(params...);
	}
};

template<class R, class C, class... Types> struct __function_const__ : __function_base__
{
	typedef R(C::*Method)(Types...) const;
	Method pMethod;

	__function_const__(Method m)
	{
		pMethod = m;
	}

	void Invoke(R &result, C *object, Types... params)
	{
		result = (object->*pMethod)(params...);
	}
};

template<class C, class... Types> struct __void_function__ : __function_base__
{
	typedef void (C::*Method)(Types...);
	Method pMethod;

	__void_function__(Method m)
	{
		pMethod = m;
	}

	void Invoke(C *object, Types... params)
	{
		(object->*pMethod)(params...);
	}
};

template<class C, class... Types> struct __void_function_const__ : __function_base__
{
	typedef void (C::*Method)(Types...) const;
	Method pMethod;

	__void_function_const__(Method m)
	{
		pMethod = m;
	}

	void Invoke(C *object, Types... params)
	{
		(object->*pMethod)(params...);
	}
};

template <class R, class C, class... Types> __function__<R, C, Types...>* __create_function__(R(C::*method)(Types...))
{
	return new __function__<R, C, Types...>(method);
}

template <class R, class C, class... Types> __function_const__<R, C, Types...>* __create_function__(R(C::*method)(Types...) const)
{
	return new __function_const__<R, C, Types...>(method);
}

template <class C, class... Types> __void_function__<C, Types...>* __create_function__(void (C::*method)(Types...))
{
	return new __void_function__<C, Types...>(method);
}

template <class C, class... Types> __void_function_const__<C, Types...>* __create_function__(void (C::*method)(Types...) const)
{
	return new __void_function_const__<C, Types...>(method);
}


class Class;
class Property;
class Function;

class Property
{
public:
	Property(Class *pClass, unsigned long offset, const char *type, const char *name, const char *describtion, const std::type_info &typeInfo)
		: m_typeInfo(typeInfo)
	{
		m_pClass = pClass;
		m_offset = offset;
		m_type = type;
		m_name = name;
		m_describtion = describtion;
	}

	const std::string& GetType(void) const
	{
		return m_type;
	}

	const std::string& GetName(void) const
	{
		return m_name;
	}

	const std::string& GetDescribtion(void) const
	{
		return m_describtion;
	}

	const std::type_info& GetTypeInfo(void) const
	{
		return m_typeInfo;
	}

	template<class C, class Value>void Set(C *object, const Value &v)
	{
		if (typeid(v) == m_typeInfo) {
			*(Value *)((unsigned char *)object + m_offset) = v;
			return;
		}

		printf("Set Fail. %s::%s %s\n", m_pClass->GetName(), m_type.c_str(), m_name.c_str());
	}

	template<class C, class Value>void Get(C *object, Value &v) const
	{
		if (typeid(v) == m_typeInfo) {
			v = *(Value *)((unsigned char *)object + m_offset);
			return;
		}

		printf("Get Fail. %s::%s %s\n", m_pClass->GetName(), m_type.c_str(), m_name.c_str());
	}

	template<class C, class Value>void GetPtr(C *object, Value **v) const
	{
		if (typeid(v) == m_typeInfo) {
			*v = (Value *)((unsigned char *)object + m_offset);
			return;
		}

		printf("GetPtr Fail. %s::%s %s\n", m_pClass->GetName(), m_type.c_str(), m_name.c_str());
	}

private:
	std::string m_type;
	std::string m_name;
	std::string m_describtion;
	const std::type_info &m_typeInfo;

private:
	Class *m_pClass;
	unsigned long m_offset;
};

class Function
{
public:
	Function(Class *pClass, __function_base__ *pFunction, const char *ret, const char *name, const char *params, const char *describtion)
	{
		m_pClass = pClass;
		m_pFunction = pFunction;
		m_name = name;
		m_return = ret;
		m_params = params;
		m_describtion = describtion;
	}

	const std::string& GetName(void) const
	{
		return m_name;
	}

	const std::string& GetReturnType(void) const
	{
		return m_return;
	}

	const std::string& GetParams(void) const
	{
		return m_params;
	}

	template <class R, class C, class... Types>void Invoke(R &result, C *object, Types... params)
	{
		__function__<R, C, Types...>* pFunction = dynamic_cast<__function__<R, C, Types...>*>(m_pFunction);
		__function_const__<R, C, Types...>* pFunctionConst = dynamic_cast<__function_const__<R, C, Types...>*>(m_pFunction);

		if (pFunction) {
			pFunction->Invoke(result, object, params...);
			return;
		}

		if (pFunctionConst) {
			pFunctionConst->Invoke(result, object, params...);
			return;
		}

		printf("Invoke Fail: %s::%s %s%s\n", m_pClass->GetName().c_str(), m_return.c_str(), m_name.c_str(), m_params.c_str());
	}

	template <class C, class... Types>void InvokeVoid(C *object, Types... params)
	{
		__void_function__<C, Types...>* pFunction = dynamic_cast<__void_function__<C, Types...>*>(m_pFunction);
		__void_function_const__<C, Types...>* pFunctionConst = dynamic_cast<__void_function_const__<C, Types...>*>(m_pFunction);

		if (pFunction) {
			pFunction->Invoke(object, params...);
			return;
		}

		if (pFunctionConst) {
			pFunctionConst->Invoke(object, params...);
			return;
		}

		printf("InvokeVoid Fail: %s::%s %s%s\n", m_pClass->GetName().c_str(), m_return.c_str(), m_name.c_str(), m_params.c_str());
	}

private:
	std::string m_name;
	std::string m_return;
	std::string m_params;
	std::string m_describtion;

private:
	Class *m_pClass;
	__function_base__ *m_pFunction;
};

class ClassNull
{
public:
	static Class *GetClass(void)
	{
		return NULL;
	}
};

class Class
{
public:
	Class(const char *name, Class *pBase)
	{
		m_name = name;
		m_pBase = pBase;
	}

	~Class(void)
	{
		for (std::map<std::string, Property*>::const_iterator itProperty = m_properties.begin(); itProperty != m_properties.end(); ++itProperty) {
			delete itProperty->second;
		}

		for (std::map<std::string, Function*>::const_iterator itFunction = m_functions.begin(); itFunction != m_functions.end(); ++itFunction) {
			delete itFunction->second;
		}
	}

	Class* GetBase(void) const
	{
		return m_pBase;
	}

	const std::string& GetName(void) const
	{
		return m_name;
	}

	const std::map<std::string, Property*>& GetProperties(void) const
	{
		return m_properties;
	}

	const std::map<std::string, Function*>& GetFunctions(void) const
	{
		return m_functions;
	}

	void AddProperty(const char *name, Property *pProperty)
	{
		std::map<std::string, Property*>::const_iterator itProperty = m_properties.find(name);
		if (itProperty == m_properties.end()) {
			m_properties[name] = pProperty;
		}
	}

	Property* GetProperty(const char *name)
	{
		std::map<std::string, Property*>::const_iterator itProperty = m_properties.find(name);
		return itProperty != m_properties.end() ? itProperty->second : m_pBase ? m_pBase->GetProperty(name) : NULL;
	}

	void AddFunction(const char *name, Function *pFunction)
	{
		std::map<std::string, Function*>::const_iterator itFunction = m_functions.find(name);
		if (itFunction == m_functions.end()) {
			m_functions[name] = pFunction;
		}
	}

	Function* GetFunction(const char *name)
	{
		std::map<std::string, Function*>::const_iterator itFunction = m_functions.find(name);
		return itFunction != m_functions.end() ? itFunction->second : m_pBase ? m_pBase->GetFunction(name) : NULL;
	}

private:
	Class *m_pBase;

	std::string m_name;
	std::map<std::string, Property*> m_properties;
	std::map<std::string, Function*> m_functions;
};

extern void RegistRelfectionClass(const char *name, Class *pClass);
extern Class* GetRelfectionClass(const char *name);
