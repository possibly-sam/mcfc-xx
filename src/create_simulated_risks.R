# create simulated risks . R


simulated_risks_ <- function() {
  result <- new.env(emptyenv())
  

get_some_sample_probabilities <- function(n, alpha=2) {
  
  q <- c( rep(.5, n/alpha),
          rep(.333, n/alpha^2),
          rep(.666, n/alpha^2),
          rep(.25, n/alpha^3),
          rep(.75, n/alpha^3),
          c(.2, .4, .6, .8) %>% map( ~rep(., n/alpha^4)) %>% unlist(),
          ( (1:9)/10 ) %>% map( ~rep(., n/alpha^5)) %>% unlist(),
          ( (1:19)/20 ) %>% map( ~rep(., n/alpha^6)) %>% unlist()
          
  )
  
  result <- round(100*rnorm(n, .5, .25))/100
  result[ result < 0 ] <- min(.5, -result[ result < 0 ])
  result[ result > 1 ] <- max(.5, 1-result[ result > 1 ])
  result <- c(q, result)
  result %>% sample(n);
}


kround <- function(n) ((n/1000) %>% round()) * 1000

get_some_sample_amounts <- function(n, mu=11.25) {
  result <- n %>% rnorm(11.25, 1.6) 
  result %>% exp() %>% kround();
}

# 999 %>% get_some_sample_amounts() %>% log() %>%   hist()

get_some_sample_best <- function(n) {
  result <- c(get_some_sample_amounts(n), rep(0, n/20))
  result %>% sample(n)
  
}

get_some_sample_expected <- function(n) {
  get_some_sample_amounts(n, 11.5);
}

get_some_sample_worst <- function(n) {
  get_some_sample_amounts(n, 12);
}


get_expected_from_best <- function(b) {
  
  if (0 == b) return ( 1 %>% get_some_sample_best());
  e <- rnorm(1, 1-.00*log(b), .5)
  # cat( paste(b, " ", e, "\n")) 
  max(b, b * exp(e)) %>% kround()
  
}


get_worst_from_expected <- function(e) {
  
  w <- rnorm(1, .4+log(e), 1.67)
  (e + exp(w)) %>% kround()
  
}


result$get_some_sample_risks <- function(n) {
  
  b <- get_some_sample_best(n)
  e <- b %>% map(get_expected_from_best) %>% unlist()
  w <- get_worst_from_expected(e)
  p = get_some_sample_probabilities(n)
  
  data.frame( b=b, e=e, w=w, p=p );
}

result
}


simulated_risks <- simulated_risks_()

rm(simulated_risks_)