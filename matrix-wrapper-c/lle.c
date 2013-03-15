#include <stdio.h>
#include <math.h>
#include <windows.h>

#include "../matrix_lib/matrix_lib.h"

#define MAX_BUFFER 64000000

typedef struct sort_elem_type{
	double data ;
	int index ;
}SORT_ELEM ;

void putdata(MAT *pM , char *filename) ;
void getdata(MAT *pM , char *filename) ;

void _qsort_SE(SORT_ELEM *a , int l , int r){
	int i , j ;
	SORT_ELEM tmp ;
	i = l ;
	j = r ;
	tmp = a[i] ;
	while ( i < j ){
		while ( i < j && tmp.data <= a[j].data ) j -- ;
		a[i] = a[j] ;
		while ( i < j && a[i].data <= tmp.data ) i ++ ;
		a[j] = a[i] ;
	}
	a[i] = tmp ;
	if ( l < i - 1 ) _qsort_SE(a , l , i - 1) ;
	if ( i + 1 < r ) _qsort_SE(a , i + 1 , r) ;
}

MAT _lle(MAT *pX , MAT *pK , MAT *pd){
	MAT Y , X2 , X2_T , distance , index , neighborhood , W , M , eig_v , X_T;
	MAT tmp , tmp2 , col , row ;
	int D , N , K , i , j , ii , d ;
	double tol , trace , sum ;
	SORT_ELEM *pSE ;

	//putdata(pX , "X.bin") ;
	//return Y ;

/*
function [Y] = lle(X,K,d)
[D,N] = size(X);
fprintf(1,'LLE running on %d points in %d dimensions\n',N,D);
*/
	D = pX->height ;
	N = pX->width ;
	printf("LLE running on %d points in %d dimensions\n" , N , D);
/*
% STEP1: COMPUTE PAIRWISE DISTANCES & FIND NEIGHBORS 
fprintf(1,'-->Finding %d nearest neighbours.\n',K);
*/
	K = _toint(pK->space[0][0]) ;
	printf("-->Finding %d nearest neighbours.\n" , K);
/*
X2 = sum(X.^2,1);%按照第一维度相加，即每列的数据相加，得到一行
distance = repmat(X2,N,1)+repmat(X2',1,N)-2*X'*X;%repmat(X2,N,1)，得到的是X2复制N行排成的矩阵
*/	

	X2 = m_create(1 , pX->width) ;
	for ( j = 0 ; j < pX->width ; j ++ ){
		X2.space[0][j] = 0.0 ;
		for ( i = 0 ; i < pX->height ; i ++ ){
			X2.space[0][j] += pX->space[i][j] * pX->space[i][j] ;
		}
	}
	
//	putdata(&X2 , "X2.bin") ;
//	return Y ;

	tmp = m_repmat(&X2 , N , 1) ;
	m_copy(&distance , &tmp) ;
	m_destroy(&tmp) ;

	X2_T = m_transpose(&X2) ;

	tmp = m_repmat(&X2_T , 1 , N) ;
	m_add(&distance , &tmp) ;
	m_destroy(&tmp) ;

	X_T = m_transpose(pX) ;
	tmp = m_multiply(&X_T , pX) ;
//	tmp = m_multiply(&X2_T , &X2) ;
	m_scale(&tmp , -2) ;
	
//	putdata(&tmp , "tmp.bin") ;

	m_add(&distance , &tmp) ;
	m_destroy(&tmp) ;

//	putdata(&distance , "distance.bin") ;
//	return Y;

/*
[sorted,index] = sort(distance);%将每列排序
neighborhood = index(2:(1+K),:);%提取2~1+K行的内容
%neighborhood是一个K*N的矩阵
*/
	
	index = m_sort(&distance) ;

//	putdata(&index , "index.bin") ;

	row = m_series(1 , K) ;
	col = m_series(0 , index.width - 1) ;
	neighborhood = m_get(&index ,&row , &col) ;
	m_destroy(&row) ;
	m_destroy(&col) ;

//	putdata(&neighborhood , "neighborhood.bin") ;
//	return Y;

/*
% STEP2: SOLVE FOR RECONSTRUCTION WEIGHTS
fprintf(1,'-->Solving for reconstruction weights.\n');
*/
	printf("-->Solving for reconstruction weights.\n");

/*
%标准化因子？
if(K>D) 
  fprintf(1,'   [note: K>D; regularization will be used]\n'); 
  tol=1e-3; % regularlizer in case constrained fits are ill conditioned
else
  tol=0;
end
*/
	if ( K > D ){
		printf("   [note: K>D; regularization will be used]\n");
		tol = 1e-3 ;
	}else{
		tol = 0.0 ;
	}

/*
W = zeros(K,N);%K*N的0矩阵
for ii=1:N
   %z是一个D*K的矩阵
   z = X(:,neighborhood(:,ii))-repmat(X(:,ii),1,K); % shift ith pt to origin
   %局部互协方差？？C是一个K*K的矩阵
   C = z'*z;                                        % local covariance
   %trace对角线元素的和，即特征值的和
   C = C + eye(K,K)*tol*trace(C);                   % regularlization (K>D)
   %这里有左除法，可能涉及到求逆运算-----W(:,ii)是K*1的矩阵
   W(:,ii) = C\ones(K,1);                           % solve Cw=1
   %单位化
   W(:,ii) = W(:,ii)/sum(W(:,ii));                  % enforce sum(w)=1
end;
*/
	W = m_zeros(K , N) ;
	
	for ( ii = 0 ; ii < N ; ii ++ ){
		MAT z , z_T , C ;

		/*z = X(:,neighborhood(:,ii))-repmat(X(:,ii),1,K); % shift ith pt to origin*/
		row = m_series(0 , neighborhood.height - 1) ;
		col = m_series(ii , ii) ;
		tmp = m_get(&neighborhood , &row , &col) ;
		m_destroy(&row) ;
		m_destroy(&col) ;

		m_copy(&col , &tmp) ;
		m_destroy(&tmp) ;
		row = m_series(0 , pX->height - 1) ;
		z = m_get(pX , &row , &col) ;
		m_destroy(&row) ;
		m_destroy(&col) ;

		row = m_series(0 , pX->height - 1) ;
		col = m_series(ii , ii) ;
		tmp2 = m_get(pX , &row , &col) ;
		m_destroy(&row) ;
		m_destroy(&col) ;
		tmp = m_repmat(&tmp2 , 1 , K) ;
		m_destroy(&tmp2) ;
		m_scale(&tmp , -1) ;

		m_add(&z , &tmp) ;
		m_destroy(&tmp) ;

		/*C = z'*z;                                        % local covariance*/
		z_T = m_transpose(&z) ;
		C = m_multiply(&z_T , &z) ;

		/*C = C + eye(K,K)*tol*trace(C);                   % regularlization (K>D)*/
		trace = 0 ;
		for ( i = 0 ; i < C.height ; i ++ ){
			trace += C.space[i][i] ;
		}
		tmp = m_eye(K , K) ;
		m_scale(&tmp , tol * trace) ;
		m_add(&C , &tmp) ;
		m_destroy(&tmp) ;

		/*W(:,ii) = C\ones(K,1);                           % solve Cw=1*/
		tmp2 = m_ones(K , 1) ;
		m_solve(&C , &tmp2 , &tmp) ;
		m_destroy(&tmp2) ;
		row = m_series(0 , W.height - 1) ;
		col = m_series(ii , ii) ;
		m_set(&W , &row , &col , &tmp) ;
		m_destroy(&row) ;
		m_destroy(&col) ;
		m_destroy(&tmp) ;
	
		/*W(:,ii) = W(:,ii)/sum(W(:,ii));                  % enforce sum(w)=1*/
		sum = 0.0 ;
		for ( i = 0 ; i < W.height ; i ++ ){
			sum += W.space[i][ii] ;
		}
		for ( i = 0 ; i < W.height ; i ++ ){
			W.space[i][ii] /= sum ;
		}
		
		m_destroy(&z) ;
		m_destroy(&z_T) ;
		m_destroy(&C) ;	
	}

//	putdata(&W , "W.bin") ;
//	return Y;

/*
% STEP 3: COMPUTE EMBEDDING FROM EIGENVECTS OF COST MATRIX M=(I-W)'(I-W)
fprintf(1,'-->Computing embedding.\n');
*/
	printf("-->Computing embedding.\n");

/*
% M=eye(N,N); % use a sparse matrix with storage for 4KN nonzero elements
%M为N*N的矩阵，应该不大啊，为什么要用稀疏矩阵呢
M = sparse(1:N,1:N,ones(1,N),N,N,4*K*N); 
for ii=1:N
   %W里面的一列
   w = W(:,ii);
   %neighborhood的对应列
   jj = neighborhood(:,ii);
   %减的是一行和一列的内容
   M(ii,jj) = M(ii,jj) - w';
   M(jj,ii) = M(jj,ii) - w;
   %减了一个子矩阵
   M(jj,jj) = M(jj,jj) + w*w';
end;
*/
	M = m_eye(N , N) ;
	for ( ii = 0 ; ii < N ; ii ++ ){
		MAT w , w_T , jj ;
		
		/*w = W(:,ii);*/
		row = m_series(0 , W.height - 1) ;
		col = m_series(ii , ii) ;
		w = m_get(&W , &row , &col) ;
		m_destroy(&row) ;
		m_destroy(&col) ;

		/*jj = neighborhood(:,ii);*/
		row = m_series(0 , neighborhood.height - 1) ;
		col = m_series(ii , ii) ;
		jj = m_get(&neighborhood , &row , &col) ;
		m_destroy(&row) ;
		m_destroy(&col) ;

		w_T = m_transpose(&w) ;

		/* M(ii,jj) = M(ii,jj) - w';*/
		row = m_series(ii ,  ii) ;
		tmp = m_get(&M , &row , &jj) ;

		m_copy(&tmp2 , &w_T) ;
		m_scale(&tmp2 , -1) ;
		m_add(&tmp , &tmp2) ;
		m_destroy(&tmp2) ;

		m_set(&M , &row , &jj , &tmp) ;
		m_destroy(&tmp) ;
		m_destroy(&row) ;

		/*M(jj,ii) = M(jj,ii) - w;*/
		col = m_series(ii , ii) ;
		tmp = m_get(&M , &jj , &col) ;

		m_copy(&tmp2 , &w) ;
		m_scale(&tmp2 ,  -1) ;
		m_add(&tmp , &tmp2) ;
		m_destroy(&tmp2) ;

		m_set(&M , &jj , &col , &tmp) ;
		m_destroy(&tmp) ;
		m_destroy(&col) ;

		/*M(jj,jj) = M(jj,jj) + w*w';*/
		tmp = m_get(&M , &jj , &jj) ;
		tmp2 = m_multiply(&w , &w_T) ;
		m_add(&tmp , &tmp2) ;
		m_destroy(&tmp2) ;

		m_set(&M , &jj , &jj , &tmp) ;
		m_destroy(&tmp) ;

		/*clearing the temporary memory*/
		m_destroy(&w_T) ;
		m_destroy(&w) ;
		m_destroy(&jj) ;
	}

//	putdata(&M , "M.bin") ;
//	return Y;

/*
% CALCULATION OF EMBEDDING
options.disp = 0; options.isreal = 1; options.issym = 1; 
[Y,eigenvals] = eigs(M,d+1,0,options)%Y得到了特征向量
%Y中存放的是M的前d+1个最接近0的特征值对应的特征向量。
%也就是绝对值最小的d+1个特征向量
%    EIGS(A,K,SIGMA)
%    If SIGMA is a real or complex scalar including 0, EIGS finds the
%    eigenvalues closest to SIGMA. 
%三个选项的意思是，不显示调试信息、M矩阵为实的、M矩阵是对称的
*/
	printf("begin --- m_eig , %d\n" , clock()) ;
	m_eig(&M , &eig_v , &tmp) ;
	
	//eig_v = m_create(M.height , M.width) ;
	//tmp = m_create(M.height , M.width) ;
	//getdata(&eig_v , "eig_v.bin") ;
	//getdata(&tmp , "eig_s.bin") ;
	
	printf("end --- m_eig , %d\n" , clock()) ;
	d = _toint(pd->space[0][0]) ;
	d ++ ;

	putdata(&eig_v , "eig_v.bin") ;
	putdata(&tmp , "eig_s.bin") ;

	pSE = (SORT_ELEM*) malloc(sizeof(SORT_ELEM) * eig_v.height) ;
	for ( i = 0 ; i < eig_v.height ; i ++ ){
		pSE[i].data = fabs(eig_v.space[i][i]) ;
		pSE[i].index = i ;
	}
	printf("begin -- qsort %d\n" , clock()) ;
	_qsort_SE(pSE , 0 , eig_v.height - 1) ;
	printf("end -- qsort %d\n" , clock()) ;

	

	Y = m_create(tmp.height , d) ;
	for ( j = 0 ; j < d ; j ++ ){
		for ( i = 0 ; i < tmp.height ; i ++ ){
//			Y.space[i][j] = tmp.space[i][pSE[j].index] ;
			Y.space[i][j] = tmp.space[i][pSE[d - j - 1].index] ;
		} 
	}
	//following line is meaningless
	//m_scale(&Y , -1) ;

	m_destroy(&tmp) ;
	m_destroy(&eig_v) ;
	free(pSE) ;

//	putdata(&Y , "Y.bin") ;
//	return Y ;

/*
%提取了部分列转置再乘
Y = Y(:,2:d+1)'*sqrt(N); % bottom evect is [1,1,1,1...] with eval 0
*/

	row = m_series(0 , Y.height - 1) ;
	col = m_series(1 , d - 1) ;
	tmp = m_get(&Y , &row , &col) ;
	m_destroy(&row) ;
	m_destroy(&col) ;
	m_scale(&tmp , sqrt(N)) ;
	m_destroy(&Y) ;
	Y = m_transpose(&tmp) ;
	m_destroy(&tmp) ;

/*
fprintf(1,'Done.\n');
*/
	printf("Done.\n") ;

	return Y ;
}

