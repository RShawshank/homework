#include "threadpool.h"
#include <process.h>

ThreadPool::ThreadPool(size_t minNumOfThread, size_t maxNumOfThread)
{
	if (minNumOfThread<2)
	{
		this->minNumOfThread = 2;
	}
	else {
		
		this->minNumOfThread = minNumOfThread;
	}
	if (maxNumOfThread<this->minNumOfThread*2)
	{
		maxNumOfThread = this->minNumOfThread * 2;
	}
	else {
		
		this->maxNumOfThread = maxNumOfThread;
	}
	stopEvent = CreateEvent(NULL, true, false, NULL);
	//创建一个完成端口
	completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL,0, 1);
	IdleThreadList.clear();
	CreateIdleThread(this->minNumOfThread);
	BusyThreadList.clear();
	dispatchThread = (HANDLE)_beginthreadex(0, 0, GetTaskThreadProc, this, 0, 0);
	NumOfLongFun = 0;
}

ThreadPool::~ThreadPool()
{
	SetEvent(stopEvent);
	PostQueuedCompletionStatus(completionPort, 0, (DWORD)EXIT, NULL);
	CloseHandle(stopEvent);
}
ThreadPool::Thread *ThreadPool::GetIdleThread()
{
	Thread* thread = NULL;
	IdleThreadLock.Lock();
	//从空闲线程中取出一个线程
	if(IdleThreadList.size()>0)
	{
		thread = IdleThreadList.front();
		IdleThreadList.pop_front();
	}
	IdleThreadLock.UnLock();
	//线程池中的线程数没满最大线程数，就创建一个线程
	if(thread ==NULL&&getCurNumOfThread()<maxNumOfThread)
	{
		thread = new Thread(this);
	}
	if(thread==NULL&&waitTaskList.size()>THRESHOLE_OF_WAIT_TASK)
	{
		thread = new Thread(this);
		InterlockedIncrement(&maxNumOfThread);
	}
	return thread;
}
//创建空闲队列
void ThreadPool::CreateIdleThread(size_t size)
{
	IdleThreadLock.Lock();
	for (size_t i=0;i<size;i++)
	{
		IdleThreadList.push_back(new Thread(this));
	}
	IdleThreadLock.UnLock();
}
//清空空闲队列
void ThreadPool::DeleteIdleThread(size_t size)
{
	IdleThreadLock.Lock();
	size_t t = IdleThreadList.size();
	if (size >= t)
	{
		for (size_t i = 0; i < t; i++)
		{
			auto thread = IdleThreadList.back();
			delete thread;
			IdleThreadList.pop_back();
		}
	}
	else
	{
		for (size_t i = 0; i < size; i++)
		{
			auto thread = IdleThreadList.back();
			delete thread;
			IdleThreadList.pop_back();
		}
	}

	IdleThreadLock.UnLock();
}


ThreadPool::WaitTask *ThreadPool::GetTask()
{
	WaitTask* wait_task = NULL;
	WaitTaskLock.Lock();
	if (waitTaskList.size()>0)
	{
		wait_task = waitTaskList.front();
		waitTaskList.pop_front();
	}
	WaitTaskLock.UnLock();
	return wait_task;
}
void ThreadPool::MoveBusyThreadToIdleList(Thread* busyThread)
{
	BusyThreadLock.Lock();
	BusyThreadList.push_back(busyThread);
	BusyThreadLock.UnLock();
}
void ThreadPool::MoveThreadToBusyList(Thread* thread)
{
	
}
void ThreadPool::Thread::ExecuteTask(TaskFun task, PVOID param, TaskCallbackFun taskcallbackfun)
{
	
}


void ThreadPool::GetTaskExcute()
{
	Thread* thread = NULL;
	WaitTask* wait_task = NULL;
	wait_task = GetTask();
	if (wait_task==NULL)
	{
		return;
	}
	if (wait_task->bLong)
	{
		if (IdleThreadList.size()>minNumOfThread)
		{
			thread = GetIdleThread();
		}
		else
		{
			thread = new Thread(this);
			InterlockedIncrement(&NumOfLongFun);
			InterlockedIncrement(&maxNumOfThread);
		}
	}
	else
	{
		thread = GetIdleThread();
	}
	if (thread==NULL)
	{
		thread->ExecuteTask(wait_task->task,wait_task->param, wait_task->taskCb);
		delete wait_task;
		MoveThreadToBusyList(thread);
	}
	else
	{
		WaitTaskLock.Lock();
		waitTaskList.push_front(wait_task);
		WaitTaskLock.UnLock();
	}
	
	
}


BOOL ThreadPool::QueueTaskItem(TaskFun TASK, PVOID param, TaskCallbackFun TASKCB, BOOL longFun)
{
	WaitTaskLock.Lock();
	WaitTask* wait_task = new WaitTask(TASK, param, TASKCB, longFun);
	waitTaskList.push_back(wait_task);
	WaitTaskLock.UnLock();
	PostQueuedCompletionStatus(completionPort, 0, (DWORD)GET_TASK, NULL);
	return true;
}
unsigned ThreadPool::Thread::ThreadProc(PVOID PM)
{
	Thread* thread = (Thread*)PM;
	while(true)
	{
		if (thread->exit)
		{
			break;
		}
		if (thread->task==NULL&&thread->taskcb==NULL)
		{
			thread->busy = FALSE;
			thread->threadPool->MoveBusyThreadToIdleList(thread);
			SuspendThread(thread->thread);//把线程挂起
			continue;
		}
		int result = thread->task(thread->param);
	}
	return 0;
}
ThreadPool::Thread::Thread(ThreadPool* threadPool) :
	busy(FALSE),
	thread(INVALID_HANDLE_VALUE),//类似NULL
	task(NULL),
	taskcb(NULL),
	exit(FALSE),
	threadPool(threadPool)
{
	thread =(HANDLE) _beginthreadex(0, 0, ThreadProc, this, CREATE_SUSPENDED, 0);
}

BOOL ThreadPool::Thread::isBusy()
{
	return busy;
}
ThreadPool::Thread::~Thread()
{
	exit = TRUE;
	taskcb = NULL;
	task = NULL;
	ResumeThread(thread);
	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);
}



