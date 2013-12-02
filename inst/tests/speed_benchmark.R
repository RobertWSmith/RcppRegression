# speed benchmark

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

autoplot(res)


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

autoplot(res)


vec <- as.numeric(rnorm(100, 100, 25))
mat <- matrix(rnorm(100*20, 25, 5), nrow = 100, ncol = 20)

lm <- new(LS, vec, mat)
length(vec)
print( paste("ncol = ", ncol(mat), ", nrow = ", nrow(mat) ) )

### long benchmark
res <- microbenchmark(
	lm$BestSubset( ),
	lm$ompBestSubset( ),
	times = 5L
)

print(res)

autoplot(res)

