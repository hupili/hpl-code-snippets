/*
MATRIX LIB , for quick programming in C .
Efficiency is not in consideration . 

initiated when I have to translate a MATLAB file into C . 

functions:
create
destroy
copy
get
set
transpose
series
add
multiply
scale
repmat
sort
zeros
ones
eye
solve ( built on CLAPACK )
inv ( a wrapping of 'solve' for A*X=I)
eig ( built on CLAPACK )

MAT is the structure for my MATRIX.
remember to destroy it after the work is completed.

all parameter of MAT is in pointer style , 
while returning MAT is not .

I simply use double as the element type .

indices start from 0 .

it's disturbing that the CLIENT should keep in mind that 
MAT must be DESTROYED after using .
space allocation of returning MAT and OUTPUT MAT structure is done by the functions themself , 
while INPUT MAT should be properly created by the CLIENT .

------------code by hplonline

*/

#include <stdio.h>
#include <assert.h>

#include "matrix_lib.h"

/*
these lines are for VC6 .
please add these lib files for your own compiler
*/
#pragma comment(lib , "blas.lib") 
#pragma comment(lib , "clapack.lib") 

/*
since some functions in stdlib.h collide with ones in clapack.h" , 
I make these declarations mannually
*/
void *malloc(size_t n) ;
void free(void* p) ;

#include "f2c.h"
#include "clapack.h"

/*
memory is managed through an Iliffle vector.
so reference to the elements is rather simple :

  m.space[0][1] = 3 ;
*/

int _toint(double d){
	return (int)(d + 0.5) ;
}

MAT m_create(int iHeight , int iWidth){
	int i ;
	MAT tmp ;
	tmp.width = iWidth ;
	tmp.height = iHeight ;
	tmp.space = (double **)malloc(iHeight * sizeof(double*)) ;
	for ( i = 0 ; i < iHeight ; i ++ ){
		tmp.space[i] = (double *)malloc(iWidth * sizeof(double)) ;
	}
	return tmp ;
}

/*
return a row vector from begin to end
ie.
begin = 1 , end = 4 ;
return :
1 2 3 4 
*/
MAT m_series(int begin , int end){
	MAT tmp ;
	int i ;

	assert(end >= begin) ;

	tmp = m_create(1 , end - begin + 1) ;
	for ( i = 0 ; i < tmp.width ; i ++ ){
		tmp.space[0][i] = (double)(begin + i) ;	
	}
	return tmp ;
}

MAT m_transpose(MAT *pM){
	MAT tmp ;
	int i , j ;

	assert(pM->space != NULL) ;

	tmp = m_create(pM->width , pM->height) ;
	for ( i = 0 ; i < pM->height ; i ++ ){
		for ( j = 0 ; j < pM->width ; j ++ ){
			tmp.space[j][i] = pM->space[i][j] ;
		}
	}
	return tmp ;
}

void m_destroy(MAT *pM){
	int i ;

	assert(pM->space != NULL) ;

	for ( i = 0 ; i < pM->height ; i ++ ){
		free(pM->space[i]) ;
	}
	free(pM->space) ;
	pM->space = NULL ;
	pM->height = 0 ;
	pM->width = 0 ;
}

void m_copy(MAT *pMdst , MAT *pMsrc){
	int i , j ;

	assert(pMsrc->space != NULL) ;

	//m_destroy(pMdst) ;
	*pMdst = m_create(pMsrc->height , pMsrc->width) ;
	for ( i = 0 ; i < pMsrc->height ; i ++ ){
		for ( j = 0 ; j < pMsrc->width ; j ++ ){
			pMdst->space[i][j] = pMsrc->space[i][j] ;
		}
	}
}

