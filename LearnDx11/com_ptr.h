#pragma once

template <typename Type>
class com_ptr
{
	typedef Type* PtrType;
	typedef com_ptr<Type> ThisType;
public:
	com_ptr() : _ptr(nullptr) {}
	com_ptr(PtrType p) : _ptr(p)
	{
		if (p) p->AddRef();
	}
	com_ptr(ThisType&& other)
	{
		_ptr = other._ptr;
		other._ptr = nullptr;
	}
	template <typename TUnknown>
	com_ptr(TUnknown* p) : _ptr(nullptr)
	{
		if (p)
			p->QueryInterface(__uuidof(Type), (void**)&_ptr);
	}
	template <typename TUnknown>
	com_ptr(const com_ptr<TUnknown>& ptr) : _ptr(nullptr)
	{
		if (ptr)
			ptr->QueryInterface(__uuidof(Type), (void**)&_ptr);
	}
	~com_ptr() { clear(); }

	void clear()
	{
		if (_ptr) _ptr->Release();
		_ptr = nullptr;
	}
	void attach(PtrType p) { _ptr = p; }
	PtrType deatch()
	{
		PtrType p = _ptr;
		_ptr = nullptr;
		return p
	}

	operator PtrType () { return _ptr; }
	operator const PtrType () const { return _ptr; }
	PtrType operator-> () { return _ptr; }
	const PtrType operator-> () const { return _ptr; }
	PtrType* operator& () { return &_ptr; }
	operator bool() const { return (nullptr != _ptr); }
	bool operator== (PtrType p) const { return (p == _ptr); }
	bool operator!= (PtrType p) const { return (p != _ptr); }
	void operator= (PtrType* p)
	{
		if (_ptr) _ptr->Release();
		_ptr = p;
		if (_ptr) _ptr->AddRef();
	}
	void operator= (ThisType&& other)
	{
		std::swap(_ptr, other._ptr);
	}
	template <typename TUnknown>
	void operator= (TUnknown* p)
	{
		clear();
		if (p)
			p->QueryInterface(__uuidof(Type), (void**)&_ptr);
	}

private:
	PtrType _ptr;
};