double buffer[MAX_BUFFER] ;

void getdata(MAT *pM , char *filename){
	FILE *fp ;
	int i , j ;
	fp = fopen(filename , "rb") ;
	fread(buffer , sizeof(double) , pM->height * pM->width , fp) ;
	for ( j = 0 ; j < pM->width ; j ++ ){
		for ( i = 0 ; i < pM->height ; i ++ ){
			pM->space[i][j] = buffer[j * pM->height + i] ;
		}
	}
	fclose(fp) ;
}

void putdata(MAT *pM , char *filename){
	FILE *fp ;
	int i , j ;
	fp = fopen(filename , "wb") ;
	for ( j = 0 ; j < pM->width ; j ++ ){
		for ( i = 0 ; i < pM->height ; i ++ ){
			buffer[j * pM->height + i] = pM->space[i][j] ;
		}
	}
	fwrite(buffer , sizeof(double) , pM->height * pM->width , fp) ;
	fclose(fp) ;
}

int main(){
	FILE *fp ;
	MAT X , K , d , Y;
	int h , w ;

/*	X = m_create(4 , 4) ;
	getdata(&X , "test.bin") ;
	m_print(&X) ;
	return 0 ;
*/

/*	X = m_eye(4 , 5) ;
	putdata(&X , "test.bin") ;
	m_print(&X) ;
	m_destroy(&X) ;
	return 0 ;
*/

/*	X = m_series(0 , 100) ;
	putdata(&X , "test.bin") ;
	return ;
*/

	fp = fopen("testdata\\description.txt" , "r") ;
	fscanf(fp , "%d%d" , &h , &w) ;
	X = m_create(h , w) ;
	fscanf(fp , "%d%d" , &h , &w) ;
	K = m_create(h , w) ;
	fscanf(fp , "%d%d" , &h , &w) ;
	d = m_create(h , w) ;
	fclose(fp) ;

	getdata(&X , "testdata\\X.bin") ;
	getdata(&K , "testdata\\K.bin") ;
	getdata(&d , "testdata\\d.bin") ;

	Y = _lle(&X , &K , &d) ;

	putdata(&Y , "output.bin") ;

	//WinExec("fc /B testdata\\Y.bin output.bin" , SW_NORMAL) ;
	return 0 ;
}

/*
int main(){
	int i , j ;
	double data_a[3][3] = {
		{1 , 3 , 5} ,
		{2 , 7 , 1} , 
		{9 , 2 , 4}
	} ;
	double data_b[3] = {5 , 1 , 2} ;
	MAT A , b , x , D , V;

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
	//m_inv(&A) ;
	m_print(&A) ;

	m_destroy(&A) ;
	m_destroy(&b) ;
	m_destroy(&x) ;
	m_destroy(&D) ;
	m_destroy(&V) ;

	return 0 ;
}
*/