#include "a3.h"

matrix read_data(const char* fp)
{
  FILE *file = fopen(fp, "r");
  if (!file)
    printf("Couldn't open file %s.\n", fp); // TODO: then die?

  // get number of elements
  int i = 0,
      r = fscanf(file, "%*f %*f %*f\n");
  while (r != EOF)
  {
    i++;
    r = fscanf(file, "%*f %*f %*f\n");
  }

  int N = i;

  // load data in Nx2 matrix
  matrix dat = null_matrix(N,2);
  rewind(file);
  for (i = 0; i < N; i++)
  {
    fscanf(file, "%lf %lf\n", &MatrixVAL(dat,i,0), &MatrixVAL(dat,i,1));
  }

  return dat;
}

matrix init_F(const matrix dat, int N, base_funct f)
{
  matrix F = null_matrix(N, N);
  int i, j;
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      double sum = 0;
      int k;
      for (k = 0; k < dat.N; k++)
      {
        double x       = MatrixGET(dat,k,0),
               y       = MatrixGET(dat,k,1),
               delta_y = sqrt(y);
        sum += 1 / pow(delta_y*2, 2) * f(i, x) * f(j, x);
      }
      MatrixSET(F, i, j, sum);
    }
  }
  return F;
}

vector init_b(matrix dat, int N, base_funct f)
{
  vector b = null_vector(N);
  int i;
  for (i = 0; i < N; i++)
  {
    double sum = 0;
    int k;
    for (k = 0; k < dat.N; k++)
    {
      double x       = MatrixGET(dat,k,0),
             y       = MatrixGET(dat,k,1),
             delta_y = sqrt(y);
      sum += 1 / pow(delta_y*2, 2) * f(i, x) * y;
    }
    VectorSET(b, i, sum);
  }
  return b;
}

double polynom(int l, double x)
{
  return pow(x,l);
}

double legendre_polynom(int l, double x)
{
  if (l == 0)
    return x;
  else if (l == 1)
    return x;
  else
    // rekursion
    return (2*l-1)/l * x * legendre_polynom(l-1,x) - (l-1)/l * legendre_polynom(l-2,x);
}