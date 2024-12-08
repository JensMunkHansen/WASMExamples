#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Structure for a 3D vector
typedef struct
{
  float x, y, z;
} Vec3;

// Structure for a 4D vector
typedef struct
{
  float x, y, z, w;
} Vec4;

// Function to initialize a random 3D vector array
void init_3D_vector_array(Vec3* array, int size)
{
  for (int i = 0; i < size; i++)
  {
    array[i].x = (float)rand() / RAND_MAX;
    array[i].y = (float)rand() / RAND_MAX;
    array[i].z = (float)rand() / RAND_MAX;
  }
}

// Function to initialize a random 4D vector array
void init_4D_vector_array(Vec4* array, int size)
{
  for (int i = 0; i < size; i++)
  {
    array[i].x = (float)rand() / RAND_MAX;
    array[i].y = (float)rand() / RAND_MAX;
    array[i].z = (float)rand() / RAND_MAX;
    array[i].w = (float)rand() / RAND_MAX;
  }
}

// Function to perform a lookup in 3D array
void lookup_3D_vector(Vec3* array, int size, int index)
{
  if (index >= 0 && index < size)
  {
    printf("3D Vector at index %d: (%f, %f, %f)\n", index, array[index].x, array[index].y,
      array[index].z);
  }
  else
  {
    printf("Index out of bounds.\n");
  }
}

// Function to perform a lookup in 4D array
void lookup_4D_vector(Vec4* array, int size, int index)
{
  if (index >= 0 && index < size)
  {
    printf("4D Vector at index %d: (%f, %f, %f, %f)\n", index, array[index].x, array[index].y,
      array[index].z, array[index].w);
  }
  else
  {
    printf("Index out of bounds.\n");
  }
}

int main()
{
  srand(time(NULL));

  // Array sizes
  int size_3D = 1000;
  int size_4D = 1000;

  // Allocate memory for vectors
  Vec3* vector_3D_array = (Vec3*)malloc(size_3D * sizeof(Vec3));
  Vec4* vector_4D_array = (Vec4*)malloc(size_4D * sizeof(Vec4));

  // Initialize the arrays
  init_3D_vector_array(vector_3D_array, size_3D);
  init_4D_vector_array(vector_4D_array, size_4D);

  // Perform random lookups
  int random_index_3D = rand() % size_3D;
  int random_index_4D = rand() % size_4D;

  lookup_3D_vector(vector_3D_array, size_3D, random_index_3D);
  lookup_4D_vector(vector_4D_array, size_4D, random_index_4D);

  // Clean up
  free(vector_3D_array);
  free(vector_4D_array);

  return 0;
}