/*
pRow and pCol must be row vectors 
pM->:
1 2 3 
4 5 6
7 8 9
pRow->:
0 2 
pCol->:
0 1
return:
1 2 
7 8
*/
MAT m_get(MAT *pM , MAT *pRow , MAT *pCol){
	MAT tmp , row , col;
	int i , j ;

	if ( pRow->height == 1 ){
		m_copy(&row , pRow) ;
	}else{
		row = m_transpose(pRow) ;
	}
	
	if ( pCol->height == 1 ){
		m_copy(&col , pCol) ;
	}else{
		col = m_transpose(pCol) ;
	}

	tmp = m_create(row.width , col.width) ;
	for ( i = 0 ; i < row.width ; i ++ ){
		for ( j = 0 ; j < col.width ; j ++ ){
			tmp.space[i][j] = 
				pM->space[_toint(row.space[0][i])][_toint(col.space[0][j])] ;
		}
	}

	m_destroy(&col) ;
	m_destroy(&row) ;

	return tmp ;
}

void m_set(MAT *pMdst , MAT *pRow , MAT *pCol , MAT *pMsrc){
	int i , j ;
	MAT row , col ;

	if ( pRow->height == 1 ){
		m_copy(&row , pRow) ;
	}else{
		row = m_transpose(pRow) ;
	}
	
	if ( pCol->height == 1 ){
		m_copy(&col , pCol) ;
	}else{
		col = m_transpose(pCol) ;
	}

	assert(row.width == pMsrc->height && col.width == pMsrc->width) ;

	for ( i = 0 ; i < row.width ; i ++ ){
		for ( j = 0 ; j < col.width ; j ++ ){
			pMdst->space[_toint(row.space[0][i])][_toint(col.space[0][j])]
				= pMsrc->space[i][j] ;
		}
	}

	m_destroy(&col) ;
	m_destroy(&row) ;
}

/* add *pMsrc to *pMdst */
void m_add(MAT *pMdst , MAT *pMsrc){
	int i , j ;

	assert(pMdst->height == pMsrc->height 
		&& pMdst->width == pMsrc->width) ;

	for ( i = 0 ; i < pMsrc->height ; i ++ ){
		for ( j = 0 ; j < pMsrc->width ; j ++ ){
			pMdst->space[i][j] += pMsrc->space[i][j] ;
		}
	}
}

/*
return *m1 multiply *m2
*/
MAT m_multiply(MAT *pM1 , MAT *pM2){	
	MAT tmp ;
	int i , j , k ;

	assert(pM1->width == pM2->height) ;

	tmp = m_create(pM1->height , pM2->width) ;
	for ( i = 0 ; i < tmp.height ; i ++ ){
		for ( j = 0 ; j < tmp.width ; j ++ ){
			tmp.space[i][j] = (double)0.0 ;
			for ( k = 0 ; k < pM1->width ; k ++ ){
				tmp.space[i][j] += 
					pM1->space[i][k] * pM2->space[k][j] ;
			}
		}
	}

	return tmp ;
}

/*
return k*A
*/
void m_scale(MAT *pM , double k){
	int i , j ;
	for ( i = 0 ; i < pM->height ; i ++ ){
		for ( j = 0 ; j < pM->width ; j ++ ){
			pM->space[i][j] *= k ;
		}
	}
}

/*
pm->:
1 2
3 4
repmat(pm , 1 , 2)->:
1 2 
3 4
1 2
3 4
*/

MAT m_repmat(MAT *pM , int iHeight , int iWidth){
	int i , j , ii , jj;
	MAT tmp ;
	
	assert(pM->space != NULL) ;

	tmp = m_create(pM->height * iHeight , pM->width * iWidth) ;
	for ( i = 0 ; i < iHeight ; i ++ ){
		for ( j = 0 ; j < iWidth ; j ++ ){
			for ( ii = 0 ; ii < pM->height ; ii ++ ){
				for ( jj = 0 ; jj < pM->width ; jj ++ ){
					tmp.space[i * pM->height + ii][j * pM->width + jj]
						= pM->space[ii][jj] ;
				}
			}
		}
	}

	return tmp ;
}

/*
sort each column
return indice

using select sorting 
*/

