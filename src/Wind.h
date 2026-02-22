#pragma once

#include "PluginAPI.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

namespace wind
{
	class Config;

	class Wind final : 
		public hdt::IPreStepListener
	{
	private:
		class ThreadPool
		{
		public:
			static constexpr int MAX_THREADS = 128;

			ThreadPool(int count);
			~ThreadPool();
			
			int  size() const { return (int)m_threads.size(); }
			void release(Wind* target);
			void wait() { m_barrier.arrive_and_wait(); }

		private:
			void worker();
			ThreadPool(const ThreadPool&) = delete;
			ThreadPool& operator=(const ThreadPool&) = delete;
		private:
			std::vector<std::thread>             m_threads;
			std::counting_semaphore<MAX_THREADS> m_signal{ 0 };
			std::barrier<>                       m_barrier;
			Wind*                                m_target{ nullptr };
		};
	public:
		Wind() = default;
		~Wind() = default;

		// override (hdt::IPreStepListener)
		RE::BSEventNotifyControl ProcessEvent(const hdt::PreStepEvent* a_event, RE::BSTEventSource<hdt::PreStepEvent>* a_eventSource) override;
		
		void init(const Config& config);
		void updateThreadCount();
	private:
		btVector3 eval(const btVector3& at);
		void process(btCollisionObject* object);
		void processThreadsafe();
	private:
		const RE::Sky*                                  m_sky{ nullptr };
		const Config*                                   m_config{ nullptr };
		float                                           m_currentTime{ 0.0f };
		btVector3                                       m_currentDir;
		btVector3                                       m_orthoDir;
		std::unique_ptr<ThreadPool>                     m_threadPool;
		const btAlignedObjectArray<btCollisionObject*>* m_objArr{ nullptr };
		std::atomic<int>                                m_nextElement{ -1 };
	};
}
