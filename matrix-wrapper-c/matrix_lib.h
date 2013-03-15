
#ifndef __H_MATRIX_LIB_
#define __H_MATRIX_LIB_

typedef struct mat_type{
	int height ;
	int width ;
	double **space ;
}MAT ;

int _toint(double d) ;

MAT m_create(int iHeight , int iWidth);
void m_destroy(MAT *pM);
void m_copy(MAT *pMdst , MAT *pMsrc);
MAT m_get(MAT *pM , MAT *pRow , MAT *pCol);
void m_set(MAT *pMdst , MAT *pRow , MAT *pCol , MAT *pMsrc);
void m_add(MAT *pMdst , MAT *pMsrc);
MAT m_multiply(MAT *pM1 , MAT *pM2);
void m_scale(MAT *pM , double k);
MAT m_repmat(MAT *pM , int iHeight , int iWidth);
MAT m_sort(MAT *pM);
MAT m_zeros(int iHeight , int iWidth);
MAT m_ones(int iHeight , int iWidth);
MAT m_eye(int iHeight , int iWidth);
void m_print(MAT *pM);
int m_solve(MAT *pA , MAT *pb , MAT *px) ;
void m_inv(MAT *pM);
int m_eig(MAT *pM , MAT *pD , MAT *pV);
MAT m_series(int begin , int end);
MAT m_transpose(MAT *pM) ;

#endif 