//
// Created by zhaohaibin on 1/31/18.
//

#include "TaskRuner.h"
#include "../Configure.h"
#include <boost/bind.hpp>

TaskRuner::TaskRuner(deque<shared_ptr<Task>> tasks,
                     TaskStatusDelegate* pTaskStatusDelegate)
        : m_tasks(tasks)
        , m_pTaskStatusDelegate(pTaskStatusDelegate)
        , m_maxThreadCount(2)
{
    m_maxThreadCount = Configure::Instance()->GetTaskRunnerThreadCount();
}

TaskRuner::~TaskRuner()
{

}

void TaskRuner::Run()
{
    boost::thread_group threadGroup;
    for(int i = 0; i < m_maxThreadCount; ++i)
    {
        threadGroup.create_thread(boost::bind(&TaskRuner::JobHandler, this));
    }
    threadGroup.join_all();
    AllTaskComplete();
}

shared_ptr<Task> TaskRuner::GetTask()
{
    boost::mutex::scoped_lock lock(m_mutex);
    shared_ptr<Task> spTask;
    if(m_tasks.size() > 0)
    {
        spTask = m_tasks.front();
        m_tasks.pop_front();
    }
    return spTask;
}

void TaskRuner::JobHandler()
{
    while (true)
    {
        shared_ptr<Task> spTask = GetTask();
        if(spTask)
        {
            DoHandle(spTask);
            OnTaskComplete(spTask);
        }
        else
        {
            break;
        }
    }
}

void TaskRuner::DoHandle(shared_ptr<Task> spTask)
{
    m_pTaskStatusDelegate->OnStarted(spTask);
    spTask->Run();
    m_pTaskStatusDelegate->OnComplete(spTask);
}

void TaskRuner::OnTaskComplete(shared_ptr<Task> spTask)
{

}

void TaskRuner::AllTaskComplete()
{

}
