#include <framework/marshal/marshal.hpp>
#include <string>
#include <iostream>

using namespace v8;

std::string ToString(size_t value)
{
    std::ostringstream str;
    str << value;
    return str.str();
}

NodeObjectProperty::NodeObjectProperty(Persistent<Object> parent, const std::string& propertyName)
	: m_parent(parent)
	, m_propertyName(propertyName)
{

}

NodeObjectProperty::~NodeObjectProperty()
{
	m_parent.Dispose();
}

NodeObjectProperty NodeObjectProperty::operator[](const std::string& propertyName)
{
	NanScope();

	Local<Value> prop = m_parent->Get(String::NewSymbol(m_propertyName.c_str()));

	bool propertyExistsAndIsObject = prop->IsObject();

	Persistent<Object> target;

	if (!propertyExistsAndIsObject)
	{
		target = Persistent<Object>::New(Object::New());
		m_parent->Set(v8::String::NewSymbol(m_propertyName.c_str()), target);
	}
	else
	{
		target = Persistent<Object>::New( Local<Object>::Cast(m_parent->Get(String::NewSymbol(m_propertyName.c_str()))));
	}

	return NodeObjectProperty(target, propertyName);
}

NodeObjectProperty NodeObjectProperty::operator[](size_t propertyIdx)
{
    return this->operator[](ToString(propertyIdx));
}

NodeObject::NodeObject(Local<Object>& target)
	: m_target(target)
{

}

NodeObjectProperty NodeObject::operator[](const std::string& propertyName)
{
	return NodeObjectProperty(Persistent<Object>::New(m_target), propertyName);
}

NodeObjectProperty NodeObject::operator[](size_t propertyIdx)
{
    return NodeObjectProperty(Persistent<Object>::New(m_target), ToString(propertyIdx));
}
