# mcfc-xx
Project Hack VI


in order to generate the random numbers to simulate risk in the monte carlo analysis, either to determine if a given risk tripped, or the value of the risk within a given distribution, we use the standard c++ 

*std::random_device*

https://en.cppreference.com/w/cpp/numeric/random/random_device

This is then wrapped into two classes 

*get_probability*

and 

*triangle_distribution*

which determine if a given risk trips and the value of the impact respectively.  Templates are used liberally and some might say frivolously in these classes.  

Aside:  this method of determining the value is by no means the only one.  It is not even the only one for the triangle distribution.  An excercise would be to implement other distributions here.



Then the class 

*mc_collapse*


uses the above classes to 'collapse' the data.  Here 'collapse' is meant to determine if a risk trips and if so what the impact is, in analogy to the 'collapse' of a quantum wave function.

The data is organized as four arrays for ( best, expected, worst, probablity).  It is necessary to organize this way instead of as an array of objects containing 4 fields in order to integrate efficiently with R.

The functions *collapse_all* and *collapse_many* in the *mc_collapse* class do the main jobs of collapsing the four arrays mentioned above.  *collapse_all* does it one time and  *collapse_many*  does it many times.  This is the main result that gets put in the histogram that is one of the primary outputs of the foundational 


