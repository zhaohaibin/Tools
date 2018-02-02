//
// Created by zhaohaibin on 1/31/18.
//

#ifndef IMAGEMATCHER_TASKRUNER_H
#define IMAGEMATCHER_TASKRUNER_H

#include <deque>
#include <memory>
#include <boost/thread.hpp>
#include "Task.h"

using namespace std;

class TaskRuner;
class TaskStatusDelegate
{
public:
    friend class TaskRuner;
public:
    inline TaskStatusDelegate(){}
    inline ~TaskStatusDelegate(){}

protected:
    virtual void OnStarted(std::shared_ptr<Task> spTask) = 0;
    virtual void OnComplete(std::shared_ptr<Task> spTask) = 0;

};

class TaskRuner
{
public:
    TaskRuner(deque<std::shared_ptr<Task>> tasks, TaskStatusDelegate* pTaskStatusDelegate);
    virtual ~TaskRuner();

    void Run();

private:
    void JobHandler();
    std::shared_ptr<Task> GetTask();

protected:
    virtual void DoHandle(std::shared_ptr<Task> spTask);
    virtual void OnTaskComplete(std::shared_ptr<Task> spTask);
    virtual void AllTaskComplete();
private:
    TaskStatusDelegate* m_pTaskStatusDelegate;
    deque<std::shared_ptr<Task>> m_tasks;
    boost::mutex m_mutex;
    int m_maxThreadCount;
};


#endif //IMAGEMATCHER_TASKRUNER_H
