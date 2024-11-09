// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

#define BITS_IN_ONE_MEM (sizeof(TELEM) * 8)

TBitField::TBitField(int len)
{
    if (len < 0) throw std::out_of_range("error len");
    BitLen = len;
    MemLen = BitLen / BITS_IN_ONE_MEM + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) 
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) 
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return (n / BITS_IN_ONE_MEM);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n % BITS_IN_ONE_MEM);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n > BitLen) 
    {
        throw std::out_of_range("error bit");
    }
    TELEM index = GetMemIndex(n);
    pMem[index] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen) 
    {
        throw std::out_of_range("error bit");
    }
    TELEM index = GetMemIndex(n);
    pMem[index] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > BitLen) 
    {
        throw std::out_of_range("error bit");
    }
    TELEM index = GetMemIndex(n);
    return (pMem[index] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    delete[] pMem;
    this->BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (MemLen == bf.MemLen)
    {
        for (int i = 0; i < MemLen; ++i)
        {
            if (pMem[i] != bf.pMem[i])
            {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen)
    {
        return 1;
    }
    for (TELEM i = 0; i < BitLen; ++i)
    {
        if (this->GetBit(i) != bf.GetBit(i))
        {
            return 1;
        }
    }
    return 0;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    TBitField BitField(max(BitLen, bf.BitLen));
    for (TELEM i = 0; i < BitField.MemLen; i++) 
    {
        if (i < MemLen) 
        {
            if (i < bf.MemLen) 
            {
                BitField.pMem[i] = pMem[i] | bf.pMem[i]; 
            }
            else 
            {
                BitField.pMem[i] = pMem[i];
            }
        }
        else 
        {
            if (i < bf.MemLen) 
            {
                BitField.pMem[i] = bf.pMem[i];
            }
        }
    }
    return BitField;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"  
{
    TELEM size = max(BitLen, bf.BitLen);
    TBitField BitField(size);
    for (TELEM i = 0; i < BitField.MemLen; i++)
    {
        if ((i <= MemLen) && (i <= bf.MemLen))
            BitField.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return BitField;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField BitField(BitLen);
    for (TELEM i = 0; i < BitLen; i++)
        if ((this->GetBit(i))) BitField.ClrBit(i);
        else BitField.SetBit(i);
    return BitField;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    char data;
    for (int i = 0; i < bf.BitLen; i++)
    {
        istr >> data;
        if (data == '1')
        {
            bf.SetBit(i);
        }
        else if (data == '0')
        {
            bf.ClrBit(i);
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (TELEM i = 0; i < bf.BitLen; i++)
    {
        if (bf.GetBit(i)) cout << i << " ";
    }
    cout << endl;
    return ostr;
}