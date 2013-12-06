# speed benchmark

library(RcppRegression)
library(ggplot2)
library(scales)
library(microbenchmark)
library(gridExtra)

vecfun <- function(LEN, MEAN, SD) {
	return (rnorm(LEN, MEAN, SD))
}

matfun <- function(NROW, NCOL, MEAN, SD) {
	return ( matrix( rnorm( (NROW*NCOL), MEAN, SD ), nrow = NROW, ncol = NCOL))
}

## Test 1 - small matrix w/ known fit

# load(file = "data/cereal.rda")
# vec <- as.numeric(cereal[,2])
# mat <- as.matrix(cereal[,3:6])
# lm <- new(LS, vec, mat)
# 
# res.df <- as.data.frame(
# 	res <- microbenchmark(
# 		lm$BestSubset( ),
# 		lm$ompBestSubset( ),
# 		times = 10000L
# 	)
# )
# 
# print(res)
# 
# ggplot(res.df, aes(x = expr, y = time)) + 
# 	geom_histogram(aes(fill))
# 	geom_boxplot(notch = TRUE, outlier.color = "red") + 
# 	scale_y_log10() + coord_trans(y = "log10")
# 
# autoplot(res)
# autoplot(res, log = FALSE)

### test 2 - small 10 columns x 1000 rows

set.seed(42)

lm <- new( LS )
lm$Y <- vecfun(1000, 100, 25)
lm$X <- matfun(1000, 10, 25, 5)

res <- microbenchmark(
	lm$BestSubset( ),
	lm$ompBestSubset( ),
	times = 1000L
)

print(res)

grid.arrange(
	autoplot(res) + 
		ggtitle("1000 rows by 10 columns @ 1,000 replications, log scale"), 
	autoplot(res, log=FALSE) + 
		ggtitle("1000 rows by 10 columns @ 1,000 replications"), 
	ncol = 1, 
	main = "RcppRegression Tests of C++ functions as intefaced through R"
)


### test 3 - medium 10 columns x 1000 rows

set.seed(424)

# lm <- new(LS)
lm$Y <- vecfun(1000, 100, 25)
lm$X <- matfun(1000, 15, 25, 5)

res <- microbenchmark(
	lm$BestSubset( ),
	lm$ompBestSubset( ),
	times = 50L
)

print(res)

grid.arrange(
	autoplot(res) + 
		ggtitle("1000 rows by 15 columns @ 100 replications, log scale"), 
	autoplot(res, log=FALSE) + 
		ggtitle("1000 rows by 15 columns @ 100 replications"), 
	ncol = 1, 
	main = "RcppRegression Tests of C++ functions as intefaced through R"
)

### test 4 - large 1000 rows x 15 columns

set.seed(4242)

lm$Y <- vecfun(1000, 100, 25)
lm$X <- matfun(1000, 15, 25, 5)

### long benchmark
res <- microbenchmark(
# 	lm$BestSubset( ),
	lm$ompBestSubset( ),
	times = 50L
)

print(res)

autoplot(res)
autoplot(res, log = FALSE)

### test 4 - large 1000 rows x 20 columns

vec <- vecfun(1000, 100, 25)
mat <- matfun(1000, 20, 25, 5)

lm <- new(LS, vec, mat)

### long benchmark
res <- microbenchmark(
	lm$BestSubset( ),
	lm$ompBestSubset( ),
	times = 1L
)

print(res)

autoplot(res)
autoplot(res, log = FALSE)


