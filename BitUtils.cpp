#include "BitUtils.h"

int setBitNToX(int num, int n, bool x)
{
    return (num & ~(1 << n)) | ((static_cast<int>(x)) << n);
}
