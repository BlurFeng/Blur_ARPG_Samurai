#pragma once

namespace WeakObjectPtrExtensions
{
	template <class T>
	static void Add(TArray<TWeakObjectPtr<T>>& WeakObjectPtrArray, T* Object)
	{
		WeakObjectPtrArray.Add(TWeakObjectPtr<T>(Object));
	}
	
	template <class T>
	static bool Equals(const TWeakObjectPtr<T>& WeakObjectPtr, T* Object)
	{
		return WeakObjectPtr.IsValid() && WeakObjectPtr.Get() == Object;
	}

	template <class T>
	static bool Contains(const TArray<TWeakObjectPtr<T>>& WeakObjectPtrArray, T* Object)
	{
		for (const TWeakObjectPtr<T>& WeakObjectPtr : WeakObjectPtrArray)
		{
			if (Equals(WeakObjectPtr, Object))
			{
				return true;
			}
		}

		return false;
	}
}