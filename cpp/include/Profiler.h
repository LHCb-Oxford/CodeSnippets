#include <chrono>
#include <cxxabi.h>

template <class TYPE> std::string type_name(){
  int status=0;
  return abi::__cxa_demangle(typeid(TYPE).name() , 0, 0, &status );
}

template < class FCN > void Profile( FCN&& fcn, const unsigned int& nTrials = 100 ){

  double PROFILE_TIME=0;
  double PROFILE_TIME2=0;

  for( unsigned int TRIAL= 0 ; TRIAL < nTrials; ++TRIAL ){
    auto PROFILE_TIME_START = std::chrono::high_resolution_clock::now();
    fcn();
    auto PROFILE_TIME_END   = std::chrono::high_resolution_clock::now();
    double PROFILE_RUN_TIME = std::chrono::duration<double, std::milli>(PROFILE_TIME_END-PROFILE_TIME_START).count() ;
    PROFILE_TIME+=PROFILE_RUN_TIME;
    PROFILE_TIME2+=PROFILE_RUN_TIME*PROFILE_RUN_TIME;
  }
  PROFILE_TIME/=double(nTrials);
  PROFILE_TIME2 = PROFILE_TIME2 / double(nTrials ) - PROFILE_TIME * PROFILE_TIME ;
  std::cout << type_name<FCN>() << "   time per run = " << PROFILE_TIME << " ± " << PROFILE_TIME2 << "[ms]"<< std::endl;
}

