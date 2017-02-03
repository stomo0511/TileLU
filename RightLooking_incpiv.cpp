//
//  RightLooking
//
//  Created by T. Suzuki on 2014/01/05.
//  Copyright (c) 2013 T. Suzuki. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <omp.h>

#include <TMatrix.hpp>
#include <PivMatrix.hpp>
#include <CoreBlasTile.hpp>

using namespace std;

void tileLU( const int MT, const int NT, TMatrix& A, TMatrix& L, PivMatrix& P )
{
	for (int k=0; k < min(MT,NT); k++ )
	{
		#pragma omp parallel
		{
			#pragma omp single
			{
				GETRF( A(k,k), P(k,k) );

				#ifdef DEBUG
                #pragma omp critical
				cout << "GETRF(" << k << "," << k << "," << k << ")\n";
				#endif
			}

			#pragma omp for
			for (int j=k+1; j < NT; j++)
			{
				GESSM( A(k,k), A(k,j), P(k,k) );

				#ifdef DEBUG
				#pragma omp critical
				cout << "GESSM(" << k << "," << j << "," << k << ")\n";
				#endif
			} // j-LOOP END

			for (int i=k+1; i < MT; i++)
			{
				#pragma omp single
				{
					TSTRF( A(k,k), A(i,k), L(i,k), P(i,k));

					#ifdef DEBUG
                    #pragma omp critical
					cout << "TSTRF(" << i << "," << k << "," << k << ")\n";
					#endif
				}

				#pragma omp for
				for (int j=k+1; j < NT; j++)
				{
					SSSSM( L(i,k), A(i,k), A(k,j), A(i,j), P(i,k) );

					#ifdef DEBUG
					#pragma omp critical
					cout << "SSSSM(" << i << "," << j << "," << k << ")\n";
					#endif
				} // j-LOOP END
			} // i-LOOP END
		} // parallel section END
	} // k-LOOP END
}
