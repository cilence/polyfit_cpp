//
//  PolyfitEigen.hpp
//  Polyfit
//
//  Created by Patrick Löber on 23.11.18.
//  Copyright © 2018 Patrick Loeber. All rights reserved.
//
//  Use the Eigen library for fitting: http://eigen.tuxfamily.org
//  See https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html for different methods

#include "Eigen/Dense"

template<typename T>
std::vector<T> polyfit_Eigen(const std::vector<T> &xValues, const std::vector<T> &yValues, const int degree, const std::vector<T>& weights = std::vector<T>(), bool useJacobi = true)
{
    using namespace Eigen;
    
    bool useWeights = weights.size() > 0 && weights.size() == xValues.size();
    
    int numCoefficients = degree + 1;
    size_t nCount = xValues.size();
    
    MatrixXf X(nCount, numCoefficients);
    MatrixXf Y(nCount, 1);
    
    // fill Y matrix
    for (size_t i = 0; i < nCount; i++)
    {
        if (useWeights)
            Y(i, 0) = yValues[i] * weights[i];
        else
            Y(i, 0) = yValues[i];
    }
    
    // fill X matrix (Vandermonde matrix)
    for (size_t nRow = 0; nRow < nCount; nRow++)
    {
        T nVal = 1.0f;
        for (int nCol = 0; nCol < numCoefficients; nCol++)
        {
            if (useWeights)
                X(nRow, nCol) = nVal * weights[nRow];
            else
                X(nRow, nCol) = nVal;
            nVal *= xValues[nRow];
        }
    }
    
    VectorXf coefficients;
    if (useJacobi)
        coefficients = X.jacobiSvd(ComputeThinU | ComputeThinV).solve(Y);
    else
        coefficients = X.colPivHouseholderQr().solve(Y);
    
    return std::vector<T>(coefficients.data(), coefficients.data() + numCoefficients);
}

/*
 Calculates the value of a polynomial of degree n evaluated at x. The input
 argument coefficients is a vector of length n+1 whose elements are the coefficients
 in incremental powers of the polynomial to be evaluated.

 param:
 coefficients       polynomial coefficients generated by polyfit() function
 xValues            x axis values

 return:
 Fitted Y values.
*/
template<typename T>
std::vector<T> polyval( const std::vector<T>& coefficients, const std::vector<T>& xValues )
{
    size_t nCount = xValues.size();
    size_t nDegree = coefficients.size();
    std::vector<T> yValues( nCount );

    for (size_t i = 0; i < nCount; i++ )
    {
        T yVal = 0;
        T xPowered = 1;
        T xVal = xValues[i];
        for (size_t j = 0; j < nDegree; j++ )
        {
            // multiply current x by coefficient
            yVal += coefficients[j] * xPowered;
            // power up the X
            xPowered *= xVal;
        }
        yValues[i] = yVal;
    }

    return yValues;
}