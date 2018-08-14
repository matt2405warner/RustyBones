#pragma once

#include <vector>
#include <cassert>

class BasePool
{
public:
	virtual ~BasePool() = default;

	virtual std::size_t size() const = 0;
	virtual std::size_t capacity() const = 0;

	virtual void resize(std::size_t n) = 0;
	virtual void reserve(std::size_t n) = 0;
	virtual void destroy(std::size_t index) = 0;
	virtual void* get(std::size_t index) = 0;
	virtual const void* get(std::size_t index) const = 0;
};

template <class T>
class Pool : public BasePool
{
protected:
	std::vector<T> m_pool;

public:
	Pool() = default;
	explicit Pool(std::size_t size);
	std::size_t size() const override;
	std::size_t capacity() const override;

	void resize(std::size_t n) override;
	void reserve(std::size_t n) override;
	void destroy(std::size_t index) override;

	const void* get(std::size_t index) const override;
	void* get(std::size_t index) override;
};

template<class T>
std::size_t Pool<T>::size() const
{
	return m_pool.size();
}

template<class T>
std::size_t Pool<T>::capacity() const
{
	return m_pool.capacity();
}

template<class T>
void Pool<T>::resize(std::size_t n)
{
	m_pool.resize(n);
}

template<class T>
void Pool<T>::reserve(std::size_t n)
{
	m_pool.reserve(n);
}

template<class T>
void Pool<T>::destroy(std::size_t index)
{
	assert(index >= 0 && index < m_pool.size());
	m_pool[index].~T();
}

template<class T>
const void* Pool<T>::get(std::size_t index) const
{
	assert(index >= 0 && index < m_pool.size());
	return &m_pool[index];
}

template<class T>
void *Pool<T>::get(std::size_t index)
{
	assert(index >= 0 && index < m_pool.size());
	return &m_pool[index];
}

template<class T>
Pool<T>::Pool(std::size_t size)
	: m_pool(size)
{

}

template <class T>
class SlotPool : public Pool<T>
{
protected:
	std::vector<std::size_t> m_free_list;

public:
	explicit SlotPool(std::size_t size);
	std::pair<T*, std::size_t> create();
	void destroy(std::size_t index) override;
	T* get_element(std::size_t index) const;

private:
	std::size_t  get_next_internal();
};

template<class T>
std::size_t SlotPool<T>::get_next_internal()
{
	std::size_t index = 0;
	if (!m_free_list.empty())
	{
		index = m_free_list[0];
		m_free_list[0] = m_free_list[this->size() - 1];
		m_free_list.pop_back();
	}
	else {
		index = this->size();
		this->m_pool.resize(this->size() + 1);
	}

	return index;
}

template<class T>
SlotPool<T>::SlotPool(std::size_t size)
	: Pool<T>(size)
{
	if (size > 0)
	{
		m_free_list.reserve(size);
		for (std::size_t i = 0; i < size; i++)
		{
			m_free_list.emplace_back(i);
		}
	}
}

template<class T>
inline std::pair<T*, std::size_t> SlotPool<T>::create()
{
	auto index = get_next_internal();
	::new(this->get(index)) T();
	return std::make_pair(&this->m_pool[index], index);
}

template<class T>
void SlotPool<T>::destroy(std::size_t index)
{
	Pool<T>::destroy(index);
	m_free_list.emplace_back(index);
}

template<class T>
inline T * SlotPool<T>::get_element(std::size_t index) const
{
	assert(index >= 0 && index < m_pool.size());
	return m_pool[index];
}
