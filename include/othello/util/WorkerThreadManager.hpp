#ifndef OTHELLO_UTIL_WORKERTHREADMANAGER_HPP
#define OTHELLO_UTIL_WORKERTHREADMANAGER_HPP

//Standard C++:
#include <thread>
#include <condition_variable>
#include <tuple>
#include <iostream>
#include <vector>
#include <atomic>
//Othello headers:
#include <othello/util/FutureValue.hpp>


namespace othello
{
    
    namespace util
    {
        
        ////////////////////////////////////////////////////////////////
        /// \class WorkerThreadManager
        ///
        /// \brief Nested class for managing the worker threads
        ///
        ////////////////////////////////////////////////////////////////
        template<class RetT, class... ArgsT>
        class WorkerThreadManager
        {
            private:
                
                ////////////////////////////////////////////////////////////////
                /// \class Nested class for managing a single worker thread
                ///
                ////////////////////////////////////////////////////////////////
                class Worker
                {
                    private:
        
                        ////////////////////////////////////////////////////////////////
                        /// \brief The thread itself
                        ///
                        ////////////////////////////////////////////////////////////////
                        std::thread thread;
        
        
                        ////////////////////////////////////////////////////////////////
                        /// \brief Mutex for protecting the member variables
                        ///
                        ////////////////////////////////////////////////////////////////
                        mutable std::mutex mutex;
                        
                        
                        ////////////////////////////////////////////////////////////////
                        /// \brief Condition variable for indicating when to work
                        ///
                        ////////////////////////////////////////////////////////////////
                        std::condition_variable waitForWork;
        
        
                        ////////////////////////////////////////////////////////////////
                        /// \brief Whether the worker thread should be running
                        ///
                        ////////////////////////////////////////////////////////////////
                        bool running = true;
                        
                        
                        ////////////////////////////////////////////////////////////////
                        /// \brief Whether the worker should be working
                        ///
                        ////////////////////////////////////////////////////////////////
                        std::atomic<bool> hasWork{false};
        
                        
                        ////////////////////////////////////////////////////////////////
                        /// \brief A tuple containing the arguments for the function
                        ///
                        ////////////////////////////////////////////////////////////////
                        boost::optional<
                                std::tuple<
                                        //If the argument type isn't a pointer
                                        typename std::enable_if<!std::is_pointer<ArgsT>::value,
                                                //Remove the const-ness and reference-ness
                                                typename std::remove_const<typename std::remove_reference<ArgsT>::type>::type
                                        >::type
                                        //Do that for all the arguments
                                        ...> > args;
        
        
                        ////////////////////////////////////////////////////////////////
                        /// \brief The future value to store the result of the work
                        ///
                        ////////////////////////////////////////////////////////////////
                        FutureValue<RetT> returnVal;
        
        
                        ////////////////////////////////////////////////////////////////
                        /// \brief Template function to call the given function with
                        ///        `args` and `returnVal`
                        ///
                        ////////////////////////////////////////////////////////////////
                        //Since std::apply isn't in C++14, in order to pass the
                        //arguments in the args tuple as parameters to the function
                        //we have to use std::index_sequence
                        //
                        //basically, here by dangerous template magicks
                        template<std::size_t... Is>
                        void callFunc(std::function<RetT(ArgsT...)> func,
                                std::index_sequence<Is...>)
                        {
                            returnVal.assign(func(std::get<Is>(args.get())...));
                        }
                        
                        
                    public:
        
