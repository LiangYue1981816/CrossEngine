#include <map>
#include <string>


#define __REPEATA0
#define __REPEATA1 , class T1
#define __REPEATA2 , class T1, class T2
#define __REPEATA3 , class T1, class T2, class T3
#define __REPEATA4 , class T1, class T2, class T3, class T4
#define __REPEATA5 , class T1, class T2, class T3, class T4, class T5
#define __REPEATA6 , class T1, class T2, class T3, class T4, class T5, class T6
#define __REPEATA7 , class T1, class T2, class T3, class T4, class T5, class T6, class T7
#define __REPEATA8 , class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8
#define __REPEATA9 , class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9
#define REPEATA(N) __REPEATA##N

#define __REPEATB0
#define __REPEATB1 T1
#define __REPEATB2 T1, T2
#define __REPEATB3 T1, T2, T3
#define __REPEATB4 T1, T2, T3, T4
#define __REPEATB5 T1, T2, T3, T4, T5
#define __REPEATB6 T1, T2, T3, T4, T5, T6
#define __REPEATB7 T1, T2, T3, T4, T5, T6, T7
#define __REPEATB8 T1, T2, T3, T4, T5, T6, T7, T8
#define __REPEATB9 T1, T2, T3, T4, T5, T6, T7, T8, T9
#define REPEATB(N) __REPEATB##N

#define __REPEATC0
#define __REPEATC1 , T1 t1
#define __REPEATC2 , T1 t1, T2 t2
#define __REPEATC3 , T1 t1, T2 t2, T3 t3
#define __REPEATC4 , T1 t1, T2 t2, T3 t3, T4 t4
#define __REPEATC5 , T1 t1, T2 t2, T3 t3, T4 t4, T5 t5
#define __REPEATC6 , T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6
#define __REPEATC7 , T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7
#define __REPEATC8 , T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8
#define __REPEATC9 , T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9
#define REPEATC(N) __REPEATC##N

#define __REPEATD0
#define __REPEATD1 t1
#define __REPEATD2 t1, t2
#define __REPEATD3 t1, t2, t3
#define __REPEATD4 t1, t2, t3, t4
#define __REPEATD5 t1, t2, t3, t4, t5
#define __REPEATD6 t1, t2, t3, t4, t5, t6
#define __REPEATD7 t1, t2, t3, t4, t5, t6, t7
#define __REPEATD8 t1, t2, t3, t4, t5, t6, t7, t8
#define __REPEATD9 t1, t2, t3, t4, t5, t6, t7, t8, t9
#define REPEATD(N) __REPEATD##N

#define __REPEATE0
#define __REPEATE1 , T1
#define __REPEATE2 , T1, T2
#define __REPEATE3 , T1, T2, T3
#define __REPEATE4 , T1, T2, T3, T4
#define __REPEATE5 , T1, T2, T3, T4, T5
#define __REPEATE6 , T1, T2, T3, T4, T5, T6
#define __REPEATE7 , T1, T2, T3, T4, T5, T6, T7
#define __REPEATE8 , T1, T2, T3, T4, T5, T6, T7, T8
#define __REPEATE9 , T1, T2, T3, T4, T5, T6, T7, T8, T9
#define REPEATE(N) __REPEATE##N


struct __function_base
{
	virtual ~__function_base(void)
	{
	}
};

#define DEFINE_FUNCTION(N)                                                                         \
template<class R, class C REPEATA(N)> struct __function_##N : __function_base                      \
{                                                                                                  \
	typedef R(C::*Method)(REPEATB(N));                                                             \
	Method pMethod;                                                                                \
	__function_##N(Method m)                                                                       \
	{                                                                                              \
		pMethod = m;                                                                               \
	}                                                                                              \
	void Invoke(R &result, C *object REPEATC(N))                                                   \
	{                                                                                              \
		result = (object->*pMethod)(REPEATD(N));                                                   \
	}                                                                                              \
};

