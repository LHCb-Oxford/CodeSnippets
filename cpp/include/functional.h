namespace functional {
  template <class fcn>
    class functor {
      private :
        fcn m_fcn;
      public:
        functor( fcn f) : m_fcn(f) {}
        template <typename...Args>
          auto operator()( Args&&... args ){ return m_fcn(std::forward<Args>(args)...) ; }
    };

  template <class fcn>
    functor<fcn> make_functor( fcn&& f ){
      return functor<fcn>(f);  
    }

  template <class T1,class T2>
    class functor_sum {
      private:
        functor<T1> m_t1;
        functor<T2> m_t2;
      public:
        functor_sum( const functor<T1>& t1, const functor<T2>& t2 ) : m_t1(t1),m_t2(t2) {}
        template <typename... Args>
          auto operator()( Args&&... args ){ return m_t1(std::forward<Args>(args)...) + 
            m_t2(std::forward<Args>(args)...); }
    };

  template<class T1,class T2>
    auto operator+( const functor<T1>& t1, const functor<T2>& t2 ){
      return make_functor( functor_sum<T1,T2>(t1,t2) );
    }

  template <class T1,class T2>
    class functor_prod {
      private:
        functor<T1> m_t1;
        functor<T2> m_t2;
      public:
        functor_prod( const functor<T1>& t1, const functor<T2>& t2 ) : m_t1(t1),m_t2(t2) {}
        template <typename... Args>
          auto operator()( Args&&... args ){ return m_t1(std::forward<Args>(args)...) * m_t2(std::forward<Args>(args)...); }
    };

  template<class T1,class T2>
    auto operator*( const functor<T1>& t1, const functor<T2>& t2 ){
      return make_functor( functor_prod<T1,T2>(t1,t2) );
    }
  
  template <class T1,class T2>
    class functor_div {
      private:
        functor<T1> m_t1;
        functor<T2> m_t2;
      public:
        functor_div( const functor<T1>& t1, const functor<T2>& t2 ) : m_t1(t1),m_t2(t2) {}
        template <typename... Args>
          auto operator()( Args&&... args ){ return m_t1(std::forward<Args>(args)...) / m_t2(std::forward<Args>(args)...); }
    };

  template<class T1,class T2>
    auto operator/( const functor<T1>& t1, const functor<T2>& t2 ){
      return make_functor( functor_div<T1,T2>(t1,t2) );
    }
  
  template <class T1,class T2>
    class functor_sub {
      private:
        functor<T1> m_t1;
        functor<T2> m_t2;
      public:
        functor_sub( const functor<T1>& t1, const functor<T2>& t2 ) : m_t1(t1),m_t2(t2) {}
        template <typename... Args>
          auto operator()( Args&&... args ){ return m_t1(std::forward<Args>(args)...) - m_t2(std::forward<Args>(args)...); }
    };

  template<class T1,class T2>
    auto operator-( const functor<T1>& t1, const functor<T2>& t2 ){
      return make_functor( functor_sub<T1,T2>(t1,t2) );
    }
  
  template <class T1,class T2>
    class functor_and {
      private:
        functor<T1> m_t1;
        functor<T2> m_t2;
      public:
        functor_and( const functor<T1>& t1, const functor<T2>& t2 ) : m_t1(t1),m_t2(t2) {}
        template <typename... Args>
          auto operator()( Args&&... args ){ return m_t1(std::forward<Args>(args)...) && m_t2(std::forward<Args>(args)...); }
    };

  template<class T1,class T2>
    auto operator&&( const functor<T1>& t1, const functor<T2>& t2 ){
      return make_functor(functor_and<T1,T2>(t1,t2));
    }
}
#include <cxxabi.h>

template <class T1> std::string getClassName( const T1& f1 ){
  int status=0;
  const std::string retName = abi::__cxa_demangle(typeid(T1).name(), 0, 0, &status);
  return retName; 
}

