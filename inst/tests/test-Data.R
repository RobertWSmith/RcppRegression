# library(testthat)

context("Data Class")

test_that("Data Loads as Expected", {
	load("cereal.Rdata")
	vec <- as.numeric(cereal[,2])
	mat <- as.matrix(cereal[,3:6])
	mat1 <- cbind(rep(1, nrow(mat)), mat)
	
	d <- new(Data, vec, mat)
	
	expect_that(d$Y, equals(vec))
	expect_that(d$X, equals(mat))
	expect_that(d$subsetX(1:4), equals(mat1))

	expect_that((mat == d$X), is_true())
	expect_that((d$subsetX(1:4) == mat1), is_true())
	expect_that((d$nrow.X == nrow(mat)), is_true())
	expect_that((d$nrow.Y == length(vec)), is_true())
	expect_that((d$ncol.X == ncol(mat)), is_true())
	expect_that((d$ncol.Y == 1), is_true())
})
