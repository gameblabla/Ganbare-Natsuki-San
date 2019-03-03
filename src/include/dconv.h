/*==========================================================================
 *
 *  Copyright (C) 1998 Jumpei Isshiki. All Rights Reserved.
 *
 *  File	:dconv.h
 *  Content	:データ変換定義
 *	Program	:T.Sato / J.Issiki
 *	Date	:1998/5/13
 *	UpDate	:2002/11/14
 *
 ==========================================================================*/

#ifndef	_DCONV_INCLUDED_
#define	_DCONV_INCLUDED_

#include "general.h"

/*----------------------------------------------------------------------*/
/*	定数の定義															*/
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/*	構造体の宣言														*/
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/*	外部参照変数の宣言													*/
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/*	外部参照関数の宣言													*/
/*----------------------------------------------------------------------*/
extern	uint16_t	DCNVrand( uint16_t );					/* 指定範囲の乱数を取得する			*/
extern	void	DCNVinitRand( uint16_t );				/* 乱数の発生系列初期化				*/
extern	void	DCNVshuffleDim( int,uint16_t * );		/* テーブルシャッフル				*/
extern	void	DCNVshuffleMain( int,uint16_t * );	/* テーブルシャッフル本体			*/
extern	int32_t	MOTsin( int16_t );					/* sin(a)を計算する					*/
extern	int32_t	MOTcos( int16_t );					/* cos(a)を計算する					*/
extern	int16_t	MOTatan( int32_t,int32_t );			/* (posX,posY)への角度を計算する	*/


/*----------------------------------------------------------------------*/
#endif					/* end of Dconv.h	*/
/*----------------------------------------------------------------------*/
