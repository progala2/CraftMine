#ifndef _XKS_WQUEUE_H_
#define _XKS_WQUEUE_H_
#include "stdafx.h"

	template<class T>
	class WQueue
	{
	public:
		WQueue() {}
		~WQueue() {}
		void Add(T& obj) 
		{
			m_mtx.lock();
			m_queue.push(obj);
			m_mtx.unlock();
		}
		T& Remove() 
		{
			m_mtx.lock();
			T& obj = m_queue.front();
			m_queue.pop();
			m_mtx.unlock();
			return obj;
		}
		int Size() {
			m_mtx.lock();
			int size = m_queue.size();
			m_mtx.unlock();
			return size;
		}

	private:
		std::mutex m_mtx;
		std::queue<T> m_queue;
	};
#endif