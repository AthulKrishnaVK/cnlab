#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Link 
{
  int hop, dist, wt;
};

struct Network
{
  int H, L;
  struct Link* link;
};

int main()
{
  int H, L, S, i, j;
  
  printf("Distance Vector Routing using Bellman Ford Algorithm:\n\nEnter the number of nodes: ");
  scanf("%d", &H);
  
  printf("\nEnter the number of links: ");
  scanf("%d", &L);
  
  struct Network* n = (struct Network*)malloc(sizeof(struct Network));
  
  n->H = H;
  n->L = L; // Fixed: Assign the number of links to struct member L
  
  n->link = (struct Link*)malloc(n->L * sizeof(struct Link));
  
  for (i = 0; i < L; i++)
  {
    printf("\nLink %d: Enter source, destination, and weight: ", i + 1);
    scanf("%d %d %d", &n->link[i].hop, &n->link[i].dist, &n->link[i].wt);
  }
  
  printf("\nEnter the source node: ");
  scanf("%d", &S);

  int dist[H];
  
  for (i = 0; i < H; i++)
  {
    dist[i] = INT_MAX;
  }
  dist[S] = 0;
  
  for (i = 0; i < H - 1; i++) // Changed: H - 1 iterations for Bellman-Ford
  {
    for (j = 0; j < L; j++)
    {
      int u = n->link[j].hop;
      int v = n->link[j].dist;
      int wt = n->link[j].wt;
      
      if (dist[u] != INT_MAX && dist[u] + wt < dist[v])
      {
        dist[v] = dist[u] + wt;
      }
    }
  }
  
  // Check for negative weight cycle
  for (i = 0; i < L; i++)
  {
    int u = n->link[i].hop;
    int v = n->link[i].dist;
    int wt = n->link[i].wt;
    
    if (dist[u] != INT_MAX && dist[u] + wt < dist[v])
    {
      printf("Network contains negative weight cycle\n\n");
      return 0; // Exit program if negative weight cycle found
    }
  }
  
  printf("\nNode\tDistance from source\n");
  for (i = 0; i < H; i++)
  {
    printf("%d\t%d\n", i, dist[i]);
  }
  
  return 0;
}

