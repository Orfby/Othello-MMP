#ifndef OTHELLO_UTIL_FUTUREVALUE_HPP
#define OTHELLO_UTIL_FUTUREVALUE_HPP

//Standard C++:
#include <mutex>
#include <memory>
//Boost headers:
#include <boost/optional.hpp> //Can't use std::optional as the project is
//C++14 (std::optional was introduced in C++17)


namespace othello
{
    
    namespace util
    {
    
        ////////////////////////////////////////////////////////////////
        /// \class FutureValue
        ///
        /// \brief Nested class representing a shared future value
        ///
        /// The class uses a shared pointer in order to share the value
        /// between copies of itself
        ///
        ////////////////////////////////////////////////////////////////
        template<class T>
        class FutureValue
        {
            private:
        
                ////////////////////////////////////////////////////////////////
                /// \struct SharedData
                ///
                /// \brief Struct containing the member variables that are
                ///        shared between std::shared_ptr instances
                ///
                ////////////////////////////////////////////////////////////////
                struct SharedData
                {
                    
                    ////////////////////////////////////////////////////////////////
                    /// \brief The value, or nothing if it doesn't exist yet
                    ///
                    ////////////////////////////////////////////////////////////////
                    boost::optional<T> value;
    
    
                    ////////////////////////////////////////////////////////////////
                    /// \brief A mutex to protect value
                    ///
                    ////////////////////////////////////////////////////////////////
                    mutable std::mutex mutex;
    
    
                    ////////////////////////////////////////////////////////////////
                    /// \brief Condition variable for waiting for the value to be
                    ///        assigned
                    ///
                    ////////////////////////////////////////////////////////////////
                    std::condition_variable waitForValue;
    
    
                    ////////////////////////////////////////////////////////////////
                    /// \brief Class constructor to initialise the value as nothing
                    ///
                    /// This doesn't initialise the value, so exists will return
                    /// false
                    ///
                    ////////////////////////////////////////////////////////////////
                    explicit SharedData(boost::none_t none) noexcept
                            : value(none) {}
    
    
                    ////////////////////////////////////////////////////////////////
                    /// \brief Class constructor to copy val onto the value
                    ///
                    ////////////////////////////////////////////////////////////////
                    explicit SharedData(const T& val) noexcept
                            : value(val) {}
    
    
                    ////////////////////////////////////////////////////////////////
                    /// \brief Class constructor to move val onto the value
                    ///
                    ////////////////////////////////////////////////////////////////
                    explicit SharedData(T&& val) noexcept
                    : value(val) {}
                    
                };
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief A shared pointer containing the data
                ///
                ////////////////////////////////////////////////////////////////
                std::shared_ptr<SharedData> ptr;
        
                
            public:
        
                ////////////////////////////////////////////////////////////////
                /// \brief Default class constructor
                ///
                /// This doesn't initialise the value, so exists will return
                /// false
                ///
                ////////////////////////////////////////////////////////////////
                FutureValue() noexcept = default;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Class constructor to initialise the value as nothing
                ///
                /// This doesn't initialise the value, so exists will return
                /// false
                ///
                ////////////////////////////////////////////////////////////////
                explicit FutureValue(boost::none_t none) noexcept
                    : ptr(new SharedData(none)) {}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Class constructor to copy val onto the value
                ///
                ////////////////////////////////////////////////////////////////
                explicit FutureValue(const T& val) noexcept
                        : ptr(new SharedData(val)) {}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Class constructor to move val onto the value
                ///
                ////////////////////////////////////////////////////////////////
                explicit FutureValue(T&& val) noexcept
                        : ptr(new SharedData(val)) {}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Default copy constructor
                ///
                ////////////////////////////////////////////////////////////////
                FutureValue(const FutureValue&) = default;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Default move constructor
                ///
                ////////////////////////////////////////////////////////////////
                FutureValue(FutureValue&&) noexcept = default;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Default copy operator
                ///
                ////////////////////////////////////////////////////////////////
                FutureValue& operator=(const FutureValue&) = default;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Default move operator
                ///
                ////////////////////////////////////////////////////////////////
                FutureValue& operator=(FutureValue&&) noexcept = default;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Bool operator
                ///
                ////////////////////////////////////////////////////////////////
                explicit operator bool() const {return exists();}
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief Whether the value has been initialised yet
                ///
                ////////////////////////////////////////////////////////////////
                bool exists() const
                {
                    std::unique_lock<std::mutex> lock(ptr->mutex);
                    return ptr->value.has_value();
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to get the value
                ///
                ////////////////////////////////////////////////////////////////
                const T& get() const
                {
                    std::unique_lock<std::mutex> lock(ptr->mutex);
                    return ptr->value.get();
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to wait for the future value to be assigned
                ///
                ////////////////////////////////////////////////////////////////
                void wait() const
                {
                    std::unique_lock<std::mutex> lock(ptr->mutex);
                    //While a value doesn't exist yet
                    while (!ptr->value.has_value())
                    {
                        //Wait for the value to be assigned
                        ptr->waitForValue.wait(lock);
                    }
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to assign the value of the future value
                ///
                ////////////////////////////////////////////////////////////////
                void assign(const T& val)
                {
                    std::unique_lock<std::mutex> lock(ptr->mutex);
                    //Set the value
                    ptr->value.reset(val);
                    //Notify anyone waiting
                    ptr->waitForValue.notify_all();
                }
        
        };
        
    }
    
}

#endif //OTHELLO_UTIL_FUTUREVALUE_HPP
