#ifndef THREADPOOL_H
#define THREADPOOL_H
#include<exception>
#include<windows.h>
#include <list>
#include <queue>
#include <memory>
using namespace std;
#define THRESHOLE_OF_WAIT_TASK 20
typedef int(*TaskFun)(PVOID param);//任务函数
typedef void(*TaskCallbackFun)(int result);//回调函数
class ThreadPool
{

private:
	class Thread
	{
	public: Thread(ThreadPool* threadPool);
		  ~Thread();
		  BOOL isBusy();
		  void ExecuteTask(TaskFun task, PVOID param, TaskCallbackFun taskcallbackfun);//执行任务
	private:
		ThreadPool* threadPool;//所属线程池
		BOOL busy;//任务是否在执行
		BOOL exit;//是否退出
		HANDLE thread;//线程句柄
		TaskFun task;//待执行的任务
		PVOID param;//任务参数
		TaskCallbackFun taskcb;//回调的任务
		static unsigned int __stdcall ThreadProc(PVOID PM);//线程函数

	};
	//线程临界锁
	class CriticalSectionLock
	{
	private:
		CRITICAL_SECTION  CS;//临界区
	public:
		CriticalSectionLock()
		{
			InitializeCriticalSection(&CS);
		}
		~CriticalSectionLock()
		{
			DeleteCriticalSection(&CS);
		}
		void Lock()
		{
			EnterCriticalSection(&CS);
		}
		void UnLock()
		{
			LeaveCriticalSection(&CS);
		}
	};
	class WaitTask
	{
	public:
		TaskFun    task;                    // 要执行的任务
		PVOID      param;                    // 任务参数
		TaskCallbackFun taskCb;            // 回调的任务
		BOOL        bLong;                        // 是否时长任务
		WaitTask(TaskFun TASK, PVOID param, TaskCallbackFun TASKCB, BOOL LONG)
		{
			this->bLong = LONG;
			this->param = param;
			this->task = TASK;
			this->taskCb = TASKCB;
		}
		~WaitTask()
		{
			task = NULL;
			taskCb = NULL;
			bLong = FALSE;
			param = NULL;
		}
	};

	HANDLE dispatchThread;//分发任务线程
	HANDLE stopEvent;//通知线程退出的时间
	HANDLE completionPort;//完成端口
	size_t maxNumOfThread;//线程池中最大的线程数
	size_t minNumOfThread;//线程池中最小的线程数
	size_t NumOfLongFun;
	CriticalSectionLock IdleThreadLock;//空闲线程列表锁
	list<Thread*> IdleThreadList;//空闲线程列表
	CriticalSectionLock BusyThreadLock;//忙碌线程列表锁
	list<Thread*>BusyThreadList;//忙碌线程列表
	CriticalSectionLock WaitTaskLock;
	list<WaitTask*> waitTaskList;//任务列表

	size_t getIdleThreadNum()
	{
		return IdleThreadList.size();
	}

	size_t getBusyThreadNum()
	{
		return BusyThreadList.size();
	}

	size_t getCurNumOfThread()
	{
		return getBusyThreadNum() + getIdleThreadNum();
	}

	size_t getMinNumOfThread()
	{
		return minNumOfThread;
	}

	size_t getMaxNumOfThread()
	{
		return maxNumOfThread - NumOfLongFun;
	}

	void SetMaxNumOfThread(size_t size)
	{
		if (size < NumOfLongFun)
		{
			this->maxNumOfThread = size+NumOfLongFun;
		}
		else
		{
			maxNumOfThread = size;
		}
	}
	void SetMinNumOfThread(size_t size)
	{
		this->minNumOfThread = size;
	}

	//创建空闲线程
	void CreateIdleThread(size_t size);
	void DeleteIdleThread(size_t size);
	Thread* GetIdleThread();
	//将忙碌线程加入到空闲线程中
	void MoveBusyThreadToIdleList(Thread* busyThread);
	//将线程加入忙碌线程中
	void MoveThreadToBusyList(Thread* thread);
	//从任务队列中取得任务
	void GetTaskExcute();
	WaitTask* GetTask();
	
	enum WAIT_OPERATION_TYPE
	{
		GET_TASK,EXIT
	};
	
	//从任务列表中取任务的线程函数
	static unsigned int __stdcall GetTaskThreadProc(PVOID PM)
	{
		ThreadPool* threadPool = (ThreadPool*)PM;
		BOOL bRet = FALSE;
		DWORD dwbytes = 0;
		WAIT_OPERATION_TYPE OpType;
		OVERLAPPED* ol;
		while(WAIT_OBJECT_0!=WaitForSingleObject(threadPool->stopEvent,0))
		{
			BOOL bRet = GetQueuedCompletionStatus(threadPool->completionPort, &dwbytes, (PULONG_PTR)&OpType, &ol, INFINITE);
			//收到退出标志
			if (EXIT == (DWORD)OpType)
				break;
			else if (GET_TASK == (DWORD)OpType)
				threadPool->GetTaskExcute();
		}
		return 0;
	}
public:
	ThreadPool(size_t minNumOfThread=2, size_t maxNumOfThread=10);
	~ThreadPool();
	BOOL QueueTaskItem(TaskFun TASK, PVOID param, TaskCallbackFun TASKCB = NULL, BOOL longFun = false);
};

#endif
