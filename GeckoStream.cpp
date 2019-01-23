
// For full license terms please see the LICENSE file distributed with this
// source code

#include "GeckoStream.h"

template <class T>
GeckoStream<T>::GeckoStream(const unsigned int ARRAY_SIZE, int device)
{

  array_size = ARRAY_SIZE;

  #pragma gecko config file("gecko.conf")

  // Set up data region on device
  #pragma gecko memory allocate(a[0:array_size]) type(T) location(exec_location) 
  #pragma gecko memory allocate(b[0:array_size]) type(T) location(exec_location) 
  #pragma gecko memory allocate(c[0:array_size]) type(T) location(exec_location) 
}

template <class T>
GeckoStream<T>::~GeckoStream()
{
  // End data region on device
  #pragma gecko memory free(a,b,c)  
}

template <class T>
void GeckoStream<T>::init_arrays(T initA, T initB, T initC)
{
  unsigned int array_size = this->array_size;
  T * restrict a = this->a;
  T * restrict b = this->b;
  T * restrict c = this->c;
  // #pragma acc parallel loop present(a[0:array_size], b[0:array_size], c[0:array_size]) wait

  #pragma gecko region at(exec_location) exec_pol(exec_pol_chosen) variable_list(a,b,c)
  for (int i = 0; i < array_size; i++)
  {
    a[i] = initA;
    b[i] = initB;
    c[i] = initC;
  }
  #pragma gecko region end

  #pragma gecko region pause at(exec_location) 
}

template <class T>
void GeckoStream<T>::read_arrays(std::vector<T>& h_a, std::vector<T>& h_b, std::vector<T>& h_c)
{
  T *a = this->a;
  T *b = this->b;
  T *c = this->c;

  h_a.clear();
  h_b.clear();
  h_c.clear();
  h_a.resize(array_size);
  h_b.resize(array_size);
  h_c.resize(array_size);

  T *v;
  v = (T*) h_a.data();
  #pragma gecko memory register(v[0:array_size]) type(T) loc("LocN")
  #pragma gecko memory copy from(a[0:array_size]) to(v[0:array_size])
  v = (T*) h_b.data();
  #pragma gecko memory register(v[0:array_size]) type(T) loc("LocN")
  #pragma gecko memory copy from(b[0:array_size]) to(v[0:array_size])
  v = (T*) h_c.data();
  #pragma gecko memory register(v[0:array_size]) type(T) loc("LocN")
  #pragma gecko memory copy from(c[0:array_size]) to(v[0:array_size])


  // for(int i=0;i<array_size;i++) {
  //   h_a[i] = a[i];
  //   h_b[i] = b[i];
  //   h_c[i] = c[i];
  // }
}

template <class T>
void GeckoStream<T>::copy()
{
  // unsigned int array_size = this->array_size;
  // T * restrict a = this->a;
  // T * restrict c = this->c;
  // #pragma acc parallel loop present(a[0:array_size], c[0:array_size]) wait
  // for (int i = 0; i < array_size; i++)
  // {
  //   c[i] = a[i];
  // }
}

template <class T>
void GeckoStream<T>::mul()
{
  // const T scalar = startScalar;

  // unsigned int array_size = this->array_size;
  // T * restrict b = this->b;
  // T * restrict c = this->c;
  // #pragma acc parallel loop present(b[0:array_size], c[0:array_size]) wait
  // for (int i = 0; i < array_size; i++)
  // {
  //   b[i] = scalar * c[i];
  // }
}

template <class T>
void GeckoStream<T>::add()
{
  // unsigned int array_size = this->array_size;
  // T * restrict a = this->a;
  // T * restrict b = this->b;
  // T * restrict c = this->c;
  // #pragma acc parallel loop present(a[0:array_size], b[0:array_size], c[0:array_size]) wait
  // for (int i = 0; i < array_size; i++)
  // {
  //   c[i] = a[i] + b[i];
  // }
}

template <class T>
void GeckoStream<T>::triad()
{
  const T scalar = startScalar;

  unsigned int array_size = this->array_size;
  T * restrict a = this->a;
  T * restrict b = this->b;
  T * restrict c = this->c;

  // #pragma acc parallel loop present(a[0:array_size], b[0:array_size], c[0:array_size]) wait
  #pragma gecko region exec_pol(exec_pol_chosen) variable_list(a,b,c)
  for (int i = 0; i < array_size; i++)
  {
    a[i] = b[i] + scalar * c[i];
  }
  #pragma gecko region end

  #pragma gecko region pause at(exec_location) 
}

template <class T>
T GeckoStream<T>::dot()
{
  // T sum = 0.0;

  // unsigned int array_size = this->array_size;
  // T * restrict a = this->a;
  // T * restrict b = this->b;
  // #pragma acc parallel loop reduction(+:sum) present(a[0:array_size], b[0:array_size]) wait
  // for (int i = 0; i < array_size; i++)
  // {
  //   sum += a[i] * b[i];
  // }

  // return sum;

  return 0;
}

void listDevices(void)
{
  // Get number of devices
  int count = acc_get_num_devices(acc_device_nvidia);

  // Print device list
  if (count == 0)
  {
    std::cerr << "No devices found." << std::endl;
  }
  else
  {
    std::cout << "There are " << count << " devices." << std::endl;
  }
}

std::string getDeviceName(const int)
{
  return std::string("Device name unavailable");
}

std::string getDeviceDriver(const int)
{
  return std::string("Device driver unavailable");
}
template class GeckoStream<float>;
template class GeckoStream<double>;
