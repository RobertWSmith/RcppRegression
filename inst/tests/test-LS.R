# library(testthat)

context("Data Class")

matfun <- function(NR, NC, MEAN, SD) {
	return( as.matrix( rnorm( NR * NC , (MEAN/25), SD ), nrow = NR, ncol = NC ) )
}

vecfun <- function(NR, MEAN, SD) {
	return( as.numeric( rnorm( NR, MEAN, SD ) ) )
}

test_that("Data Loads as Expected", {
	set.seed(42)
	
	NC <- 5
	NR <- 100
	MEAN <- 100
	SD <- 15
	
	vec <- vecfun( NR, MEAN, SD ) 
	mat <- matfun( NR, NC, MEAN, SD ) 
	
	control.ls <- new(LS, vec, mat)
	test.omp.ls <- new(LS, vec, mat)
	
	control.ls$BestSubset()
	test.omp.ls$ompBestSubset()
	
	expect_that(control.ls$data, is_equivalent_to(test.omp.ls$data))
	
})