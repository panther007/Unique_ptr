#ifndef CS540_UNIQUEPTR_HPP
#define CS540_UNIQUEPTR_HPP

#include<cstddef>
#include<utility>

#include<iostream>


#define DEBUG_STM(fmt) \
        std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<": "<<fmt<<std::endl
#define ERROR_STM(fmt) \
        std::cerr<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<": "<<fmt<<std::endl



namespace cs540{
  
  class UniquePtrHelper{
    public:
      virtual ~UniquePtrHelper(){
      }
  };
  
  template<class H>
  class UniquePtrHelper_2 : public UniquePtrHelper{
    public:
      typedef  H* ptr;
    private:
      ptr ptr_m;
    public:
      UniquePtrHelper_2(H* h) : ptr_m(h){}
      virtual ~UniquePtrHelper_2(){
        delete ptr_m;
      }
  };
  
  /*******************************************************************************
   * class: template<typename T> class UniquePtr
   * ----------------------------
   * Description:
   *   UniquePtr class is a template pointer with implementation similar to
   *   std::unique_ptr
   *
   ******************************************************************************/
  template<typename T>
  class UniquePtr{
    public:
      typedef T* ptrType;
      typedef UniquePtrHelper* dtrType;
            
    private:
      //member variables
      ptrType ptr_m;
      dtrType dtr_m;
    
    public:
      /*******************************************************************************
       * Function: UniquePtr()
       * ----------------------------
       * Description:
       *   Default constructor. Initializes the member variable with nullptr
       *
       * Arguments:
       *   None
       *
       * returns:
       *   None
       ******************************************************************************/
      UniquePtr() : ptr_m(nullptr), dtr_m(nullptr){
      }
      
      /*******************************************************************************
       * Function: template<typename U> explicit UniquePtr(U *)
       * ----------------------------
       * Description:
       *   Parameterized template constructor. Initializes the uniquePtr member
       *   varaable with the input U typr pointer
       *
       * Arguments:
       *   U *p
       *
       * returns:
       *   None
       ******************************************************************************/
      template<typename U> explicit UniquePtr(U *p) : ptr_m(p), dtr_m(new UniquePtrHelper_2<U>(p)){
      }
      
      /*******************************************************************************
       * Function: UniquePtr(UniquePtr&& U)
       * ----------------------------
       * Description:
       *   Move constructor. moves the input pinter to the member variable of the class
       *   using std::move. sets the input pointer to nullptr
       *
       * Arguments:
       *   UniquePtr&& U
       *
       * returns:
       *   None
       ******************************************************************************/
      UniquePtr(UniquePtr&& U) : ptr_m(std::move(U.ptr_m)), dtr_m(std::move(U.dtr_m)){
        U.ptr_m = nullptr;
        U.dtr_m = nullptr;
      }
      
      /*******************************************************************************
       * Function: template<typename U> UniquePtr(UniquePtr<U>&&)
       * ----------------------------
       * Description:
       *   Template Move constructor. moves the input U type pinter to the member 
       *   variable of the class using std::move. sets the input pointer to nullptr
       *
       * Arguments:
       *   UniquePtr<U>&& u
       *
       * returns:
       *   None
       ******************************************************************************/
      template<typename U> UniquePtr(UniquePtr<U>&& u) : ptr_m(std::move(u.ptr_m)), dtr_m(std::move(u.dtr_m)){
        u.ptr_m = nullptr;
        u.dtr_m = nullptr;
      }
      
      /*
       * deleting the copy constructor
       */
      UniquePtr(const UniquePtr&) = delete;
      template<typename U> UniquePtr(const UniquePtr<U>&) = delete;
      
      /*
       * Destructor
       */
      ~UniquePtr(){
        this->ptr_m=nullptr;
        delete this->dtr_m;
      }
      
      /*******************************************************************************
       * Function: get() const
       * ----------------------------
       * Description:
       *   This functions returns the the member varible
       *
       * Arguments:
       *   None
       *
       * returns:
       *   T *
       ******************************************************************************/
      T *get() const{
        return this->ptr_m;
      }
      
      /*******************************************************************************
       * Function: template<typename U> reset(U*)
       * ----------------------------
       * Description:
       *   This template function replaces the member variable with the input pointer of 
       *   type U by deleting the pointer which it holds already
       *
       * Arguments:
       *   U *p
       *
       * returns:
       *   None
       ******************************************************************************/
      template<typename U> void reset(U *p){
        if(this->ptr_m!=nullptr)
          delete this->dtr_m;
        dtr_m = new UniquePtrHelper_2<U>(p);
        ptr_m = p;
      }
      
      /*******************************************************************************
       * Function: reset
       * ----------------------------
       * Description:
       *   This function replaces the member variable with nullptr by deleting the 
       *   pointer which it holds already
       *
       * Arguments:
       *   U *p
       *
       * returns:
       *   None
       ******************************************************************************/
      void reset(std::nullptr_t){
        delete this->dtr_m;
        this->ptr_m = nullptr;
        this->dtr_m = nullptr;
      }
      
      /*
       * Deleting the copy assignment operator
       */
      UniquePtr& operator=(const UniquePtr& ) = delete;
      template<typename U> UniquePtr& operator=(const UniquePtr<U>&) = delete;
      
      
      UniquePtr& operator=(UniquePtr&& p){
        delete ptr_m;
        ptr_m = std::move(p.ptr_m);
        p.ptr_m = nullptr;
        return *this;
      }
      
      template<typename U> UniquePtr& operator=(UniquePtr<U>&& p){
        delete ptr_m;
        delete dtr_m;
        dtr_m = new UniquePtrHelper_2<U>(p);
        ptr_m = std::move(p.ptr_m);
        p.ptr_m = nullptr;
        return *this;
      }
      T& operator*() const{
        return *(this->ptr_m);
      }
      T *operator->() const{
        return this->ptr_m;
      }
      explicit operator bool() const{
        return (ptr_m != nullptr) ? true : false;
      }
    
  };
  
  template<typename T1, typename T2>
  inline bool operator==(const UniquePtr<T1>& A, const UniquePtr<T2>& B){
    return A.get()==B.get();
  }
  
  template<typename T1, typename T2>
  inline bool operator!=(const UniquePtr<T1>&A, const UniquePtr<T2>&B){
    //return !(A==B);
    return A.get()!=B.get();
  }
  
  template<typename T1>
  inline bool operator==(const UniquePtr<T1>&A, std::nullptr_t){
    return A.get()==nullptr;
  }
  
  template<typename T1>
  inline bool operator==(std::nullptr_t, const UniquePtr<T1>&A){
    return A.get()==nullptr;
  }
  
  template<typename T1>
  inline bool operator!=(const UniquePtr<T1>&A, std::nullptr_t){
    return A.get()!=nullptr;
  }
  
  template<typename T1>
  inline bool operator!=(std::nullptr_t, const UniquePtr<T1>&A){
    return A.get()!=nullptr;
  }

}//End of cs540 namespace

#endif //#ifndef CS540_UNIQUEPTR_HPP

