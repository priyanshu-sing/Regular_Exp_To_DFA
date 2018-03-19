#include<stdio.h>
#include<string.h>
#define MAX 20
struct nfa_state
{
  int a, b, eps1, eps2;
} NFA[20];
struct dfa_state
{
  int state[20], a[20], b[20];
} DFA[20];

int cur, initial_state, final_state;

char stack[MAX];
int top=-1;
void push (char val)
{
  stack[++top] = val;
}

char pop ()
{
  return stack[top--];
}

int priority (char op)
{
  switch (op)
    {
    case '+':
      return 1;
    case '.':
      return 2;
    case '*':
      return 3;
    }
  return 0;
}

void init_nfa_table ()
{
  int i;
  for (i = 0; i < 20; i++)
    {
      NFA[i].a = NFA[i].b = -1;
      NFA[i].eps1 = NFA[i].eps2 = -1;
    }
}
void symbol (char c)
{
  if (c == 'a')
    NFA[cur].a = cur + 1;

  if (c == 'b')
    NFA[cur].b = cur + 1;

  push (cur);
  push (cur + 1);

  cur += 2;
}

void concat ()
{
  int first1, first2, last1, last2;

  last2 = pop ();
  first2 = pop ();
  last1 = pop ();
  first1 = pop ();

  NFA[last1].eps1 = first2;

  push (first1);
  push (last2);
}

void parallel ()
{
  int first1, first2, last1, last2;

  last2 = pop ();
  first2 = pop ();
  last1 = pop ();
  first1 = pop ();

  NFA[cur].eps1 = first1;
  NFA[cur].eps2 = first2;
  NFA[last1].eps1 = cur + 1;
  NFA[last2].eps2 = cur + 1;

  push (cur);
  push (cur + 1);
  cur += 2;
}

void closure ()
{
  int first, last;

  last = pop ();
  first = pop ();

  NFA[cur].eps1 = first;
  NFA[cur].eps2 = cur + 1;
  NFA[last].eps1 = first;
  NFA[last].eps2 = cur + 1;

  push (cur);
  push (cur + 1);

  cur += 2;
}

void construct_nfa (char *postfix)
{
  int i = 0;

  top = -1;

  for (i = 0; postfix[i] != '\0'; i++)
    {
      switch (postfix[i])
	{
	case 'a':
	case 'b': symbol (postfix[i]);
	          break;
        case '.': concat ();
	          break;
	case '+':
	         parallel ();
	  break;
	case '*':
	  closure ();
	}
    }
  final_state = pop ();
  initial_state = pop ();
}

void disp_NFA ()
{
  int i;
  printf ("\nstate\ta\tb\tî");
  for (i = 0; i < cur; i++)
    {
      if (i == initial_state)
	printf ("\n->%d", i);
      else if (i == final_state)
	printf ("\n* %d", i);
      else
	printf ("\n %d", i);

      if (NFA[i].a == -1)
	printf ("\t-");
      else
	printf ("\t{%d}", NFA[i].a);

      if (NFA[i].b == -1)
	printf ("\t-");
      else
	printf ("\t{%d}", NFA[i].b);

      if (NFA[i].eps1 != -1)
	{
	  printf ("\t{%d", NFA[i].eps1);
	  if (NFA[i].eps2 != -1)
	    {
	      printf (",%d", NFA[i].eps2);
	    }
	  printf ("}");
	}
      else
	printf ("\t-");
    }
}

void init_dfa_table ()
{
  int i, j;
  for (i = 0; i < 20; i++)
    {
      for (j = 0; j < 20; j++)
	{
	  DFA[i].state[j] = -1;
	  DFA[i].a[j] = -1;
	  DFA[i].b[j] = -1;
	}
    }
}

void print_state (int t[])
{
  int i = 0;
  printf ("[");
  for (i = 0; t[i] != -1; i++)
    printf ("%d,", t[i]);
  printf ("\b]");
}

int
isPresent (int T[], int v)
{
  int i;
  for (i = 0; T[i] != -1; i++)
    if (T[i] == v)
      return 1;
  return 0;
}

void disp_DFA (int n)
{
  int i;
  printf ("\nstate\t\t\ta\t\t\tb");
  for (i = 0; i <= n; i++)
    {
      printf ("\n");
      if (i == 0)
	printf ("->");

      if (isPresent (DFA[i].state, final_state))
	printf ("*");


      print_state (DFA[i].state);
      printf ("\t\t");

      if (DFA[i].a[0] != -1)
	print_state (DFA[i].a);
      else
	printf ("Dead State");
      printf ("\t\t");
      if (DFA[i].b[0] != -1)
	print_state (DFA[i].b);
      else
	printf ("Dead State");
    }
}

