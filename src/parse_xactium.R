
library(tidyverse)
library(readr)

Xactium <- read_csv("~/Desktop/hack-04/Xactium.csv")

# Xactium$Business_Unit__c %>% table() %>% pie()



x_residual <- Xactium[ Xactium$Risk__Mitigation__c == "Residual" , ]
# x_target <- Xactium[ Xactium$Risk__Mitigation__c == "Target" , ]
# x_inherent <- Xactium[ Xactium$Risk__Mitigation__c == "Inherent" , ]

# clean up stuff when we no longer need it.  Not because memory is a concern at this level, but just for organization to avoid cluttering up the namespace.
rm(Xactium)


x_residual_likelihood <- x_residual[ x_residual$Risk__Category__c=="Likelihood", ]
x_residual_cost <- x_residual[ x_residual$Risk__Category__c=="Cost", ]

rm(x_residual)


xx_residual <- x_residual_likelihood %>% left_join(x_residual_cost, c("grc__Risk_Name__c", "ExtractDate"), FALSE, c(".prob", ".cost"))

rm(x_residual_likelihood)
rm(x_residual_cost)

xxx_residual <- xx_residual[ , c( "Risk__Value__c.prob",  "Risk__Best_Case__c.cost", "Risk__Value__c.cost", "Risk__Worst_Case__c.cost"  )]

rm(xx_residual)

colnames(xxx_residual) <- c("p", "b", "e", "w")
xxx_residual <- xxx_residual[ !is.na(xxx_residual$b),]



xxx_residual <- xxx_residual[xxx_residual$b <= xxx_residual$e, ]
xxx_residual <- xxx_residual[xxx_residual$e <= xxx_residual$w, ]
xxx_residual <- xxx_residual[ !(xxx_residual$b == xxx_residual$w & xxx_residual$w == 0) , ]
xxx_residual$p <- xxx_residual$p/100


xxx_residual %>% head()
