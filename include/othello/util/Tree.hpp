#ifndef OTHELLO_UTIL_TREE_HPP
#define OTHELLO_UTIL_TREE_HPP

//Standard C++:
#include <vector>

namespace othello
{
    
    namespace util
    {
    
        ////////////////////////////////////////////////////////////////
        /// \class tree
        ///
        /// \brief Recursive class for representing a tree
        ///
        /// Since this class is basically just a recursive vector with a
        /// value, a lot of the member functions are copy+pasted (with
        /// modification to fit a tree-based container) from
        /// https://en.cppreference.com/w/cpp/container/vector. This is
        /// because inheriting from C++ containers is widely regarded as
        /// a bad move (due to lack of virtual destructor etc.)
        ///
        ////////////////////////////////////////////////////////////////
        template<class T, class Allocator = std::allocator<T> >
        class tree
        {
            private:
        
                ////////////////////////////////////////////////////////////////
                /// \brief The tree node's value
                ///
                ////////////////////////////////////////////////////////////////
                T value = T();
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief A vector of the tree's children
                ///
                ////////////////////////////////////////////////////////////////
                std::vector<T, Allocator> children;
            
                
            public:
                
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector
                ///
                ////////////////////////////////////////////////////////////////
                typedef tree<T> value_type;
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector
                ///
                ////////////////////////////////////////////////////////////////
                typedef Allocator allocator_type;
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector
                ///
                ////////////////////////////////////////////////////////////////
                typedef std::size_t size_type;
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector
                ///
                ////////////////////////////////////////////////////////////////
                typedef std::ptrdiff_t difference_type;
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector
                ///
                ////////////////////////////////////////////////////////////////
                typedef value_type& reference;
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector
                ///
                ////////////////////////////////////////////////////////////////
                typedef const value_type& const_reference;
        
                
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector
                ///
                ////////////////////////////////////////////////////////////////
                typedef typename std::allocator_traits<Allocator>::pointer pointer;
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector
                ///
                ////////////////////////////////////////////////////////////////
                typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector
                ///
                ////////////////////////////////////////////////////////////////
                typedef typename std::vector<tree<T>, Allocator>::iterator iterator;
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector
                ///
                ////////////////////////////////////////////////////////////////
                typedef typename std::vector<tree<T>, Allocator>::const_iterator const_iterator;
                
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector
                ///
                ////////////////////////////////////////////////////////////////
                typedef typename std::vector<tree<T>, Allocator>::reverse_iterator reverse_iterator;
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector
                ///
                ////////////////////////////////////////////////////////////////
                typedef typename std::vector<tree<T>, Allocator>::const_reverse_iterator const_reverse_iterator;
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/vector
                ///
                ////////////////////////////////////////////////////////////////
                tree() noexcept(noexcept(Allocator())) = default;
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/vector
                ///
                ////////////////////////////////////////////////////////////////
                explicit tree(const Allocator& alloc) noexcept : children(alloc) {}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Class constructor that copies the given value and
                ///        sets it as the node's value
                ///
                ////////////////////////////////////////////////////////////////
                explicit tree(const T& nodeValue) : value(nodeValue) {}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Class constructor that moves the given value and sets
                ///        it as the node's value
                ///
                ////////////////////////////////////////////////////////////////
                explicit tree(T&& nodeValue) noexcept : value(std::move(nodeValue)) {}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/vector
                ///
                /// \param nodeValue The tree node's value
                /// \param count The number of child nodes to create
                /// \param childValue The value of all the created child nodes
                ///
                ////////////////////////////////////////////////////////////////
                tree(const T& nodeValue, size_type count, const tree<T>& childValue,
                        const Allocator& alloc = Allocator())
                    : value(nodeValue), children(count, childValue, alloc) {}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/vector
                ///
                /// \param count The number of default child nodes to create
                ///
                ////////////////////////////////////////////////////////////////
                explicit tree(size_type count, const Allocator& alloc = Allocator())
                    : children(count, alloc) {}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/vector
                ///
                ////////////////////////////////////////////////////////////////
                template<class InputIt>
                tree(const T& nodeValue, InputIt first, InputIt last,
                        const Allocator& alloc = Allocator())
                    : value(nodeValue), children(first, last, alloc) {}
                
                
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/vector
                ///
                ////////////////////////////////////////////////////////////////
                tree(const tree& other)
                    : value(other.value), children(other) {}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/vector
                ///
                ////////////////////////////////////////////////////////////////
                tree(const tree& other, const Allocator& alloc)
                    : value(other.value), children(other, alloc) {}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/vector
                ///
                ////////////////////////////////////////////////////////////////
                tree(tree&& other) noexcept
                    : value(std::move(other.value)), children(other) {}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/vector
                ///
                ////////////////////////////////////////////////////////////////
                tree(tree&& other, const Allocator& alloc)
                    : value(std::move(other.value), children(other, alloc)) {}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/vector
                ///
                ////////////////////////////////////////////////////////////////
                tree(const T& value, std::initializer_list<tree<T> > init,
                        const Allocator& alloc = Allocator())
                    : value(value), children(init, alloc) {}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Dereference operator
                ///
                /// This function is similar to std::shared_ptr::operator*()
                ///
                /// \see https://en.cppreference.com/w/cpp/memory/shared_ptr/operator*
                ///
                /// \return The node's value
                ///
                ////////////////////////////////////////////////////////////////
                T& operator*() const noexcept {return value;}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Pointer access operator
                ///
                /// This function is similar to std::shared_ptr::operator->()
                ///
                /// \see https://en.cppreference.com/w/cpp/memory/shared_ptr/operator*
                ///
                /// \return The node's value
                ///
                ////////////////////////////////////////////////////////////////
                T* operator->() const noexcept {return &value;}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to get the node's value
                ///
                ////////////////////////////////////////////////////////////////
                T& get() const {return value;}
                
                
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/operator%3D
                ///
                ////////////////////////////////////////////////////////////////
                tree& operator=(const tree& other)
                {
                    *this = other;
                    return *this;
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/operator%3D
                ///
                ////////////////////////////////////////////////////////////////
                tree& operator=(tree&& other) noexcept
                {
                    *this = std::move(other);
                    return *this;
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/operator%3D
                ///
                ////////////////////////////////////////////////////////////////
                tree& operator=(std::pair<const T&, std::initializer_list<tree<T> > > valAndList)
                {
                    value = valAndList.first;
                    children = valAndList.second;
                    return *this;
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/assign
                ///
                ////////////////////////////////////////////////////////////////
                void assign(const T& nodeValue, size_type count, const tree<T>& childValue)
                {
                    value = nodeValue;
                    children.assign(count, childValue);
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/assign
                ///
                ////////////////////////////////////////////////////////////////
                template<class InputIt>
                void assign(const T& nodeValue, InputIt first, InputIt last)
                {
                    value = nodeValue;
                    children.assign(first, last);
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/assign
                ///
                ////////////////////////////////////////////////////////////////
                void assign(const T& nodeValue, std::initializer_list<tree<T> > ilist)
                {
                    value = nodeValue;
                    children.assign(ilist);
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/get_allocator
                ///
                ////////////////////////////////////////////////////////////////
                allocator_type get_allocator() const {return children.get_allocator();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/at
                ///
                ////////////////////////////////////////////////////////////////
                reference at(size_type pos) {return children.at(pos);}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/at
                ///
                ////////////////////////////////////////////////////////////////
                const_reference at(size_type pos) const {return children.at(pos);}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/operator_at
                ///
                ////////////////////////////////////////////////////////////////
                reference operator[](size_type pos) {return children.operator[](pos);}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/operator_at
                ///
                ////////////////////////////////////////////////////////////////
                const_reference operator[](size_type pos) const {return children.operator[](pos);}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/front
                ///
                ////////////////////////////////////////////////////////////////
                reference front() {return children.front();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/front
                ///
                ////////////////////////////////////////////////////////////////
                const_reference front() const {return children.front();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/back
                ///
                ////////////////////////////////////////////////////////////////
                reference back() {return children.back();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/back
                ///
                ////////////////////////////////////////////////////////////////
                const_reference back() const {return children.back();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to get a pointer to the underlying child
                ///        node array
                ///
                /// \see https://en.cppreference.com/w/cpp/container/vector/data
                ///
                ////////////////////////////////////////////////////////////////
                pointer* data() noexcept {return children.data();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to get a const pointer to the underlying
                ///        child node array
                ///
                /// \see https://en.cppreference.com/w/cpp/container/vector/data
                ///
                ////////////////////////////////////////////////////////////////
                const_pointer* data() const noexcept {return children.data();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/begin
                ///
                ////////////////////////////////////////////////////////////////
                iterator begin() noexcept {return children.begin();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/begin
                ///
                ////////////////////////////////////////////////////////////////
                const_iterator begin() const noexcept {return children.begin();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/begin
                ///
                ////////////////////////////////////////////////////////////////
                const_iterator cbegin() const noexcept {return children.cbegin();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/end
                ///
                ////////////////////////////////////////////////////////////////
                iterator end() noexcept {return children.end();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/end
                ///
                ////////////////////////////////////////////////////////////////
                const_iterator end() const noexcept {return children.end();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/end
                ///
                ////////////////////////////////////////////////////////////////
                const_iterator cend() const noexcept {return children.cend();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/rbegin
                ///
                ////////////////////////////////////////////////////////////////
                reverse_iterator rbegin() noexcept {return children.rbegin();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/rbegin
                ///
                ////////////////////////////////////////////////////////////////
                const_reverse_iterator rbegin() const noexcept {return children.rbegin();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/rbegin
                ///
                ////////////////////////////////////////////////////////////////
                const_reverse_iterator crbegin() const noexcept {return children.crbegin();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/rend
                ///
                ////////////////////////////////////////////////////////////////
                reverse_iterator rend() noexcept {return children.rend();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/rend
                ///
                ////////////////////////////////////////////////////////////////
                const_reverse_iterator rend() const noexcept {return children.rend();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/rend
                ///
                ////////////////////////////////////////////////////////////////
                const_reverse_iterator crend() const noexcept {return children.crend();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to determine whether this tree has no child
                ///        nodes
                ///
                /// \see https://en.cppreference.com/w/cpp/container/vector/empty
                ///
                ////////////////////////////////////////////////////////////////
                bool empty() const noexcept {return children.empty();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to get the number of child nodes directly
                ///        owned by this tree node
                ///
                /// \see https://en.cppreference.com/w/cpp/container/vector/size
                ///
                ////////////////////////////////////////////////////////////////
                size_type size() const noexcept {return children.size();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to get the max amount of child nodes
                ///
                /// \see https://en.cppreference.com/w/cpp/container/vector/max_size
                ///
                ////////////////////////////////////////////////////////////////
                size_type max_size() const noexcept {return children.max_size();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to reserve child nodes
                ///
                /// \see https://en.cppreference.com/w/cpp/container/vector/reserve
                ///
                ////////////////////////////////////////////////////////////////
                void reserve(size_type new_cap) {children.reserve(new_cap);}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to get the capacity of child nodes for this
                ///        tree node
                ///
                /// \see https://en.cppreference.com/w/cpp/container/vector/capacity
                ///
                ////////////////////////////////////////////////////////////////
                size_type capacity() const noexcept {return children.capacity();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to shrink the child node vector to fit the
                ///        actual number of child nodes
                ///
                /// \see https://en.cppreference.com/w/cpp/container/vector/shrink_to_fit
                ///
                ////////////////////////////////////////////////////////////////
                void shrink_to_fit() {children.shrink_to_fit();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to erase all the children in the tree
                ///
                /// \see https://en.cppreference.com/w/cpp/container/vector/clear
                ///
                ////////////////////////////////////////////////////////////////
                void clear() noexcept {children.clear();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/insert
                ///
                ////////////////////////////////////////////////////////////////
                iterator insert(const_iterator pos, const tree<T>& childValue)
                {
                    return children.insert(pos, childValue);
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/insert
                ///
                ////////////////////////////////////////////////////////////////
                iterator insert(const_iterator pos, tree<T>&& childValue)
                {
                    return children.insert(pos, childValue);
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/insert
                ///
                ////////////////////////////////////////////////////////////////
                iterator insert(const_iterator pos, size_type count, const tree<T>& childValue)
                {
                    return children.insert(pos, count, childValue);
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/insert
                ///
                ////////////////////////////////////////////////////////////////
                template<class InputIt>
                iterator insert(const_iterator pos, InputIt first, InputIt last)
                {
                    return children.insert(pos, first, last);
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/insert
                ///
                ////////////////////////////////////////////////////////////////
                iterator insert(const_iterator pos, std::initializer_list<tree<T> > ilist)
                {
                    return children.insert(pos, ilist);
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/emplace
                ///
                ////////////////////////////////////////////////////////////////
                template<class... Args>
                iterator emplace(const_iterator pos, Args&&... args)
                {
                    return children.emplace(pos, std::forward<Args>(args)...);
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/erase
                ///
                ////////////////////////////////////////////////////////////////
                iterator erase(const_iterator pos) {return children.erase(pos);}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/erase
                ///
                ////////////////////////////////////////////////////////////////
                iterator erase(const_iterator first, const_iterator last)
                {
                    return children.erase(first, last);
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/push_back
                ///
                ////////////////////////////////////////////////////////////////
                void push_back(const tree<T>& childValue) {return children.push_back(childValue);}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/push_back
                ///
                ////////////////////////////////////////////////////////////////
                void push_back(tree<T>&& childValue) {return children.push_back(childValue);}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/emplace_back
                ///
                ////////////////////////////////////////////////////////////////
                template<class... Args>
                void emplace_back(Args&&... args)
                {
                    children.emplace_back(std::forward<Args>(args)...);
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/pop_back
                ///
                ////////////////////////////////////////////////////////////////
                void pop_back() {children.pop_back();}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/resize
                ///
                ////////////////////////////////////////////////////////////////
                void resize(size_type count) {children.resize(count);}
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/resize
                ///
                ////////////////////////////////////////////////////////////////
                void resize(size_type count, const value_type& childValue)
                {
                    children.resize(count, childValue);
                }
        
        
                ////////////////////////////////////////////////////////////////
                /// \see https://en.cppreference.com/w/cpp/container/vector/swap
                ///
                ////////////////////////////////////////////////////////////////
                void swap(tree<T>& other) noexcept
                {
                    std::swap(value, other.value);
                    children.swap(other.children);
                }
                
        };
    
    
        ////////////////////////////////////////////////////////////////
        /// \see https://en.cppreference.com/w/cpp/container/vector/operator_cmp
        ///
        ////////////////////////////////////////////////////////////////
        template<class T, class Alloc>
        bool operator==(const tree<T, Alloc>& lhs, const tree<T, Alloc>& rhs)
        {
            return lhs.value == rhs.value && lhs.children == rhs.children;
        }
    
    
        ////////////////////////////////////////////////////////////////
        /// \see https://en.cppreference.com/w/cpp/container/vector/operator_cmp
        ///
        ////////////////////////////////////////////////////////////////
        template<class T, class Alloc>
        bool operator!=(const tree<T, Alloc>& lhs, const tree<T, Alloc>& rhs)
        {
            return lhs.value != rhs.value || lhs.children != rhs.children;
        }
    
    
        ////////////////////////////////////////////////////////////////
        /// \see https://en.cppreference.com/w/cpp/container/vector/swap2
        ///
        ////////////////////////////////////////////////////////////////
        template<class T, class Alloc>
        void swap(tree<T, Alloc>& lhs, tree<T, Alloc>& rhs) {lhs.swap(rhs);}
        
    }
    
}

#endif //OTHELLO_UTIL_TREE_HPP