#define DEFINE_FUNCTION_CONST(N)                                                                   \
template<class R, class C REPEATA(N)> struct __function_##N##_const : __function_base              \
{                                                                                                  \
	typedef R(C::*Method)(REPEATB(N)) const;                                                       \
	Method pMethod;                                                                                \
	__function_##N##_const(Method m)                                                               \
	{                                                                                              \
		pMethod = m;                                                                               \
	}                                                                                              \
	void Invoke(R &result, C *object REPEATC(N))                                                   \
	{                                                                                              \
		result = (object->*pMethod)(REPEATD(N));                                                   \
	}                                                                                              \
};

#define DEFINE_VOID_FUNCTION(N)                                                                    \
template<class C REPEATA(N)> struct __void_function_##N : __function_base                          \
{                                                                                                  \
	typedef void(C::*Method)(REPEATB(N));                                                          \
	Method pMethod;                                                                                \
	__void_function_##N(Method m)                                                                  \
	{                                                                                              \
		pMethod = m;                                                                               \
	}                                                                                              \
	void Invoke(C *object REPEATC(N))                                                              \
	{                                                                                              \
		(object->*pMethod)(REPEATD(N));                                                            \
	}                                                                                              \
};

#define DEFINE_VOID_FUNCTION_CONST(N)                                                              \
template<class C REPEATA(N)> struct __void_function_##N##_const : __function_base                  \
{                                                                                                  \
	typedef void(C::*Method)(REPEATB(N)) const;                                                    \
	Method pMethod;                                                                                \
	__void_function_##N##_const(Method m)                                                          \
	{                                                                                              \
		pMethod = m;                                                                               \
	}                                                                                              \
	void Invoke(C *object REPEATC(N))                                                              \
	{                                                                                              \
		(object->*pMethod)(REPEATD(N));                                                            \
	}                                                                                              \
};

#define CREATE_FUNCTION(N)                                                                         \
template <class R, class C REPEATA(N)> __function_##N<R, C REPEATE(N)>* __create_function(R(C::*method)(REPEATB(N))) \
{                                                                                                  \
	return new __function_##N<R, C REPEATE(N)>(method);                                            \
}

#define CREATE_FUNCTION_CONST(N)                                                                   \
template <class R, class C REPEATA(N)> __function_##N##_const<R, C REPEATE(N)>* __create_function(R(C::*method)(REPEATB(N))const) \
{                                                                                                  \
	return new __function_##N##_const<R, C REPEATE(N)>(method);                                    \
}

#define CREATE_VOID_FUNCTION(N)                                                                    \
template <class C REPEATA(N)> __void_function_##N<C REPEATE(N)>* __create_function(void(C::*method)(REPEATB(N))) \
{                                                                                                  \
	return new __void_function_##N<C REPEATE(N)>(method);                                          \
}

#define CREATE_VOID_FUNCTION_CONST(N)                                                              \
template <class C REPEATA(N)> __void_function_##N##_const<C REPEATE(N)>* __create_function(void(C::*method)(REPEATB(N))const) \
{                                                                                                  \
	return new __void_function_##N##_const<C REPEATE(N)>(method);                                  \
}

#define INVOKE(N)                                                                                  \
template <class R, class C REPEATA(N)>void Invoke(R &result, C *object REPEATC(N))                 \
{                                                                                                  \
	__function_##N<R, C REPEATE(N)>* pFunction = dynamic_cast<__function_##N<R, C REPEATE(N)>*>(m_pFunction); \
	__function_##N##_const<R, C REPEATE(N)>* pFunctionConst = dynamic_cast<__function_##N##_const<R, C REPEATE(N)>*>(m_pFunction); \
	if (pFunction)                                                                                 \
	{                                                                                              \
		pFunction->Invoke(result, object, REPEATD(N));                                             \
		return;                                                                                    \
	}                                                                                              \
	if (pFunctionConst)                                                                            \
	{                                                                                              \
		pFunctionConst->Invoke(result, object, REPEATD(N));                                        \
		return;                                                                                    \
	}                                                                                              \
	printf("Invoke Fail: %s::%s %s%s\n", m_pClass->GetName().c_str(), m_return.c_str(), m_name.c_str(), m_params.c_str()); \
}

