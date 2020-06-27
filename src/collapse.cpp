#ifndef RAW
  #include <Rcpp.h>
  using namespace Rcpp;
#endif 
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

#include "probz.h"
#include "mc_collapse.h"

using namespace std;
// This is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp 
// function (or via the Source button on the editor toolbar). Learn
// more about Rcpp at:
//
//   http://www.rcpp.org/
//   http://adv-r.had.co.nz/Rcpp.html
//   http://gallery.rcpp.org/
//

#ifdef RAW
  typedef vector<long double> NumericVector;
#endif







/*
 *  ok if we can do a collapse; now do a collapse accross vectors of risks
 * 
 * 
 */


// [[Rcpp::export]]
NumericVector collapse_all(const NumericVector& best, const NumericVector& expected,  const NumericVector& worst, const NumericVector& prob) {
 
  NumericVector result(best.size());
  mc_collapse<NumericVector> mc(best, expected, worst, prob);
  mc.collapse_all(result);
  return result;
}
  


  // [[Rcpp::export]]
NumericVector collapse_many(int n, const NumericVector& best, const NumericVector& expected,  const NumericVector& worst, const NumericVector& prob) {
  
  vector<long double> b( best.begin(), best.end() );
  vector<long double> e ( expected.begin(), expected.end());
  vector<long double> w ( worst.begin(), worst.end());
  vector<long double> p ( prob.begin(), prob.end() );
  
  mc_collapse<vector<long double>> mc(b, e, w, p);
  vector<long double> result =   mc.collapse_many(n);
  

  NumericVector  r(result.begin(), result.end() );
  return r; 
  

}  


NumericVector  readme(const char* filename) {
  NumericVector result;
  ifstream in(filename);
  
  if (!in) return result;
  
  
  
  while ( !in.eof() ) {
    long double q0 = 0;
    in >> q0;
    result.push_back(q0);
  }
  return result;
  
}

int main(int argc, char** argv) {
  
  NumericVector  b0;
  NumericVector  e0;
  NumericVector  w0;
  NumericVector  p0;
  long n = 0;
  
  
  if (argc == 6) {
    b0 = readme(argv[1]);
    e0 = readme(argv[2]);
    w0 = readme(argv[3]);
    p0 = readme(argv[4]);
    n = atol(argv[5]);
  }
  
  
  cout << b0.size() << endl;
  cout << e0.size() << endl;
  cout << w0.size() << endl;
  cout << p0.size() << endl;
  
  
  if (b0.size() && b0.size() == e0.size() && e0.size() == w0.size() && w0.size() == p0.size())
  {
    cout << "ok" << endl;
    const NumericVector result = collapse_many(n, b0, e0, w0, p0);
    for (NumericVector::const_iterator it = result.begin(); it != result.end(); ++it) {
      cout << *it << endl;
    }
    
    
  }
  
  
  
  return 0;
}

// Normal Distribution Lookup Table
#include "normdist.h"  
    

// You can include R code blocks in C++ files processed with sourceCpp
// (useful for testing and development). The R code will be automatically 
// run after the compilation.
//

/*** X

b0 <- c(1,2,3)
e0 <- c(2, 4, 8)
w0 <- c(10, 100, 1000)
p0 <- c(.62, .38, .24)


x0 <- collapse_all(b0, e0, w0, p0);

x0 %>% plot()

x1 <- collapse_many( 99, b0, e0, w0, p0)

x1 %>% hist()

rm(b0, e0, w0, p0, x0, x1)


*/







// You can include R code blocks in C++ files processed with sourceCpp
// (useful for testing and development). The R code will be automatically 
// run after the compilation.
//    R -d gdb -e "Rcpp::sourceCpp('collapse.cpp')"

/*** R
library(tidyverse)

xxx <- read.csv("xxx.csv")


cm0 <- function(it, n= 19)  collapse_many(n, it$b, it$e, it$w, it$p )


xxx %>% sample_frac(.9) %>% cm0(999) %>% summary() # hist(breaks=29)

rm(cm0)
rm (xxx)

*/


