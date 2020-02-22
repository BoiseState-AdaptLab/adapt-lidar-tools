#ifndef ADAPTLIDAR_TASKTHREAD_HPP
#define ADAPTLIDAR_TASKTHREAD_HPP

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

template<typename ReturnValue>
class TaskThread{
public:
    TaskThread(){
        thread = std::thread(&TaskThread::runQueue, this);
    }

    TaskThread(const TaskThread&) = delete; //We don't want to copy this
    TaskThread(TaskThread&& obj) = delete;  //Move construction is tricky and not needed for this
    TaskThread operator=(const TaskThread&) = delete; //No copy-assignment
    TaskThread operator=(TaskThread&& obj) = delete; //No move assignment


    ~TaskThread(){
        {
            std::unique_lock<std::mutex> lock(mutex);
            done = true;
        }

        conditionVar.notify_one();  //Signal thread to stop
        thread.join();
    }

    void queueTask(std::packaged_task<ReturnValue()>&& task){
        {
            std::unique_lock<std::mutex> lock(mutex);
            taskQueue.push(std::move(task));
        }
        conditionVar.notify_one();  //Signal thread that new task is available
    }

    void queueMultipleTasks(std::vector<std::packaged_task<ReturnValue()>>&& tasks){
        {
            std::unique_lock<std::mutex> lock(mutex);
            for(auto& task : tasks){
                taskQueue.push(std::move(task));
            }
        }
        conditionVar.notify_one();  //Signal thread. We don't have to signal it multiple times, as the wait condition will eval to true.
    }

private:
    void runQueue(){
        while(true){
            std::packaged_task<ReturnValue()> task;
            {
                std::unique_lock<std::mutex> lock(mutex);
                //Note that the below checks the condition before releasing the mutex, so we incur no overhead from repeatedly checking it
                conditionVar.wait(lock, [&](){return !taskQueue.empty() || done;}); //Wait until we have a task or we are done.

                if(taskQueue.empty()) return; //If this is true, done is true. Note that if done is true but queue not empty, we'll finish processing first.

                task = std::move(taskQueue.front());  //Move the task out of the queue
                taskQueue.pop();    //Remove task
            }

            task(); //Run task
        }
    }

    std::queue<std::packaged_task<ReturnValue()>> taskQueue;
    std::thread thread;
    bool done = false; //Used to tell when we are done processing elements
    std::mutex mutex;
    std::condition_variable conditionVar;
};
#endif // ADAPTLIDAR_TASKTHREAD_HPP
