//
//  main.cpp
//
//  Created by T. Suzuki on 2014/01/05.
//  Copyright (c) 2013 T. Suzuki. All rights reserved.
//

#include <iostream>
#include <omp.h>
#include <cassert>
#include <cstdlib>

#include <TMatrix.hpp>
#include <PivMatrix.hpp>

using namespace std;

void tileLU( const int MT, const int NT, TMatrix& A, TMatrix& L, PivMatrix& P );

int main(int argc, const char * argv[])
{
	if (argc < 5)
	{
		cerr << "Usage: a.out [M] [N] [NB] [IB]\n";
		exit (1);
	}
	
	const int M =  atoi(argv[1]);  // n. of rows of the matrix
	const int N =  atoi(argv[2]);  // n. of columns of the matrix
	const int NB = atoi(argv[3]);  // tile size
	const int IB = atoi(argv[4]);  // inner blocking size

	assert( M >= N );
	assert( NB >= IB );
	
	#ifdef DEBUG
	cout << "M = " << M << ", N = " << N << ", NB = " << NB << ", IB = " << IB << endl;
//	cout << "clock precision = " << omp_get_wtick() << endl;
	#endif

	//////////////////////////////////////////////////////////////////////
	// Definitions and Initialize
	TMatrix A(M,N,NB,NB,IB);

	const int MT = A.mt();
	const int NT = A.nt();

	TMatrix L(MT*IB,NT*NB,IB,NB,IB); //
	PivMatrix P(MT,NT,NB); // Pivot

	// Initialize matrix A
	A.Set_Rnd( 20140105 );

	// Definitions and Initialize　END
	//////////////////////////////////////////////////////////////////////

//    #ifdef DEBUG
//	A.File_Out("orig");
//    #endif

	// Timer start
//	double time = omp_get_wtime();
	
	//////////////////////////////////////////////////////////////////////
	// tile QR variants
	tileLU(MT,NT,A,L,P);
	//////////////////////////////////////////////////////////////////////
	
	// Timer stop
//	time = omp_get_wtime() - time;
//	cout << M << ", " << NB << ", " << IB << ", " << time << endl;

//    #ifdef DEBUG
//	A.File_Out("orig");
//    #endif
	// Definitions and Initialize　END
	//////////////////////////////////////////////////////////////////////

	// Timer start
	double time = omp_get_wtime();
	
	//////////////////////////////////////////////////////////////////////
	// tile QR variants
	tileLU(MT,NT,A,L,P);
	//////////////////////////////////////////////////////////////////////
	
	// Timer stop
	time = omp_get_wtime() - time;
	cout << M << ", " << NB << ", " << IB << ", " << time << endl;

//    #ifdef DEBUG
//	A.File_Out("factored");
//    #endif

	return EXIT_SUCCESS;
}
