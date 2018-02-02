//
// Created by zhaohaibin on 1/31/18.
//

#ifndef IMAGEMATCHER_TASK_H
#define IMAGEMATCHER_TASK_H

#include <memory>
#include <boost/any.hpp>

using namespace std;

enum TaskType
{
    TASK_TYPE_MODIFY_BACKGROUD_COLOR,
    TASK_TYPE_GENERATE_THUMBNIAL,
    TASK_TYPE_IMAGE_MATCH
};

class Task
{
public:
    inline Task(TaskType taskType):
            m_taskType(taskType)
    {};

    inline virtual ~Task(){};
public:
    inline TaskType Type()
    {
        return m_taskType;
    }
public:
    virtual bool Run() = 0;
    inline virtual void GetResult(boost::any result){};

private:
    TaskType m_taskType;
};


#endif //IMAGEMATCHER_TASK_H