                        ////////////////////////////////////////////////////////////////
                        /// \brief Class constructor
                        ///
                        ////////////////////////////////////////////////////////////////
                        explicit Worker(const std::function<RetT(ArgsT...)>& function)
                        {
                            //Start the thread
                            thread = std::thread(
                                    [this, function]()
                                    {
                                        //todo catch and deal with exceptions
                                        //Lock the mutex
                                        std::unique_lock<std::mutex> lock(mutex);
                                        
                                        //While the worker is running
                                        while (running)
                                        {
                                            //Wait for a job
                                            waitForWork.wait(lock, [this]() {return !running || hasWork;});
    
                                            //If there's no work, the worker woke up to die (so metal), so exit
                                            if (!hasWork) {return;}
    
                                            //Unlock the lock while we're working
                                            lock.unlock();
                                            
                                            try
                                            {
                                                //Do some work
                                                callFunc(function, std::index_sequence_for<ArgsT...>{});
                                            }
                                            catch (const std::exception& e)
                                            {
                                                std::cerr << "Exception caught in worker thread: " << e.what() << std::endl;
                                            }
                                            catch (...)
                                            {
                                                std::cerr << "Unknown exception caught in worker thread" << std::endl;
                                            }
                                            
                                            //Relock the lock
                                            lock.lock();
                                            
                                            //The work is complete
                                            hasWork = false;
                                        }
                                    });
                        }
        
        
                        ////////////////////////////////////////////////////////////////
                        /// \brief Class destructor for telling the thread to end
                        ///        properly
                        ///
                        ////////////////////////////////////////////////////////////////
                        ~Worker()
                        {
                            //If the thread is actually running
                            if (thread.joinable())
                            {
                                {
                                    //Lock the worker
                                    std::unique_lock<std::mutex> lock(mutex);
                                    //Change the running flag
                                    running = false;
                                    //Tell the worker to wake up
                                    waitForWork.notify_all();
                                }
                                //Wait for the thread
                                thread.join();
                            }
                            else
                            {
                                std::cerr << "Worker thread ended before destruction. "
                                             "Maybe an exception was thrown?" << std::endl;
                            }
                        }
                        
                        
                        ////////////////////////////////////////////////////////////////
                        /// \brief Function to determine whether the worker is available
                        ///        or if it's busy doing a job
                        ///
                        ////////////////////////////////////////////////////////////////
                        bool isAvailable() const
                        {
                            //Return if the worker doesn't have work
                            return !hasWork;
                        }
        
        
                        ////////////////////////////////////////////////////////////////
                        /// \brief Function to tell the worker to start the job
                        ///
                        ////////////////////////////////////////////////////////////////
                        void startJob(FutureValue<RetT> futureVal, ArgsT... arguments)
                        {
                            //Set the return value and arguments
                            returnVal = futureVal;
                            args.reset(std::make_tuple(arguments...));
                            {
                                //Lock the worker
                                std::unique_lock<std::mutex> lock(mutex);
                                //Start working
                                hasWork = true;
                            }
                            //Wake the worker up
                            waitForWork.notify_all();
                        }
                        
                };
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief A vector of the worker threads
                ///
                ////////////////////////////////////////////////////////////////
                std::vector<std::unique_ptr<Worker> > workers;
                //The workers are unique pointers because Worker isn't copyable
                //(since mutex, thread, condition var, etc. aren't copyable)
                //and apparently it's basically impossible to have a vector of
                //non-copyables
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief A mutex to protect the workers vector
                ///
                ////////////////////////////////////////////////////////////////
                mutable std::mutex mutex;
        
        
            public:
            
                ////////////////////////////////////////////////////////////////
                /// \brief Class constructor that specifies the number of worker
                ///        threads
                ///
                ////////////////////////////////////////////////////////////////
                explicit WorkerThreadManager(const std::function<RetT(ArgsT...)>& func, const uint8_t& numThreads)
                {
                    //Reserve space for the workers
                    workers.reserve(numThreads);
                    //Create the workers
                    for (uint8_t i = 0; i < numThreads; ++i) {workers.emplace_back(new Worker(func));}
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to lock the worker manager
                ///
                /// \note This function should be called and the lock should be
                ///       kept when calling any member functions
                ///
                ////////////////////////////////////////////////////////////////
                std::unique_lock<std::mutex> lock() const {return std::unique_lock<std::mutex>(mutex);}
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief Function to determine if a worker is available
                ///
                /// \note A lock should be obtained before calling this function
                ///
                ////////////////////////////////////////////////////////////////
                bool hasAvailableWorker() const
                {
                    //Iterate over the workers
                    for (const auto& worker : workers)
                    {
                        //No need to lock the worker, worker::hasWork
                        //is an atomic type so is thread-safe
                        
                        //If the worker is available, return true
                        if (worker->isAvailable()) {return true;}
                    }
                    return false;
                }
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief Function to tell a worker to do some work
                ///
                /// \note A lock should be obtained before calling this function
                ///
                /// \return A future value that will be updated when the work is
                ///         complete. If there are no workers available the
                ///         future value will never be updated
                ///
                ////////////////////////////////////////////////////////////////
                FutureValue<RetT> startWork(ArgsT... args)
                {
                    //Create an empty future value to return
                    FutureValue<RetT> ret(boost::none);
                    
                    //Iterate over the workers
                    for (auto& worker : workers)
                    {
                        //If the worker is available
                        if (worker->isAvailable())
                        {
                            //Start the job
                            worker->startJob(ret, args...);
                            //Exit
                            break;
                        }
                    }
                    //Return the future value
                    return ret;
                }
        
        };
    
    }
    
}

#endif //OTHELLO_UTIL_WORKERTHREADMANAGER_HPP
