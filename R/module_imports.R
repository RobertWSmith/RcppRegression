
#' @export
Rcpp::loadModule( "Regression", what = c("Data", "Output", "OutVec", "LS") )



# library(RcppRegression)
# load(file = "data/cereal.rda")
# vec <- as.numeric(cereal[,2])
# mat <- as.matrix(cereal[,3:6])
# 
# ls <- new(LS, vec, mat)
# str(ls)
# 
# op <- ls$OLS(1:4)
# str(op)
# ls$print() # OLS method doesn't save in LS object
# op$print() # op is returned Output class
# 
# ls$BestSubset()
# op <- ls$data
# str(op)
# ls$print()
# ls$data$print()
# 
# ls$ompBestSubset()
# op <- ls$data
# 
# 
# str(op)
# ls$print()
# ls$data$print()
# 
# os <- ls$data
# os$print()
# 
# ls$data$data.frame
# 
# os <- ls$data
# str(os)
# for (i in os) {
# 	i$print()
# }
# 
# dt <- os$data
# str(dt)
# for (i in dt) {
# 	i$print()
# }
# 

library(RcppRegression)
library(ggplot2)
library(microbenchmark)
load(file = "data/cereal.rda")
vec <- as.numeric(cereal[,2])
mat <- as.matrix(cereal[,3:6])
lm <- new(LS, vec, mat)

res <- microbenchmark(
	lm$BestSubset( ),
	lm$ompBestSubset( ),
	times = 1000L
	)

print(res)

plot(res)


vec <- as.numeric(rnorm(100, 100, 25))
mat <- matrix(rnorm(100*10, 25, 5), nrow = 100, ncol = 10)

lm <- new(LS, vec, mat)
length(vec)
print( paste("ncol = ", ncol(mat), ", nrow = ", nrow(mat) ) )

res <- microbenchmark(
	lm$BestSubset( ),
	lm$ompBestSubset( ),
	times = 100L
)

print(res)

plot(res)


vec <- as.numeric(rnorm(100, 100, 25))
mat <- matrix(rnorm(100*20, 25, 5), nrow = 100, ncol = 20)

lm <- new(LS, vec, mat)
length(vec)
print( paste("ncol = ", ncol(mat), ", nrow = ", nrow(mat) ) )

### long benchmark
res <- microbenchmark(
	lm$BestSubset( ),
	lm$ompBestSubset( ),
	times = 1L
)

print(res)

plot(res)


