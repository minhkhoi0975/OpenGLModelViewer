#pragma once
#include <functional>
#include <list>

template<typename... Args>
class MulticastDelegate
{
public:
	/// <summary>
	/// Binds a function to the delegate. Returns the iterator to the function.
	/// </summary>
	typename std::list<std::function<void(Args...)>>::iterator Bind(std::function<void(Args...)> function);

	/// <summary>
	/// Uses an iterator to unbind a function from the delegate.
	/// </summary>
	/// <param name="iterator"></param>
	void Unbind(typename std::list<std::function<void(Args...)>>::iterator iterator);

	/// <summary>
	/// Remove all functions from the delegate.
	/// </summary>
	void Clear();

	/// <summary>
	/// Invokes the delegate.
	/// </summary>
	void Broadcast(Args...);

private:
	std::list<std::function<void(Args...)>> functions;
};

template<typename ...Args>
inline typename std::list<std::function<void(Args...)>>::iterator MulticastDelegate<Args...>::Bind(std::function<void(Args...)> function)
{
	typename std::list<std::function<void(Args...)>>::iterator itr = functions.insert(functions.end(), function);
	return itr;
}

template<typename ...Args>
inline void MulticastDelegate<Args...>::Unbind(typename std::list<std::function<void(Args...)>>::iterator iterator)
{
	if (functions.size() == 0)
		return;

	functions.erase(iterator);
}

template<typename ...Args>
inline void MulticastDelegate<Args...>::Clear()
{
	functions.clear();
}

template<typename ...Args>
inline void MulticastDelegate<Args...>::Broadcast(Args... args)
{
	for (auto itr = functions.begin(); itr != functions.end(); ++itr)
		(*itr)(args...);
}
