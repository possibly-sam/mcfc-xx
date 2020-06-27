#pragma once

using namespace std;

template <typename T, typename U>
inline double  collapse_( T& prob_fn, U& dist_fn, double b, double e, double w, double p) {
  
  if (!prob_fn.did_it_trip(p) ) return 0; // the risk didn't trip
  
  return dist_fn(b,e,w);
  
}


template <typename array_type> 
class mc_collapse {
private:
  const array_type& my_best;
  const array_type& my_expected;
  const array_type& my_worst;
  const array_type& my_prob;
  std::random_device my_dev;
  triangle_distribution<std::random_device>* my_td_ptr;
  get_probability<std::random_device>* my_gp_ptr;
    
    
public:
  mc_collapse( const array_type& the_best, const array_type& the_expected, const array_type& the_worst, const array_type& the_prob)
    : my_best(the_best), my_expected(the_expected), my_worst(the_worst), my_prob(the_prob) ,
    my_td_ptr(NULL), my_gp_ptr(NULL)
  {
    
    
    require(the_best.size()     == the_expected.size());
    require(the_expected.size() == the_worst.size());
    require(the_worst.size()    == the_prob.size());
    

    my_td_ptr = new triangle_distribution<std::random_device> (my_dev);
    my_gp_ptr = new get_probability<std::random_device>  (my_dev);
    
    
  }
  
  
  virtual ~mc_collapse() {
    if (my_td_ptr) delete my_td_ptr;
    if (my_gp_ptr) delete my_gp_ptr;
    
  }
  
  // collapse each risk and return the value for each risks collapse in an array.
  array_type& collapse_all(array_type& result) {
    
    typename array_type::const_iterator bit = my_best.begin();
    typename array_type::const_iterator tit = my_expected.begin();
    typename array_type::const_iterator wit = my_worst.begin();
    typename array_type::const_iterator pit = my_prob.begin();
    typename array_type::iterator rit       = result.begin();
    
    for ( ; bit != my_best.end(); ++bit, ++tit, ++wit, ++pit, ++rit) {
      *rit = collapse_( *my_gp_ptr, *my_td_ptr, *bit, *tit, *wit, *pit);
    }  
    
    return result;
  }   
  
  
  
  
  // collapse all the risks n times.  return the n sums of the collapsed set of risks.
  array_type collapse_many(int n) {
 
    array_type collapsed(my_best.size());
    array_type result(n);
    
    for (typename array_type::iterator it = result.begin(); it != result.end(); ++it) {
      
      collapse_all(collapsed);
       
      *it = std::accumulate(collapsed.begin(), collapsed.end(),0.0);
      
    }
    
    return result;
  }  
  
  
    
    
  
  
  
  
  
  
  
  
};

