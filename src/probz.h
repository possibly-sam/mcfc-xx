#pragma once

#include <random>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <exception>
#include <sstream>
#include <utility>
#include <fstream>


inline void require_(bool p, const char* file, int line) {
  std::stringstream out;
  out << "Assertion failed:  " << file << "   #" << line << std::endl;
  if (!p) throw std::logic_error(out.str() );
  }

#ifdef DEBUG
#define require(p) require_( (p), __FILE__, __LINE__ );
#else
#define require(p)
#endif



class mc_nd {
private:
  static const double* my_nd;
  
public:
  
  double get_std_normal (double p) {
    require(0.0 <= p);
    require(p <= 1.0);
    
    int p0 = round(1000*p);
    return my_nd[p0];
    
  }
  
  double get_normal (double p, double mu, double sigma) {
    return mu + sigma*get_std_normal(p);
  }
  
  
  
  
};


template < typename RNG> 
class get_probability {
private:  
  RNG&    my_rng;
  mc_nd   my_mc_normal_distribution;
  //std::uniform_real_distribution<> my_distribution; 
public:
  get_probability(RNG& the_rng)
    : my_rng(the_rng)     
      // my_distribution( 0.0, 1.0)
  { ; }
  
  double get_mc_uniform() { return std::generate_canonical<double, 5>(my_rng); }
  
  double get_normal(double mu, double sigma) {
    return my_mc_normal_distribution.get_normal(get_mc_uniform(), mu, sigma );
  }
  
  bool did_it_trip(double p) {
    require(0.0 <= p);
    require(p <= 1.0);
    return get_mc_uniform() <= p;
    // return my_distribution(my_rng) <= p;
  }
  
  bool operator() (double p) { return did_it_trip(p); }
  
};


template <typename RNG>
class triangle_distribution {
  
private:
  get_probability<RNG>  my_mc_prob;
 
 

public:
  
  triangle_distribution(RNG& the_rng)
    : my_mc_prob(the_rng){ }  
  
  double operator () (double b, double e, double w) {
    require(b <= e);
    require(e <= w);
    
    if ( (w-b)*(w-b) < 1.0 ) return e; // they are basically the same so don't worry about it.
    
    // values near the mean are the most likely
    // standard deviation affects the dispersion of generated values from the mean
    double mu = (b + 4.0*e + w)/6.0;
    double sd = sqrt((mu-b)*(w-mu)/7);
    return my_mc_prob.get_normal(mu, sd);
    
    //std::normal_distribution<> d{mu, sd};
    
    //double result =   d(my_rng);
    
    //return d(my_rng);
    
  }
  
};
