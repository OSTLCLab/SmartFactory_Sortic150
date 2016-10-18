bool ArrayIsEqual(byte a[8], byte b[8])
{
  for(int i = 0; i<7; i++)
  {
    if(a[i] != b[i])
    {
      return false;
    }
  }
  return true;
}
