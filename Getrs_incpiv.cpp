//
//  Getrs RightLooking Tile version
//
//  Created by T. Suzuki on 2017/02/08.
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

void tileGetrs( TMatrix& A, TMatrix& L, PivMatrix& P, TMatrix& B )
{
	const int aMT = A.mt();
	const int aNT = A.nt();
	const int bNT = B.nt();

	assert(aMT == B.mt());

	//////////////////////////////////////////////////////////////////////
	// trsmpl: triangular solve with unit lower triangular matrix (L)


	//////////////////////////////////////////////////////////////////////
	// Right Looking tile LU with incremental pivoting
	for (int k=0; k < min(aMT,aNT); k++ )
	{
		#pragma omp parallel
		{
			#pragma omp for
			for (int j=k; j < bNT; j++)
			{
				GESSM( A(k,k), B(k,j), P(k,k) );

				#ifdef DEBUG
				#pragma omp critical
				cout << "GESSM(" << k << "," << j << "," << k << ")\n";
				#endif
			} // j-LOOP END

			for (int i=k+1; i < aMT; i++)
			{
				#pragma omp for
				for (int j=k+1; j < bNT; j++)
				{
					SSSSM( L(i,k), A(i,k), B(k,j), B(i,j), P(i,k) );

					#ifdef DEBUG
					#pragma omp critical
					cout << "SSSSM(" << i << "," << j << "," << k << ")\n";
					#endif
				} // j-LOOP END
			} // i-LOOP END
		} // parallel section END
	} // k-LOOP END

	//////////////////////////////////////////////////////////////////////
	// trsm: triangular solve with upper triangular matrix (U)

}
