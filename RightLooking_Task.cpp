/*
 * RightLooking_Task.cpp
 *
 *  Created on: 2016/03/31
 *      Author: stomo
 */

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <omp.h>

#include <CoreBlasTile.hpp>
#include <TMatrix.hpp>

using namespace std;

void tileLU( const int MT, const int NT, TMatrix& A, TMatrix& T )
{
	//////////////////////////////////////////////////////////////////////
	// List Item
	int **Ap;

	Ap = (int **)malloc(sizeof(int *) * MT);
	for (int i=0; i<MT; i++)
		Ap[i] = (int *)malloc(sizeof(int) * NT);

	//////////////////////////////////////////////////////////////////////
	// Right Looking tile QR Task version
	#pragma omp parallel
	{
		#pragma omp single
		{
			for (int k=0; k < min(MT,NT); k++ )
			{
				#pragma omp task depend(inout:Ap[k][k])
				{
					GEQRT( A(k,k), T(k,k) );

					#ifdef DEBUG
					#pragma omp critical
					cout << "GETRF(" << k << "," << k << "," << k << ")\n";
					#endif
				}

				for (int j=k+1; j < NT; j++)
				{
					#pragma omp task depend(in:Ap[k][k]) depend(inout:Ap[k][j])
					{
						LARFB( PlasmaLeft, PlasmaTrans, A(k,k), T(k,k), A(k,j) );

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
						TSQRT( A(k,k), A(i,k), T(i,k) );

						#ifdef DEBUG
						#pragma omp critical
						cout << "TSTRF(" << i << "," << k << "," << k << ")\n";
						#endif
					}

					for (int j=k+1; j < NT; j++)
					{
						#pragma omp task depend(in:Ap[i][k]) depend(inout:Ap[k][j],Ap[i][j])
						{
							SSRFB( PlasmaLeft, PlasmaTrans, A(i,k), T(i,k), A(k,j), A(i,j) );

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