void _m_qsort(MAT *pM , MAT *pIndex , int col , int l , int r){
	int i , j ;
	double tmp , pivot , pivot_i ;
	i = l ;
	j = r ;
	pivot = pM->space[i][col] ;
	pivot_i = pIndex->space[i][col] ;
	while ( i < j ){
		while ( i < j && pivot <= pM->space[j][col] ) j -- ;
		pM->space[i][col] = pM->space[j][col] ;
		pIndex->space[i][col] = pIndex->space[j][col] ;
		while ( i < j && pM->space[i][col] <= pivot ) i ++ ;
		pM->space[j][col] = pM->space[i][col] ;
		pIndex->space[j][col] = pIndex->space[i][col] ;
	}
	pM->space[i][col] = pivot ;
	pIndex->space[i][col] = pivot_i ;
	if ( l < i - 1 ) _m_qsort(pM , pIndex , col , l , i - 1) ;
	if ( i + 1 < r ) _m_qsort(pM , pIndex , col , i + 1 , r) ;
}

MAT m_sort(MAT *pM){
	int i , j , k ;
	MAT  index ;
	
	index = m_create(pM->height , pM->width) ;
	for ( j = 0 ; j < pM->width ; j ++ ){
		for ( i = 0 ; i < pM->height ; i ++ ){
			index.space[i][j] = i ;
		}
		_m_qsort(pM , &index , j , 0 , pM->height - 1) ;
	} 

	return index ;
}

MAT m_zeros(int iHeight , int iWidth){
	MAT tmp ;
	int i , j ;
	tmp = m_create(iHeight , iWidth) ;
	for ( i = 0 ; i < iHeight ; i ++ ){
		for ( j = 0 ; j < iWidth ; j ++ ){
			tmp.space[i][j] = 0 ;
		}
	}
	return tmp ;
}

MAT m_ones(int iHeight , int iWidth){
	MAT tmp ;
	int i , j ;
	tmp = m_create(iHeight , iWidth) ;
	for ( i = 0 ; i < iHeight ; i ++ ){
		for ( j = 0 ; j < iWidth ; j ++ ){
			tmp.space[i][j] = 1 ;
		}
	}
	return tmp ;
}

MAT m_eye(int iHeight , int iWidth){
	MAT tmp ;
	int i , j ;
	tmp = m_create(iHeight , iWidth) ;
	for ( i = 0 ; i < iHeight ; i ++ ){
		for ( j = 0 ; j < iWidth ; j ++ ){
			if ( i ==j ) {
				tmp.space[i][j] = 1 ;
			}else{
				tmp.space[i][j] = 0 ;
			}
		}
	}
	return tmp ;
}

void m_print(MAT *pM){
	int i , j ;
	printf("%d * %d :\n" , pM->height , pM->width) ;
	for ( i = 0 ; i < pM->height ; i ++ ){
		for ( j = 0 ; j < pM->width ; j ++ ){
			printf("%8.2f" , pM->space[i][j]) ;
		}
		printf("\n") ;
	}
}

/*
Ax=b
return x
*/
int m_solve(MAT *pA , MAT *pb , MAT *px){
	integer n , nrhs , lda , *ipiv , ldb , info ;
	doublereal *a , *b;
	int i , j ;
    
	n = pA->width ;
	nrhs = pb->width ;
	lda = pA->height ;
	ldb = pb->height ;
	ipiv = (integer*) malloc(sizeof(integer) * n) ;

	a = (doublereal*) malloc(sizeof(doublereal) * n * lda) ;
	for ( j = 0 ; j < n ; j ++ ){
		for ( i = 0 ; i < lda ; i ++ ){
			a[lda * j + i] = pA->space[i][j] ;
		}
	}

	b = (doublereal*) malloc(sizeof(doublereal) * nrhs * ldb) ;
	for ( j = 0 ; j < nrhs ; j ++ ){
		for ( i = 0 ; i < ldb ; i ++ ){
			b[ldb * j + i] = pb->space[i][j] ;
		}
	}

	dgesv_(&n, &nrhs, a, &lda, ipiv, b, &ldb, &info) ;

	if ( info != 0 ) return info ;

/*	printf("\ntesting ipiv:\n") ;
	for ( i = 0 ; i < n ; i ++ ){
		printf("%d " , ipiv[i]) ;
	}
	printf("\ntesting ipiv end\n\n") ;

	printf("\ntesting A\n") ;
	for ( i = 0 ; i < lda ; i ++ ){
		for ( j = 0 ; j < n ; j ++ ){
			printf("%8.2f" , a[j * lda + i]) ;
		}
		printf("\n") ;
	}
	printf("\ntesting matrix A end..\n\n") ;*/

	*px = m_create(n , nrhs) ;
	for ( j = 0 ; j < nrhs ; j ++ ){
		for ( i = 0 ; i < n ; i ++ ){
			px->space[i][j] = b[j * n + i] ;
		}
	}

	free(ipiv) ;
	free(a) ;
	free(b) ;
	return info ;
}

