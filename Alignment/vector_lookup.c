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

// Function to generate better random indices
void generate_random_indices(int* lookup_indices, int num_lookups, int vector_size)
{
  for (int i = 0; i < num_lookups; i++)
  {
    lookup_indices[i] = (int)((double)rand() / RAND_MAX * (vector_size - 1));
  }
}

// Function to perform element-wise addition for 3D vectors
Vec3 add_3D_vectors(Vec3 a, Vec3 b)
{
  Vec3 result;
  result.x = a.x + b.x;
  result.y = a.y + b.y;
  result.z = a.z + b.z;
  return result;
}

// Function to perform element-wise addition for 4D vectors
Vec4 add_4D_vectors(Vec4 a, Vec4 b)
{
  Vec4 result;
  result.x = a.x + b.x;
  result.y = a.y + b.y;
  result.z = a.z + b.z;
  result.w = a.w + b.w;
  return result;
}

// Function to perform random lookups and sum vectors in a 3D array
Vec3 lookup_and_add_3D_vector(Vec3* array, int* lookup_indices, int num_lookups)
{
  Vec3 sum = { 0.0f, 0.0f, 0.0f };
  for (int i = 0; i < num_lookups; i++)
  {
    int index = lookup_indices[i];
    sum = add_3D_vectors(sum, array[index]); // Element-wise addition
  }
  return sum;
}

// Function to perform random lookups and sum vectors in a 4D array
Vec4 lookup_and_add_4D_vector(Vec4* array, int* lookup_indices, int num_lookups)
{
  Vec4 sum = { 0.0f, 0.0f, 0.0f, 0.0f };
  for (int i = 0; i < num_lookups; i++)
  {
    int index = lookup_indices[i];
    sum = add_4D_vectors(sum, array[index]); // Element-wise addition
  }
  return sum;
}

// Function to time the random lookups and additions
void time_lookups(int vector_size, int num_lookups)
{
  // Allocate memory for vectors and lookup indices
  Vec3* vector_3D_array = (Vec3*)malloc(vector_size * sizeof(Vec3));
  Vec4* vector_4D_array = (Vec4*)malloc(vector_size * sizeof(Vec4));
  int* lookup_indices = (int*)malloc(num_lookups * sizeof(int));

  // Initialize the arrays
  init_3D_vector_array(vector_3D_array, vector_size);
  init_4D_vector_array(vector_4D_array, vector_size);

  // Generate random lookup locations
  generate_random_indices(lookup_indices, num_lookups, vector_size);

  // Perform random lookups and element-wise additions for 3D vectors
  clock_t start = clock();
  Vec3 sum_3D = lookup_and_add_3D_vector(vector_3D_array, lookup_indices, num_lookups);
  clock_t end = clock();
  double time_3D = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Sum of 3D vectors: (%f, %f, %f)\n", sum_3D.x, sum_3D.y, sum_3D.z);
  printf("Time taken for %d random lookups and additions in 3D array: %f seconds\n", num_lookups,
    time_3D);

  // Perform random lookups and element-wise additions for 4D vectors
  start = clock();
  Vec4 sum_4D = lookup_and_add_4D_vector(vector_4D_array, lookup_indices, num_lookups);
  end = clock();
  double time_4D = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Sum of 4D vectors: (%f, %f, %f, %f)\n", sum_4D.x, sum_4D.y, sum_4D.z, sum_4D.w);
  printf("Time taken for %d random lookups and additions in 4D array: %f seconds\n", num_lookups,
    time_4D);

  // Clean up
  free(vector_3D_array);
  free(vector_4D_array);
  free(lookup_indices);
}

int main()
{
  srand(time(NULL));

  // Define vector size of 1e6 and number of lookups
  int vector_size = 1000000;
  int num_lookups = 1000000; // Increased number of lookups for more meaningful timing

  // Time the lookups
  time_lookups(vector_size, num_lookups);

  return 0;
}