void
epsilon_closure (int T[], int t[])
{
  int i, v;
  top = -1;

  for (i = 0; t[i] != -1; i++)
    push (t[i]);

  i = 0;

  while (top != -1)
    {
      v = pop ();

      if (isPresent (T, v) == 0)
	{
	  T[i++] = v;
	}

      if (NFA[v].eps1 != -1)
	{
	  push (NFA[v].eps1);
	}

      if (NFA[v].eps2 != -1)
	{
	  push (NFA[v].eps2);
	}
    }
}

void
init_t (int t[])
{
  int i;
  for (i = 0; i < 20; i++)
    t[i] = -1;
}

int
search (int n, int t2[])
{
  int i, j;
  for (i = 0; i <= n; i++)
    {
      for (j = 0; t2[j] != -1; j++)
	if (isPresent (DFA[i].state, t2[j]) == 0)
	  break;
      if (t2[j] == -1)
	return 1;
    }
  return 0;
}

void
copy (int t1[], int t2[])
{
  int i;
  for (i = 0; t2[i] != -1; i++)
    t1[i] = t2[i];
}
int stack1[20];
char pop1()
 {
   return stack1[top--];
  }
void push1(char data)
{
  stack1[++top]=data;
}
void postfixexp(char postfix[],char c[])
{ 
  top=-1;
  int i=0,j,k=0,l=strlen(c);
  while(c[i]!='\0')
   {
       if(c[i]=='a'||c[i]=='b')
           postfix[k++]=c[i];
       if(c[i]=='(')
           push1(c[i]);
       if(c[i]=='+'||c[i]=='*'||c[i]=='*')
           {
              while(top>=0&&priority(stack1[top])>=priority(c[i]))
                 postfix[k++]=pop1();
              push1(c[i]);
            }
        if(c[i]==')')
          {
             while(top>=0&&stack1[top]!='(')
               postfix[k++]=pop1();
             pop1();
          }
      i++;
    }
  while(top>=0)
   postfix[k++]=pop1();
  postfix[k]='\0';
}
                   
int main ()
{
  char postfix[20],c[20];
  int t[20], v;
  int n = 0, i = 0, j, k;
  printf ("\nEnter Regular Expression: ");
  scanf ("%s", c);
  postfixexp(postfix,c);
  printf ("\nPostfix Expression: %s", postfix);
  init_nfa_table ();
  construct_nfa (postfix);
  init_dfa_table ();
  init_t (t);
  t[0] = initial_state;
  epsilon_closure (DFA[0].state, t);
  init_t (t);
  for (j = 0, k = 0; DFA[0].state[j] != -1; j++)
    {
      v = DFA[0].state[j];
      if (NFA[v].a != -1)
	{
	  if (isPresent (t, NFA[v].a) == 0)
	    t[k++] = NFA[v].a;
	}
    }

  epsilon_closure (DFA[0].a, t);

  init_t (t);

  for (j = 0, k = 0; DFA[0].state[j] != -1; j++)
    {
      v = DFA[0].state[j];
      if (NFA[v].b != -1)
	{
	  if (isPresent (t, NFA[v].b) == 0)
	    t[k++] = NFA[v].b;
	}
    }
  epsilon_closure (DFA[0].b, t);

  for (i = 0; i <= n; i++)
    {
      if (search (n, DFA[i].a) == 0)
	{
	  n++;
	  copy (DFA[n].state, DFA[i].a);

	  init_t (t);

	  for (j = 0, k = 0; DFA[n].state[j] != -1; j++)
	    {
	      v = DFA[n].state[j];

	      if(NFA[v].a != -1)
		{
		  if(isPresent (t, NFA[v].a) == 0)
		    t[k++] = NFA[v].a;
		}
	    }

	  epsilon_closure (DFA[n].a, t);

	  init_t (t);

	  for (j = 0, k = 0; DFA[n].state[j] != -1; j++)
	    {
	      v = DFA[n].state[j];
	      if (NFA[v].b != -1)
		{
		  if (isPresent (t, NFA[v].b) == 0)
		    t[k++] = NFA[v].b;
		}
	    }
	  epsilon_closure (DFA[n].b, t);

	}

      if(search (n, DFA[i].b) == 0)
	{
	  n++;
	  copy (DFA[n].state, DFA[i].b);

	  init_t (t);
	  for (j = 0, k = 0; DFA[n].state[j] != -1; j++)
	    {
	      v = DFA[n].state[j];

	      if (NFA[v].a != -1)
		{
		  if (isPresent (t, NFA[v].a) == 0)
		    t[k++] = NFA[v].a;
		}
	    }
	  epsilon_closure (DFA[n].a, t);

	  init_t (t);
	  for (j = 0, k = 0; DFA[n].state[j] != -1; j++)
	    {
	      v = DFA[n].state[j];
	      if (NFA[v].b != -1)

		{
		  if (isPresent (t, NFA[v].b) == 0)
		    t[k++] = NFA[v].b;
		}
	    }
	  epsilon_closure (DFA[n].b, t);
	}
    }
  disp_DFA (n);
}
