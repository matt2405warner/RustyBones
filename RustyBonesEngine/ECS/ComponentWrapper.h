
namespace rusty
{
	template <class T>
	class ComponentWrapper
	{
		friend class EntityManager;
	private:
		T * m_component;

	public:
		ComponentWrapper() : m_component(nullptr) {}
		T* operator->();
		const T* operator->() const;

		T& operator*();
		const T& operator*() const;

		bool is_valid() const;
		operator bool() const;
		bool operator==(const ComponentWrapper<T>& other) const;
		bool operator!=(const ComponentWrapper<T>& other) const;

	private:
		ComponentWrapper(T* component) : m_component(component) {}
	};
	template<class T>
	inline T * ComponentWrapper<T>::operator->()
	{
		return m_component;
	}
	template<class T>
	inline const T * ComponentWrapper<T>::operator->() const
	{
		return m_component;
	}
	template<class T>
	inline T & ComponentWrapper<T>::operator*()
	{
		return *m_component;
	}
	template<class T>
	inline const T & ComponentWrapper<T>::operator*() const
	{
		return *m_component;
	}
	template<class T>
	inline bool ComponentWrapper<T>::is_valid() const
	{
		return m_component != nullptr;
	}
	template<class T>
	inline ComponentWrapper<T>::operator bool() const
	{
		return m_component != nullptr;
	}
	template<class T>
	inline bool ComponentWrapper<T>::operator==(const ComponentWrapper<T>& other) const
	{
		return m_component == other.m_component;
	}
	template<class T>
	inline bool ComponentWrapper<T>::operator!=(const ComponentWrapper<T>& other) const
	{
		return !(*this == other);
	}
}