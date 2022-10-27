// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len <= 0) {
        throw invalid_argument("index below zero");
        //cout << "Error!";

    }

    BitLen = len;
    MemLen = len >> 5;
    if (BitLen % 32)
        MemLen++;

    pMem = new TELEM[MemLen];
    memset(pMem, 0, MemLen * sizeof(TELEM));



}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    
    pMem = new TELEM[MemLen];

    memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField::~TBitField()
{
    delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("bit n is out of range");
    return n >> 5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("bit n is out of range");

    return 1 << (n & ((1 << 5) - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("bit n is out of range");

    int i = GetMemIndex(n);
    int j = GetMemMask(n);

    pMem[i] = pMem[i] | j;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("bit n is out of range");


    int i = GetMemIndex(n);
    int j = GetMemMask(n);

    pMem[i] = pMem[i] & (~j);

}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("bit n is out of range");

    int i = GetMemIndex(n);
    int j = GetMemMask(n);


    return pMem[i] & j;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this == &bf)
        return *this;

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    delete[]pMem;

    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];

    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;

    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i])
            return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    if (*this == bf)
        return 0;

    return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int t;
    if (BitLen > bf.BitLen) {
        t = BitLen;
    }
    else {
        t = bf.BitLen;
    }

    TBitField tmp(t);

    int i = 0;

    while (i < MemLen && i < bf.MemLen) {
        tmp.pMem[i] = pMem[i] | bf.pMem[i];
        i++;
    }

    while (i < MemLen) {
        tmp.pMem[i] = pMem[i];
        i++;
    }

    while (i < bf.MemLen) {
        tmp.pMem[i] = bf.pMem[i];
        i++;
    }

    return tmp;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
   
    int mn, mx;

    if (BitLen > bf.BitLen) {
        mx = BitLen;
        mn = bf.MemLen;
    }
    else {
        mx = bf.BitLen;
        mn = MemLen;
    }


    TBitField tmp(mx);

    int i = 0;

    while (i < mn) {
        tmp.pMem[i] = pMem[i] & bf.pMem[i];
        i++;
    }


    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(BitLen);

    int i = 0;

    for (i = 0; i < MemLen; i++)
        tmp.pMem[i] = ~pMem[i];

    int t = BitLen & ((1 << 5) - 1);
    tmp.pMem[i - 1] = tmp.pMem[i - 1] & ((1 << t) - 1);

    return tmp;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    int l = bf.GetLength();

    for (int i = 0; i < l; i++) {

        if (bf.GetBit(i))
            ostr << '1';
        else
            ostr << '0';
    }
    return ostr;
}