#define INVOKE_VOID(N)                                                                             \
template <class C REPEATA(N)>void InvokeVoid(C *object REPEATC(N))                                 \
{                                                                                                  \
	__void_function_##N<C REPEATE(N)>* pFunction = dynamic_cast<__void_function_##N<C REPEATE(N)>*>(m_pFunction); \
	__void_function_##N##_const<C REPEATE(N)>* pFunctionConst = dynamic_cast<__void_function_##N##_const<C REPEATE(N)>*>(m_pFunction); \
	if (pFunction)                                                                                 \
	{                                                                                              \
		pFunction->Invoke(object, REPEATD(N));                                                     \
		return;                                                                                    \
	}                                                                                              \
	if (pFunctionConst)                                                                            \
	{                                                                                              \
		pFunctionConst->Invoke(object, REPEATD(N));                                                \
		return;                                                                                    \
	}                                                                                              \
	printf("InvokeVoid Fail: %s::%s %s%s\n", m_pClass->GetName().c_str(), m_return.c_str(), m_name.c_str(), m_params.c_str()); \
}


#define __OFFSET__(C, M) ((unsigned long)(&((const C *)1024)->M) - 1024)


#define CLASS(NAME, BASE_NAME)                                                                     \
	public:                                                                                        \
	typedef NAME ClassType;                                                                        \
	struct __CLASS_##NAME##__                                                                      \
	{                                                                                              \
		__CLASS_##NAME##__(void)                                                                   \
		{                                                                                          \
			SetRelfectionClass(#NAME, GetClass());                                                 \
		}                                                                                          \
	} __CLASS_##NAME##__;                                                                          \
	static Class* GetClass(void)                                                                   \
	{                                                                                              \
		static Class _class(#NAME, BASE_NAME::GetClass());                                         \
		return &_class;                                                                            \
	}

#define PROPERTY(TYPE, NAME, DESCRIBTION)                                                          \
	struct __PROPERTY_##NAME##__                                                                   \
	{                                                                                              \
		__PROPERTY_##NAME##__(void)                                                                \
		{                                                                                          \
			GetClass()->AddProperty(                                                               \
				#NAME,                                                                             \
				new Property(GetClass(), __OFFSET__(ClassType, NAME), #TYPE, #NAME, DESCRIBTION, typeid(TYPE)));\
		}                                                                                          \
	} __PROPERTY_##NAME##__;                                                                       \
	TYPE NAME

#define FUNCTION(RETURN, NAME, PARAMS, DESCRIBTION)                                                \
	struct __FUNCTION_##NAME##__                                                                   \
	{                                                                                              \
		__FUNCTION_##NAME##__(void)                                                                \
		{                                                                                          \
			GetClass()->AddFunction(                                                               \
				#NAME,                                                                             \
				new Function(GetClass(), __create_function(&ClassType::NAME), #RETURN, #NAME, #PARAMS, DESCRIBTION)); \
		}                                                                                          \
	} __FUNCTION_##NAME##__;                                                                       \
	RETURN NAME PARAMS

#define VIRTUAL_FUNCTION(RETURN, NAME, PARAMS, DESCRIBTION)                                        \
	struct __FUNCTION_##NAME##__                                                                   \
	{                                                                                              \
		__FUNCTION_##NAME##__(void)                                                                \
		{                                                                                          \
			GetClass()->AddFunction(                                                               \
				#NAME,                                                                             \
				new Function(GetClass(), __create_function(&ClassType::NAME), #RETURN, #NAME, #PARAMS, DESCRIBTION)); \
		}                                                                                          \
	} __FUNCTION_##NAME##__;                                                                       \
	virtual RETURN NAME PARAMS


DEFINE_FUNCTION(0);
DEFINE_FUNCTION(1);
DEFINE_FUNCTION(2);
DEFINE_FUNCTION(3);
DEFINE_FUNCTION(4);
DEFINE_FUNCTION(5);
DEFINE_FUNCTION(6);
DEFINE_FUNCTION(7);
DEFINE_FUNCTION(8);
DEFINE_FUNCTION(9);
DEFINE_FUNCTION_CONST(0);
DEFINE_FUNCTION_CONST(1);
DEFINE_FUNCTION_CONST(2);
DEFINE_FUNCTION_CONST(3);
DEFINE_FUNCTION_CONST(4);
DEFINE_FUNCTION_CONST(5);
DEFINE_FUNCTION_CONST(6);
DEFINE_FUNCTION_CONST(7);
DEFINE_FUNCTION_CONST(8);
DEFINE_FUNCTION_CONST(9);
DEFINE_VOID_FUNCTION(0);
DEFINE_VOID_FUNCTION(1);
DEFINE_VOID_FUNCTION(2);
DEFINE_VOID_FUNCTION(3);
DEFINE_VOID_FUNCTION(4);
DEFINE_VOID_FUNCTION(5);
DEFINE_VOID_FUNCTION(6);
DEFINE_VOID_FUNCTION(7);
DEFINE_VOID_FUNCTION(8);
DEFINE_VOID_FUNCTION(9);
DEFINE_VOID_FUNCTION_CONST(0);
DEFINE_VOID_FUNCTION_CONST(1);
DEFINE_VOID_FUNCTION_CONST(2);
DEFINE_VOID_FUNCTION_CONST(3);
DEFINE_VOID_FUNCTION_CONST(4);
DEFINE_VOID_FUNCTION_CONST(5);
DEFINE_VOID_FUNCTION_CONST(6);
DEFINE_VOID_FUNCTION_CONST(7);
DEFINE_VOID_FUNCTION_CONST(8);
DEFINE_VOID_FUNCTION_CONST(9);
CREATE_FUNCTION(0);
CREATE_FUNCTION(1);
CREATE_FUNCTION(2);
CREATE_FUNCTION(3);
CREATE_FUNCTION(4);
CREATE_FUNCTION(5);
CREATE_FUNCTION(6);
CREATE_FUNCTION(7);
CREATE_FUNCTION(8);
CREATE_FUNCTION(9);
CREATE_FUNCTION_CONST(0);
CREATE_FUNCTION_CONST(1);
CREATE_FUNCTION_CONST(2);
CREATE_FUNCTION_CONST(3);
CREATE_FUNCTION_CONST(4);
CREATE_FUNCTION_CONST(5);
CREATE_FUNCTION_CONST(6);
CREATE_FUNCTION_CONST(7);
CREATE_FUNCTION_CONST(8);
CREATE_FUNCTION_CONST(9);
CREATE_VOID_FUNCTION(0);
CREATE_VOID_FUNCTION(1);
CREATE_VOID_FUNCTION(2);
CREATE_VOID_FUNCTION(3);
CREATE_VOID_FUNCTION(4);
CREATE_VOID_FUNCTION(5);
CREATE_VOID_FUNCTION(6);
CREATE_VOID_FUNCTION(7);
CREATE_VOID_FUNCTION(8);
CREATE_VOID_FUNCTION(9);
CREATE_VOID_FUNCTION_CONST(0);
CREATE_VOID_FUNCTION_CONST(1);
CREATE_VOID_FUNCTION_CONST(2);
CREATE_VOID_FUNCTION_CONST(3);
CREATE_VOID_FUNCTION_CONST(4);
CREATE_VOID_FUNCTION_CONST(5);
CREATE_VOID_FUNCTION_CONST(6);
CREATE_VOID_FUNCTION_CONST(7);
CREATE_VOID_FUNCTION_CONST(8);
CREATE_VOID_FUNCTION_CONST(9);


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
	Function(Class *pClass, __function_base *pFunction, const char *ret, const char *name, const char *params, const char *describtion)
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

	INVOKE(0);
	INVOKE(1);
	INVOKE(2);
	INVOKE(3);
	INVOKE(4);
	INVOKE(5);
	INVOKE(6);
	INVOKE(7);
	INVOKE(8);
	INVOKE(9);
	INVOKE_VOID(0);
	INVOKE_VOID(1);
	INVOKE_VOID(2);
	INVOKE_VOID(3);
	INVOKE_VOID(4);
	INVOKE_VOID(5);
	INVOKE_VOID(6);
	INVOKE_VOID(7);
	INVOKE_VOID(8);
	INVOKE_VOID(9);

private:
	std::string m_name;
	std::string m_return;
	std::string m_params;
	std::string m_describtion;

private:
	Class *m_pClass;
	__function_base *m_pFunction;
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

	void AddProperty(const char *name, Property  *pProperty)
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

extern void SetRelfectionClass(const char *name, Class *pClass);
extern Class* GetRelfectionClass(const char *name);
