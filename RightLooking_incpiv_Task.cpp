/*
 * RightLooking_incpiv_Task.cpp
 *
 *  Created on: 2017/02/08
 *      Author: stomo
 */

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <omp.h>

#include <TMatrix.hpp>
#include <PivMatrix.hpp>
#include <CoreBlasTile.hpp>

using namespace std;

void tileLU( TMatrix& A, TMatrix& L, PivMatrix& P )
{
	const int MT = A.mt();
	const int NT = A.nt();

	//////////////////////////////////////////////////////////////////////
	// List Item
	int **Ap;

	Ap = (int **)malloc(sizeof(int *) * MT);
	for (int i=0; i<MT; i++)
		Ap[i] = (int *)malloc(sizeof(int) * NT);

	//////////////////////////////////////////////////////////////////////
	// Right Looking tile LU with incremental pivoting
	// Task version
	#pragma omp parallel
	{
		#pragma omp single
		{
			for (int k=0; k < min(MT,NT); k++ )
			{
				#pragma omp task depend(inout:Ap[k][k])
				{
					GETRF( A(k,k), P(k,k) );

					#ifdef DEBUG
					#pragma omp critical
					cout << "GETRF(" << k << "," << k << "," << k << ")\n";
					#endif
				}

				for (int j=k+1; j < NT; j++)
				{
					#pragma omp task depend(in:Ap[k][k]) depend(inout:Ap[k][j])
					{
						GESSM( A(k,k), A(k,j), P(k,k) );

						#ifdef DEBUG
						#pragma omp critical
						cout << "GESSM(" << k << "," << j << "," << k << ")\n";
						#endif
					}
				}

				for (int i=k+1; i < MT; i++)
				{
					#pragma omp task depend(inout:Ap[k][k]) depend(out:Ap[i][k])
					{
						TSTRF( A(k,k), A(i,k), L(i,k), P(i,k) );

						#ifdef DEBUG
						#pragma omp critical
						cout << "TSTRF(" << i << "," << k << "," << k << ")\n";
						#endif
					}

					for (int j=k+1; j < NT; j++)
					{
						#pragma omp task depend(in:Ap[i][k]) depend(inout:Ap[k][j],Ap[i][j])
						{
							SSSSM( L(i,k), A(i,k), A(k,j), A(i,j), P(i,k) );

							#ifdef DEBUG
							#pragma omp critical
							cout << "SSSSM(" << i << "," << j << "," << k << ")\n";
							#endif
						}
					} // j-LOOP END
				} // i-LOOP END
			} // k-LOOP END
		} // parallel section END
	}
	// Right Looking tile QR END
	//////////////////////////////////////////////////////////////////////

	for (int i=0; i<MT; i++)
		free(Ap[i]);
	free(Ap);
}