/*
inverse *pM
*/

void m_inv(MAT *pM){
	MAT I , x ;
	I = m_eye(pM->height , pM->height) ;
	m_solve(pM , &I , &x) ;
	m_copy(pM , &x) ;
	m_destroy(&I) ;
	m_destroy(&x) ;
}

/* 
return eigenvectors in *pV and eigenvalues in *pD
only tested for *pM is square
*/

int m_eig(MAT *pM , MAT *pD , MAT *pV){
	int i , j ;
	char jobvl , jobvr;
	integer n , lda , info , ldvr , ldvl , lwork ;
	doublereal *a , *wr , *wi , *vl , *vr , *work;

	jobvl = 'N' ;
	jobvr = 'V' ;
	n = pM->width ;
	lda = pM->height ;

	wr = (doublereal*)malloc( sizeof(doublereal) * n) ;
	wi = (doublereal*)malloc( sizeof(doublereal) * n) ;	

	ldvr = pM->height ;
	vr = (doublereal*)malloc( sizeof(doublereal) * n * ldvr) ;
	ldvl = ldvr ;
	vl = vr ;

	a = (doublereal*) malloc(sizeof(doublereal) * n * lda) ;
	for ( j = 0 ; j < n ; j ++ ){
		for ( i = 0 ; i < lda ; i ++ ){
			a[lda * j + i] = pM->space[i][j] ;
		}
	}

/*	ldvl = 3 ;
	vl = (doublereal*)malloc( sizeof(doublereal) * n * ldvl) ;*/

	lwork = n * 40 ;
	work = (doublereal*)malloc( sizeof(doublereal) * lwork) ;

	dgeev_(&jobvl, &jobvr, &n, a,  &lda, wr, wi, vl , &ldvl , vr, &ldvr, work, &lwork, &info) ;

	if ( info != 0 ) return info ;

	*pD = m_create(n , n) ;
	for ( j = 0 ; j < n ; j ++ ){
		for ( i = 0 ; i < n ; i ++ ){
			if ( i == j ){
				pD->space[i][j] = wr[i] ;
			}else{
				pD->space[i][j] = 0.0 ;
			}
		}
	}

	*pV = m_create(n , n) ;
	for ( j = 0 ; j < n ; j ++ ){
		for ( i = 0 ; i < n ; i ++ ){
			pV->space[i][j] = vr[j * n + i] ;
		}
	}

	free(a) ;
	free(wr) ;
	free(wi) ;
	free(work) ;
	free(vr) ;

	return info ;

/*	printf("info:%d\n" , info) ;
	printf("D = \n") ;
	for ( i = 0 ; i < n ; i ++ ){
		for ( j = 0 ; j < n ; j ++ ){
			if ( i == j ) printf("%10.5f" , wr[i]) ;
			else printf("%10.5f" , 0.0) ;
		}
		printf("\n") ;
	}
	printf("Vl = \n") ;
	for ( i = 0 ; i < n ; i ++ ){
		for ( j = 0 ; j < n ; j ++ ){
			printf("%10.5f" , vl[n * j + i]) ;
		}
		printf("\n") ;
	}
	printf("Vr = \n") ;
	for ( i = 0 ; i < n ; i ++ ){
		for ( j = 0 ; j < n ; j ++ ){
			printf("%10.5f" , vr[n * j + i]) ;
		}
		printf("\n") ;
	}
*/

}

/*
tests of :
eigs , solve , inv , sort
*/

