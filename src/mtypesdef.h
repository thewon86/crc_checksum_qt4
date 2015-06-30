#ifndef MYTYPEDEF_H
#define MYTYPEDEF_H

#include <QDebug>

#ifdef _MAPPDEBUG
#define STR_X(x)    #x
#define MDEBUG(x)   qDebug() << "LINE:" << __LINE__ << "[" <<STR_X(x) << "=" << x << "]" << __FILE__
#define MDEBUGHEX(x)   qDebug() << "LINE:" << __LINE__ << "[" <<STR_X(x) << "=" << showbase  << hex << x << noshowbase << "]" << __FILE__
#define MMASSGE(x)    qDebug() << "LINE:" << __LINE__ << "[" << STR_X(x) << "]" << __FILE__

#endif

typedef unsigned char	BYTE;
typedef unsigned char*	PBYTE;
typedef unsigned short	WORD;
typedef unsigned int	DWORD;
typedef  int			LONG;

#define swab16(x) \
    ((unsigned short)( \
        (((unsigned short)(x) & (unsigned short)0x00ffU) << 8) | \
        (((unsigned short)(x) & (unsigned short)0xff00U) >> 8) ))

#define swab32(x) \
    ((unsigned int)( \
        (((unsigned int)(x) & (unsigned int)0x000000ffUL) << 24) | \
        (((unsigned int)(x) & (unsigned int)0x0000ff00UL) <<  8) | \
        (((unsigned int)(x) & (unsigned int)0x00ff0000UL) >>  8) | \
        (((unsigned int)(x) & (unsigned int)0xff000000UL) >> 24) ))
//#pragma pack(1)

#endif // MYTYPEDEF_H