/*
int main(){
	int i , j ;
	double data_a[3][3] = {
		{1 , 3 , 5} ,
		{2 , 7 , 1} , 
		{9 , 2 , 4}
	} ;
	double data_b[3] = {5 , 1 , 2} ;
	MAT A , b , x , D , V , index;

	A = m_create(3 , 3) ;
	for ( i = 0 ; i < 3 ; i ++ ){
		for ( j = 0 ; j < 3 ; j ++ ){
			A.space[i][j] = data_a[i][j] ;
		}
	}
	b = m_create(3 , 1) ;
	for ( i = 0 ; i < 3 ; i ++ ){
		b.space[i][0] = data_b[i] ;
	}

	printf("A=\n") ;
	m_print(&A) ;
	printf("b=\n") ;
	m_print(&b) ;

	m_eig(&A , &D , &V) ;
	printf("D=\n") ;
	m_print(&D) ;
	printf("V=\n") ;
	m_print(&V) ;

	m_solve(&A , &b ,&x) ;
	printf("solving A*x = b\n") ;
	printf("x=\n") ;
	m_print(&x) ;

	m_inv(&A) ;
	printf("inversing A\n") ;
	printf("A=") ;
	m_print(&A) ;

	index = m_sort(&A) ;
	printf("sorting A:\n") ;
	m_print(&A) ;
	printf("index is:\n") ;
	m_print(&index) ;

	m_destroy(&index) ;
	m_destroy(&A) ;
	m_destroy(&b) ;
	m_destroy(&x) ;
	m_destroy(&D) ;
	m_destroy(&V) ;

	return 0 ;
}

*/

/*
tests of :
get , set , series
*/

/*
int main(){
	int i , j ;
	MAT m1 , m2 , col , row ;

	m1 = m_create(3 , 3) ;
	for ( i = 0 ; i < 3 ; i ++ ){
		for ( j = 0 ; j < 3 ; j ++ ){
			m1.space[i][j] = i * 3 + j + 1 ;
		}
	}
	m_print(&m1) ;

	col = m_create(1 , 2) ;
	col.space[0][0] = 0 ;
	col.space[0][1] = 1 ;

	row = m_create(1 , 2) ;
	row.space[0][0] = 0 ;
	row.space[0][1] = 2 ;

	m2 = m_get(&m1 , &row ,  &col) ;
	m_print(&m2) ;

	m_destroy(&m2) ;
	m2 = m_zeros(2,2) ;
	col.space[0][1] = 2 ;
	m_set(&m1 , &row , &col , &m2) ;
	m_print(&m1) ;

	m_destroy(&col) ;
	m_destroy(&m2) ;
	col = m_series(0 , 2) ;
	m2 = m_get(&m1 , &row  , &col) ;
	m_print(&m2) ;

	m_destroy(&m1) ;
	m_destroy(&m2) ;
	m_destroy(&col) ;
	m_destroy(&row) ;

	return 0 ;
}
*/

/*
basic tests :
create ,destroy , copy , print ,eye ,ones ,zeros ,repmat ,add ,multiply , scale , transpose
*/

/*
int main(){
	MAT m , mm , a , b , c;
	int i , j ;

	m = m_zeros(2 , 3) ;
	m.space[1][2] = 1 ;
	m_print(&m) ;
	mm = m_repmat(&m , 3 , 2) ;
	m_print(&mm) ;
	m_destroy(&mm) ;
	m_destroy(&m) ;

	m = m_ones(6 , 5) ;
	m_print(&m) ;
	m_destroy(&m) ;

	m = m_eye(3 , 4) ;
	m_print(&m) ;
	m_destroy(&m) ;

	m = m_eye(4 , 3) ;
	m_print(&m) ;
	m_destroy(&m) ;

	a = m_ones(2 , 3) ;
	b = m_ones(3 , 2) ;
	m_print(&a) ;
	m_print(&b) ;
	c = m_multiply(&a , &b) ;
	m_print(&c) ;
	m_destroy(&a) ;
	m_destroy(&b) ;
	m_destroy(&c) ;

	a = m_ones(2 , 3) ;
	b = m_ones(2 , 3) ;
	m_scale(&b , 1.7) ;
	m_print(&b) ;
	m_add(&a , &b) ;
	m_print(&a) ;

	m_destroy(&b) ;
	b = m_transpose(&a) ;
	m_print(&b) ;
	m_destroy(&a) ;
	m_destroy(&b) ;

	return 0 ;
}
*/