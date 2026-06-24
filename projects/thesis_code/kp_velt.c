#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <float.h>

#define MAX_WEIGHT 20
#define MAX_WEIGHT_LEN 4

//1 tupwnei plhorfories enw 0 oxi
#define SHOW_INFO 0

//epistrefei to megalutero apo ta A , B
#define max(A,B) ((A) > (B) ? (A) : (B))

/*********************************************************************/
/*       DOMES GIA DIATHRHSH PLHROFORIWN KATASTASHS SYSTHMATOS       */
/*********************************************************************/
struct state {
  double cost;
  int host;
  int id;
  struct state *machine_next;
  struct state *machine_prev;
  struct state *move_next;
};

struct machine_state {
  double load;
  struct state *machine;
  struct state *machine_tail;
  struct state machine_head;
};





/*********************************************************************/
/*                  SYNARTHSEIS XEIRISMOU LISTWN                     */
/*********************************************************************/
void create(struct machine_state *list);
struct state *listsort(struct state *list, double **W, char comp); 
struct state *sorted_insert(struct state *list, struct state *job, double **W, char comp); 
struct state *delete(struct state *tail, struct state *prev);
struct state *move_insert(struct state *tail, struct state *job); 
struct state return_first(struct state *list); 
struct state *machine_insert(struct state *tail, struct state *job, double **W, int policy); 
struct state *delete_any(struct state *tail, struct state *job); 




/*********************************************************************/
/*                  BASIKES SYNARTHSEIS & ALGORI8MOI                 */
/*********************************************************************/
/* Basikes Synarthseis */
void weight_init(double **W, int n, int m, int machine_type, char system_ini);
void init(struct state *s, struct machine_state *ms, double **W, int n, int m, int policy,
          char system_ini);
double **file_init(double **W, struct state *s, struct machine_state *ms, int n, int m, int policy, char *file);
double cost(struct state *s, double **W, struct machine_state *ms, int job, int machine, int policy);
int want2migrate(int job, struct state *s, struct machine_state *ms, double **W, int n, int m, int policy);
int coalition_want2migrate(struct state *s, struct machine_state *ms, double **W, int n, int m, char min_max);
void move(int job, int m1, int m2, struct state *s, struct machine_state *ms, double **W, int n, int m, int policy);

/* Bohthhtikes Synarthseis */  
void print_state(struct state *s, struct machine_state *ms, int n, int m);
void print_move_list(struct machine_state *ms, int m);
void printW(double **W, int n, int m);

/* Algori8moi kentrikou elegkth */
int min_max_weight_job (struct state *s, struct machine_state *ms, double **W, int n, int m, int policy, char comp); 
int fifo(struct state *s, struct machine_state *ms, double **W, int n, int m, int policy); 
int Random(struct state *s, struct machine_state *ms, double **W, int n, int m, int policy); 

/* Peiramata */
void weight_a(double **W, int n, int m);
void weight_b(double **W, int n, int m);
void weight_c(double **W, int n, int m);
void weight_d(double **W, int n, int m);
void weight_e(double **W, int n, int m);
void weight_min_worst(double **W, int n, int m);
void weight_sjf_worst(double **W, int n, int m);


/*******************************************************************/
/*                              MAIN                               */
/*******************************************************************/

int main(int argc,char * argv[])
{
/* Systhma m mhxanwn kai n ergasiwn. 
 to 1o orisma einai to m
 to 2o orisma einai to n
 to 3o orisma einai to eidos tou algori8mou tou kentrikou elegkth
   1 : Max Weight Job Algorithm
   2 : Min Weight Job Algorithm
   3 : FIFO Algori8m        
   4 : Random Algori8m 
 to 4o orisma einai h politikh xrewshs ths ka8e mhxanhs
   1 : FIFO
   2 : Makespan
 to 5o orisma einai o tropos pou arxikopoieitai to systhma
   r : tyxaia arxikopoihsh
   i : mia tyxaia arxikh mhxanh[machine]
   m : manual arxikopoihsh
 to 6o orisma einai to eidos twn mhxanwn
   i : identical
   r : related
   u : unrelated
 to 7o orisma einai o tropos pou arxikopoiountai ta varh twn ergasiwn
   m : eisodos apo plhktrologio 
   f : apo arxeiofp = fopen("a.txt","r");
   [a,b,c,d,e] : kathgories varwn gia peiramata
 to 8o orisma einai o tropos pou epilegontai ta coalitions (an yparxoun)
   i : to mikrotero coalition prwto 
   a : to megalytero coalition prwto 
*/

     
struct state *s;     	  /*  s : pinakas me ta states twn ergasiwn */
struct machine_state *ms; /* ms : pinakas me ta states twn mhxanwn */

double **W; /* W : periexei to varos ths ka8e ergasias gia ka8e mhxanh (mege8os n x m)*/

int i,             /* metrhtes */
    steps = 0,     /* ari8mos bhmatwn gia NE */
    algorithm,     /* eidos algori8mou elegkth, to pairnoume ws orisma (1-4) */
    policy,        /* eidos politikhs xrewshs twn ergasiwn  sth mhxanh (1,2)*/
    n, /* n : plh8os ergasiwn */
    m, /* m : plh8os mhxanwn */
    coalition = 0, /* boolean metavlhth gia uparxei h oxi coalition kinhshs */
    num_of_flips = 0; /* plh8os twn 2-flips */
char machine_type, /* eidos mhxanwn i : identical , r : related , u : unrelated */ 
     system_ini,   /* tropos arxikopihshs ergasiwn stis mhxanes 
                      r: tyxaia arxikopoihsh, i: mia arxikh mhxanh[machine], m: apo ton xrhsth*/
     weight_ini,    /* tropos arxikopoihshs varwn f: arxeio, m: stdi, else: tyxaia */
     coalition_type;/* min h' max coalition */      

//Desmeysh mnhmhs gia tous pinakes
if (argc == 9){
  m = atoi((char *) argv[1]);
  n = atoi((char *) argv[2]);
  W = (double **) calloc(n,sizeof(double *));
  for (i = 0;i < n;i++)
    W[i] = (double *) calloc(m,sizeof(double));
  s = (struct state *) calloc(n,sizeof(struct state));
  ms = (struct machine_state *) calloc(m+1,sizeof(struct machine_state));
  algorithm = atoi((char *) argv[3]);
  policy = atoi((char *) argv[4]);
  system_ini = *argv[5];
  machine_type = *argv[6];
  weight_ini = *argv[7];
  coalition_type = *argv[8];
} else {
  printf("Lathos orismata\n");
  printf("Orisma 1: Plh8os mhxanwn (m)\n");
  printf("Orisma 2: Plh8os ergasiwn (n)\n");
  printf("Orisma 3: Algori8mos dromologhshs ergasiwn\n\t[1,5] : Proteraiothta sta megalutera varh [coalition-free,coalitions]\n\t[2,6] : Proteraiothta sta mikrotera varh [coalition-free,coalitions]\n\t[3,7] : FIFO [coalition-free,coalitions] \n\t[4,8] : Tyxaia kai isopi8anh epilogh ergasiwn [coalition-free,coalitions]\n");
  printf("Orisma 4: Politikh xrewshs ergasiwn :\n\t1 : FIFO \n\t2 : Makespan\n\t3 : SortestJobFirst\n\t4 : LongestJobFirst\n");
  printf("Orisma 5: Arxikopoihsh ergasiwn :\n\tr : Tyxaia katanomhmenes ergasies \n\ti : Oles oi ergasies se mia arxikh-tyxaia mhxanh\n\tm : Orismos apo ton xrhsth\n");
  printf("Orisma 6: Eidos mhxanwn :\n\t[i,r,u] : [identical,related,unrelated]\n");
  printf("Orisma 7: Arxikopoihsh varwn :\n\tm : manual\n\tf : apo arxeio\n\t[a,b,c,d,e,w] : kathgories barwn gia peiramta\n");
  printf("Orisma 8: Proteraiothta Coalition :\n\t[i,a] : [min,max]\n");
  exit(1);
}

/*--------------------------- ARXIKOPOIHSEIS --------------------------*/
  
  /* Arxikopoihsh fytrou gia tyxaious ari8mous */
  srand (time(0)); 
  
  /* Arxikopoihsh varwn twn ergasiwn */
  if (weight_ini == 'f'){
    W = file_init(W, s, ms, n, m, policy, "a.txt");
  }else if (weight_ini == 'm'){
    weight_init(W, n, m, machine_type, weight_ini);
  }else { //peiramata
     switch(weight_ini){ //epilogh kathgorias varwn
     case 'a':
       weight_a(W, n, m);break;
     case 'b':
       weight_b(W, n, m);break;
     case 'c':
       weight_c(W, n, m);break;
     case 'd':
       weight_d(W, n, m);break;
     case 'e':
       weight_e(W, n, m);break;
     case 'w':
       weight_min_worst(W, n, m);break;
     case 'l':
       weight_sjf_worst(W, n, m);break;
     default:
       printf("\nDwste swsth timh gia arxikopoihsh varwn\n");
       return -1;
     }
  }
  
  /* Arxikopoihsh ergasiwn & mhxanwn */
  init(s, ms, W, n, m, policy, system_ini); 
  
  if (SHOW_INFO == 1){
    /* Ektypwsh arxikhs katastashs */
    print_state(s, ms, n, m);
  }
  
  /*-----------------EPILOGH ALGORI8MOU KENTRIKOU ELEKTH ----------------*/
  switch(algorithm){
  case 1:
    steps = min_max_weight_job(s, ms, W, n, m, policy, 'a');
    //printf("\n8ewrhtiko anw orio bhmatwn: %d",n);
    break;
  case 2:
    steps = min_max_weight_job(s, ms, W, n, m, policy, 'i');
    break;
  case 3:
    steps = fifo(s, ms, W, n, m, policy);
    //printf("\n8ewrhtiko anw orio bhmatwn: %d",n*(n+1)/2);
    break;
  case 4:
    steps = Random(s, ms, W, n, m, policy);
    //printf("\nYparxei configuration me anw orio bhmatwn: %d",n*(n+1)/2);
    break;
  /****************** Coalitions ********************************************/
  case 5:
    do{
      steps += min_max_weight_job(s, ms, W, n, m, policy, 'a');
      if (coalition_want2migrate(s, ms, W, n, m, coalition_type) != 0){
        steps++;
	num_of_flips++;
	coalition = 1;
      }else{
        coalition = 0;
      }
    }while(coalition);
    //printf("\n8ewrhtiko anw orio bhmatwn: %d",n);
    break;
  case 6:
    do{
      steps += min_max_weight_job(s, ms, W, n, m, policy, 'i');
      if (coalition_want2migrate(s, ms, W, n, m, coalition_type) != 0){
        steps++;
	num_of_flips++;
	coalition = 1;
      }else{
        coalition = 0;
      }
    }while(coalition);
    //printf("\n8ewrhtiko anw orio bhmatwn: %d",n);
    break;
  case 7:
    do{
      steps += fifo(s, ms, W, n, m, policy);
      if (coalition_want2migrate(s, ms, W, n, m, coalition_type) != 0){
        steps++;
	num_of_flips++;
	coalition = 1;
      }else{
        coalition = 0;
      }
    }while(coalition);
    //printf("\n8ewrhtiko anw orio bhmatwn: %d",n);
    break;
  case 8:
    do{
      steps += Random(s, ms, W, n, m, policy);
      if (coalition_want2migrate(s, ms, W, n, m, coalition_type) != 0){
        steps++;
	num_of_flips++;
	coalition = 1;
      }else{
        coalition = 0;
      }
    }while(coalition);
    //printf("\n8ewrhtiko anw orio bhmatwn: %d",n);
    break;
  default:
    printf("\nDwste ena swsto algori8mo (3o orisma) \n");
    break;
  }
  
  if (SHOW_INFO == 1){
    /* Ektypwsh telikhs katastashs */
    print_state(s, ms, n, m);
    printf("\nFtasame se mia NE se %d bhmata, 2^n=%g (2-flips=%d)\n",steps, pow(2,n-1)-n, num_of_flips);
  }else {
    printf("%d ",steps);
    //printf("%d %d ",steps,num_of_flips);
  }
  return 1;
}







/**********************************************************************/
/*        LEITOURGIES APLA SYNDEDEMENHS LISTAS [METAKINHSHS]          */
/**********************************************************************/
/* H syndedemenh lista pou xrhsimopoioume einai mia apla sunde-*
 * demenh lista metakinhshs pou periexei tis pros metakinhsh erga-*
 * sies */

/*------------------ DHMIOURGIA APLHS LISTAS -------------------- */
/* Dhmiourgei mia kenh aplh lista list */

void create(struct machine_state *list){
  list->load = 0;
  list->machine_head.machine_next = NULL;
  list->machine_head.machine_prev = NULL;
  list->machine = &(list->machine_head);
  list->machine_tail = &(list->machine_head);
}

/*------------------------ TAXINOMHSH --------------------------- */
/* Taxinomhsh syndedemenhs listas me vash ton algori8mo mergesort */

struct state *listsort(struct state *list, double **W, char comp) {
  struct state *p, *q, *e, *tail;
  int insize, nmerges, psize, qsize, i;
    
  if (!list)
    return NULL;
  insize = 1;
  while (1) {
    p = list;
    list = NULL;
    tail = NULL;
    nmerges = 0;  /* ari8mos merges se ena perasma */
    while (p) {
      nmerges++;  
      q = p;
      psize = 0;
      for (i = 0; i < insize; i++) {
        psize++;
	q = q->move_next;
        if (!q) break;
      }
      /* an to q den eftase sto telos exoume 2 listes na ennosoume */
      qsize = insize;
      /* ennonoume tis 2 listes */
      while (psize > 0 || (qsize > 0 && q)) {
        /* apofasizei an to epomeno stoixeio 8a er8ei apo to q h to p */
        if (psize == 0) {
	  /* to p einai adeio ara to e erxetai apo to q. */
	  e = q; q = q->move_next; qsize--;
	} else if (qsize == 0 || !q) {
	  /* to q einai adeio ara to e erxetai apo to p. */
	  e = p; p = p->move_next; psize--;
	} else if ( ((comp == 'i') && (W[p->id][p->host] - W[q->id][q->host] <= 0))
	          ||((comp == 'a') && (W[p->id][p->host] - W[q->id][q->host] >= 0)) ){
	  /* To prwto stoixeio tou p einai mikrotero (megalutero)
	   * ara to e erxetai apo to p. */
	  e = p; p = p->move_next; psize--;
	} else {
	  /* To prwto stoixeio tou q einai mikrotero (megalutero)
	   * ara to e erxetai apo to q. */
	  e = q; q = q->move_next; qsize--;
	}
        /* pros8etouem to epomeno stoixeio sthn enopoihmenh lista */
	if (tail) {
	  tail->move_next = e;
	} else {
	  list = e;
	}
	tail = e;
      }
      p = q;
    }
    tail->move_next = NULL;

    if (nmerges <= 1)   
       return list;
    insize *= 2;
  }
}

/*-------------------------- METAKINHSH ----------------------------- */
/* Diatrexoume th lista list apo thn arxh kai epilegoume thn prwth ergasia  
 * pou 8elei na metakinh8ei */

/*----------------- TAXINOMHMENH EISAGWGH ERGASIAS ------------------ */
/* Eisagoume thn ergasia job sthn katallhlh 8esh me vash to baros ths.
 * Gia na ginei ayto diatrexoume thn lista list apo thn koryfh. */

struct state *sorted_insert(struct state *list, struct state *job, double **W, char comp){
  struct state *p, *prev;
  
  p = list;
  prev = list;
  if ( ((comp == 'i') && (W[p->id][p->host] > W[job->id][job->host]))
     ||((comp == 'a') && (W[p->id][p->host] < W[job->id][job->host])) ){
    job->move_next = list;
    list = job;
  } else {
    while ( ((comp == 'i')&&(p->move_next != NULL)&&(W[p->id][p->host] < W[job->id][job->host]))
          ||((comp == 'a')&&(p->move_next != NULL)&&(W[p->id][p->host] > W[job->id][job->host])) ) {
      prev = p;
      p = p->move_next;
    }
    if ( ((comp == 'i')&&(p->move_next == NULL)&&(W[p->id][p->host] < W[job->id][job->host]))
       ||((comp == 'a')&&(p->move_next == NULL)&&(W[p->id][p->host] > W[job->id][job->host])) ) { 
                                  //ftasame sto telos ths listas
      prev = p;
    }
    //printf("teliko:%d %d %d\n",p->id, job->move_next->id, p->move_next->id);
    job->move_next = prev->move_next;
    prev->move_next = job;
    //printf("teliko:%d %d %d",job->id, job->move_next->id, p->move_next->id);
  }
  return list;
}

/*----------------------- DIAGRAFH ERGASIAS -----------------------*/
/* Diagrafei thn epomenh apo thn prev ergasia apo th lista list    */

struct state *delete(struct state *tail, struct state *prev){
  if (prev->move_next == tail){
    tail = prev;
    prev->move_next = NULL;
  } else {
    prev->move_next = prev->move_next->move_next;
  }
  return tail;
}

/*---------------------- EISAGWGH ERGASIAS ----------------------- */
/* Eisagoume thn ergasia job sto telos (tail) ths listas.*/

struct state *move_insert(struct state *tail, struct state *job){
  tail->move_next = job;
  job->move_next = NULL;
  tail = job;
  return tail;
}

/*--------------------- EPISTROFH PRWTHS ERGASIAS -----------------*/
/* Epistrefei thn ergasia pou vrisketai sthn koryfh ths listas */

struct state return_first(struct state *list){
  return *list;
}



/*******************************************************************/
/*    LEITOURGIES DIPLA SYNDEDEMENWN LISTWN [ANAMONHS MHXANWN]     */
/*******************************************************************/
/* Oi listes pou xrhsimopoioume edw einai m dipla syndedemenes * 
 * listes anamonhs mia gia ka8e mia mhxanh pou periexoun tis   *
 * ergasies pou briskontai sthn ekastote mhxanh .*/

/*------------------ DHMIOURGIA DIPLHS LISTAS -------------------- */
/* Gia na dhmiourghsoume mia kenh diplh lista list xrhsimopoioume   *
 * th synarthsh create() */

/*---------------------- EISAGWGH ERGASIAS ----------------------- */
/* Eisagoume thn ergasia job sth lista analoga thn timh tou comp.*/

struct state *machine_insert(struct state *tail, struct state *job, double **W, int policy){
  struct state *p;
  
  p = tail;
  // psaxnei th lista apo to telos sthn arxh gia SJF, LJF alliws den kanei tipota 
  while ( ((policy == 3)&&(p->machine_prev != NULL)
                        &&( (W[p->id][p->host] > W[job->id][job->host]) 
			  ||((W[p->id][p->host] == W[job->id][job->host])&&(p->id > job->id))))
        ||((policy == 4)&&(p->machine_prev != NULL)
	                &&( (W[p->id][p->host] < W[job->id][job->host]) 
			  ||((W[p->id][p->host] == W[job->id][job->host])&&(p->id > job->id))))) {
    p = p->machine_prev;
  }
  // en8esh sto telos ths listas 
  // periptwseis FIFO, MAKESPAN, kai eisagwgh sto telos gia SJF, LJF
  if (p->machine_next == NULL){  
    tail->machine_next = job;
    job->machine_prev = tail;
    job->machine_next = NULL;
    tail = job;
  }else{  //en8esh kapou endiamesa (SJF, LJF)
    p->machine_next->machine_prev = job;
    job->machine_next = p->machine_next;
    job->machine_prev = p;
    p->machine_next = job;
  }
  return tail;
}

/*----------------- DIAGRAFH OPOIASDHPOTE ERGASIAS ----------------*/
/* Diagrafei thn ergasia job apo th lista list */    

struct state *delete_any(struct state *tail, struct state *job){
  if (job == tail){
    tail = job->machine_prev;
    job->machine_prev->machine_next = NULL;
  } else {
    job->machine_prev->machine_next = job->machine_next;
    job->machine_next->machine_prev = job->machine_prev;
  }
  return tail;
}







/***********************************************************************/
/*                         BASIKES SYNARTHSEIS                         */
/***********************************************************************/


/* --------------------- ARXIKOPOIHSH VARWN -----------------------*/
/* Arxikopoihsh varwn twn ergasiwn analoga me ton typo ths mhxanhs *
 * i : identical, r : related, u : unrelated */  

void weight_init(double **W, int n, int m, int machine_type, char weight_ini){
  
  int i, j, w;
  double s;
  
  switch (machine_type){
  case 'u': /* Unrelated Machines */
    for (i = 0;i < n;i++)
      for (j = 0;j < m;j++){
        if (weight_ini == 'm'){ //manual
          printf("\nVaros ergasias %d sthn %d W[%d,%d] = ",i,j,i,j);
	  scanf("%d",&W[i][j]);
        }else{ //random  
          W[i][j] = (rand()%MAX_WEIGHT) + 1;
	}
      }
    break;
  case 'r': /* Related Machines */
    /* Dhmiourgoume ta n varh twn ergasiwn */
    for (i = 0;i < n;i++){
      if (weight_ini == 'm'){ //manual
        printf("\nVaros ergasias %d = ",i);
  	scanf("%f",&W[i][m-1]);
      }else{ //random  
        W[i][m-1] = (rand()%MAX_WEIGHT) + 1;
      }
      printf("%.0f\t",W[i][m-1]);
    }
    printf("\n");
    for (j = 0;j < m;j++){
      /* Dhmiourgoume tis m taxythtes twn mhxanwn */
      if (weight_ini == 'm'){ //manual
	printf("\nTaxythta mhxanhs %d = ",j);
	scanf("%f",&s);
      }else{ //random  
	s = ((rand()%100) + 1) * 0.01; //s in (0,1]
      }
      for (i = 0;i < n;i++)
        W[i][j] = W[i][m-1] * s;
    }  
    printf("\n");
    break;
  case 'i': /* Identical Machines */
    for (i = 0;i < n;i++){
      if (weight_ini == 'm'){ //manual
	printf("\nVaros ergasias %d = ",i);
	scanf("%d",&w);
      }else{ //random  
	w = (rand()%MAX_WEIGHT) + 1;
      }
      for (j = 0;j < m;j++)
        W[i][j] = (double)w;
    }
    break;
  default:
    printf("\nDwse katallhlo typo mhxanmwn (u, r, i).\n");
    break;
  }
  if (SHOW_INFO == 1){
    printW(W, n, m);
  }
}

/* ------------- ARXIKOPOIHSH ERGASIWN & MHXANWN ------------------*/
/* Mhdenizei kai arxikopoiei (vazei NULL) stis times twn melwn twn *
 * domwn kai topo8etei tis ergasies stis mhxanes */
 
void init(struct state *s, struct machine_state *ms, double **W, int n, int m, int policy,
          char system_ini){
  int i, j;
  double cost;
  struct state *p;
  
  /* Arxikopoihsh twn domwn twn mhxanwn */
  for (i=0;i <= m;i++){
    create(&ms[i]);
  }
  
  /* Arxikopoihsh twn domwn twn ergasiwn */
  for (i=0;i < n;i++){
    s[i].id = i;
    s[i].cost = 0;
    s[i].host = 0;
    s[i].machine_next = NULL;
    s[i].machine_prev = NULL;
    /* Eisagwgh ergasias sth lista metakinhshs */
    ms[m].machine_tail = move_insert(ms[m].machine_tail, &s[i]);
  }
  
  /* Topo8ethsh twn ergasiwn stis mhxanes */
  if (system_ini == 'i') /* tyxaia epilogh arxikou monadikou host */
    j = rand()%m;
  for (i = 0;i < n;i++){
    if (system_ini == 'r'){ /* tyxaia epilogh host */
      j = rand()%m;
    }else if (system_ini == 'm'){ /* manual epilogh host */
      printf("Topothethsh ergasias %d sthn mhxanh[0,%d]:",i,m-1);
      scanf("%d",&j);
    }
    
    /* Ayxhsh fortiou mhxanhs kai pros8hkh ths ergasias sthn oura ths */
    ms[j].load += W[i][j];
    ms[j].machine_tail = machine_insert(ms[j].machine_tail, &s[i], W, policy);    
    /* Enhmerwsh host & cost ths ergasias */
    s[i].host = j;
    if (policy == 1){ //FIFO
      s[i].cost = ms[j].load;
    } 
  }
  if (policy == 2){ //a8roisma varwn
    for (i = 0;i < n;i++)
      s[i].cost = ms[s[i].host].load;
  } else if ((policy == 3) || (policy == 4)){ //SJF & LJF
    for (i=0;i < n;i++){
      p = &s[i];
      cost = 0;
      while (p->machine_prev != NULL){ //mexri na ftasoume sto machine_head
        cost += W[p->id][p->host];
	p = p->machine_prev;
      }
      s[i].cost = cost;
    }
  }
}


/* ----------------- ARXIKOPOIHSH APO ARXEIO ------------------------------*/
/* Arxikopoiei to systhma me vash ta stoixeia pou yparxoun sto arxeio file */
double **file_init(double **W, struct state *s, struct machine_state *ms, int n, int m, int policy, char *file){
  
  char *line, *w;
  int i=0, j, k;
  FILE *fp;
  
  fp = fopen(file ,"r");
  k = m * MAX_WEIGHT_LEN;
  line = (char *) calloc(MAX_WEIGHT_LEN, sizeof(char));
  printf("\tEISODOS APO ARXEIO:\nW:---------- \n\t");
  while(( fgets(line, k, fp) != NULL ) && (strlen(line) > 2)){
    j= 0;
    for( w = strtok(line," ") ; w != NULL ; w = strtok(NULL, " ") ){
      printf("%s\t",w);
      W[i][j++] = atoi(w);
    }
    i++;
  }
  printf("\nHOST:---------\n");
  while( fgets(line, k, fp) != NULL ){  
    if (strlen(line) > 2){
      for( i=0, w = strtok(line," ") ; w != NULL ; w = strtok(NULL, " "), i++ ){
        printf("%s\t",w);
        s[i].host = atoi(w);
      }
    }
  } 
  printf("\n");
    
  /* Arxikopoihsh twn domwn twn mhxanwn */
  for (i=0;i <= m;i++){
    create(&ms[i]);
  }
  /* Arxikopoihsh twn domwn twn ergasiwn */
  for (i=0;i < n;i++){
    s[i].id = i;
    s[i].cost = 0;
    s[i].machine_next = NULL;
    s[i].machine_prev = NULL;
    /* Eisagwgh ergasias sth lista metakinhshs */
    ms[m].machine_tail = move_insert(ms[m].machine_tail, &s[i]);
  }
  
  /* Topo8ethsh twn ergasiwn stis mhxanes */
  for (i = 0;i < n;i++){
    j = s[i].host; //host ths i
    /* Ayxhsh fortiou mhxanhs kai pros8hkh ths ergasias sthn oura ths */
    ms[j].load += W[i][j];
    ms[j].machine_tail = machine_insert(ms[j].machine_tail, &s[i], W, policy);    
    /* Enhmerwsh host & cost ths ergasias */
    
    if (policy == 1){ //FIFO
      s[i].cost = ms[j].load;
    } 
  }
    
  return W;
}
  
/*------------------- YPOLOGISMOS KOSTOUS ERGASIAS ---------------*/
double cost(struct state *s, double **W, struct machine_state *ms, int job, int machine, int policy){
  double cost = 0;
  struct state *p;
  
  if ((policy == 1) || (policy == 2)){        // FIFO, MAKESPAN
    cost = ms[machine].load;
  }
  else if ((policy == 3) || (policy == 4)){   //SJF, LJF
    /* Ypologizei to cost ths job sth machine */
    p = ms[machine].machine_head.machine_next;
    while ( ((policy == 3)&&(p != NULL)
                          &&( (W[p->id][p->host] < W[s[job].id][s[job].host])
                            ||((W[p->id][p->host] == W[s[job].id][s[job].host])&&(p->id < s[job].id))))
          ||((policy == 4)&&(p != NULL)
	                  &&( (W[p->id][p->host] > W[s[job].id][s[job].host])
	                    ||((W[p->id][p->host] == W[s[job].id][s[job].host])&&(p->id < s[job].id))))){
      cost += W[p->id][p->host];
      p = p->machine_next;
    } 
  }
  return cost;
}

/*--------------------- ELEGXOS METAKINHSHS -----------------------*/
/* Elegxei gia to an h ergasia job, thelei na metakinh8ei.
 * An thelei epistrefei th mhxanh pou thelei na metakinh8ei
 * An den thelei epistrefei -1 
 * DEN ALLAZEI TIPOTA STO SYSTHMA */

int want2migrate(int job, struct state *s, struct machine_state *ms, double **W, int n, int m, int policy){
  int i, best_response;
  double best_cost, exp_cost;
  
  best_cost = s[job].cost;
  best_response = s[job].host;
  /* Afairoume prosorina ta varos ths job apo to systhma */
  
  ms[s[job].host].load -= W[s[job].id][s[job].host];
  for (i = 0 ; i < m ; i++){
    exp_cost = cost(s, W, ms, job, i, policy);
    //printf ("Kostos %d sthn %d: %f\n",job,i,exp_cost);
    if (exp_cost + W[s[job].id][i] < best_cost){
      best_cost = exp_cost + W[s[job].id][i];
      best_response = i;
    }
  }
  /* Epanatopo8etoume to fortio ths job sto systhma */
  ms[s[job].host].load += W[s[job].id][s[job].host];
  if (best_response == s[job].host)
    return -1;
  return best_response;
}

/*--------------- ELEGXOS COALITION METAKINHSHS -------------------*/
/* Elegxei gia ola ta pi8ana coaltion 2 ergasiwn an exoun ofelos   *
 * na ektelesoun ena 2-flip. An exoun ofelos h synasthsh epistrefei*
 * 1 (kai ektelei to ptwro 2-flip pou 8a vrei                      *
 * h' ayto me th min h' max diafora anamesa sta varh) alliws       *
 * epistrefei 0 kai den allazei tipota sto systhma. */

int coalition_want2migrate(struct state *s, struct machine_state *ms, double **W, int n, int m, char min_max){
  /* mflip1, mflip2: apo8hkeuoun ta id twn ergasiwn pou exoun ofelos apo ena 2-flip metaxy tous*/
  /* min_max_coal: h max h' min (analoga em thn timh tou min_max) diafora twn barwn twn mflip1, mflip2 */
  int i, j, temp_host, mflip1= 0, mflip2= 0, min_max_coal= RAND_MAX;
  
  min_max_coal= (min_max == 'i') ? RAND_MAX : 0;
  for (i = 0 ; i < n-1 ; i++)
    for (j = i+1 ; j < n ; j++)
      if ((s[i].host != s[j].host) &&
          (max(s[i].cost , s[j].cost) > max(ms[s[i].host].load - W[s[i].id][s[i].host] + W[s[j].id][s[j].host] ,
	                                    ms[s[j].host].load - W[s[j].id][s[j].host] + W[s[i].id][s[i].host]))
	   && ( ((min_max == 'i') && (fabs(W[s[i].id][s[i].host] - W[s[j].id][s[j].host]) < min_max_coal )) ||
	        ((min_max == 'a') && (fabs(W[s[i].id][s[i].host] - W[s[j].id][s[j].host]) > min_max_coal )) ) ) {
	
	min_max_coal = fabs(W[s[i].id][s[i].host] - W[s[j].id][s[j].host]);
	mflip1 = s[i].id;
	mflip2 = s[j].id;
      }
  /* Ekteloume to 2-flip */
  temp_host = s[mflip1].host;
  move(mflip1, s[mflip1].host, s[mflip2].host, s, ms, W, n, m, 2);
  move(mflip2, s[mflip2].host, temp_host, s, ms, W, n, m, 2);
  if (SHOW_INFO == 1) { 
    if (mflip1 == mflip2){
      printf("\nno coalition\n");
      return 0;
    }else{
      printf("\n2-flip:%f [%d] - %f [%d]\n",W[mflip1][s[mflip1].host], mflip1, W[mflip2][s[mflip2].host], mflip2);
      print_state(s, ms, n, m);
      return 1;
    }
  }
  if (mflip1 == mflip2)
    return 0;
  else{
    //printf("%d %d ",mflip1, mflip2);
    return 1;
  }
}


/*------------------------- METAKINHSH ----------------------------*/
/* Synarthsh pou kanei tis aparaithtes allages stis domes kata th *
 * metakinhsh mias ergasias apo mia mhxanh se mia allh            */

void move(int job, int m1, int m2, struct state *s, struct machine_state *ms, double **W, int n, int m, int policy){
  struct state *p;
  int w; //to baros ths ergasias pou metakineitai prin metakinh8ei
  double cost = 0;
  
  /* Meiwsh tou fortiou ths m1 */
  ms[m1].load -= W[s[job].id][s[job].host];
  /* Update cost twn ergasiwn ths m1 */
  if ((policy == 1) || (policy == 3) || (policy == 4)){ //FIFO, SJF, LJF
    /* Update cost epomenwn apo thn job ergasiwn sthn m1*/
    p = &s[job];
    w = W[s[job].id][s[job].host];
    while (p != NULL){
      p->cost -= w;
      p = p->machine_next;
    }
  }else if (policy == 2){           //A8roisma varwn  if (policy == 2)
    /* Update cost olwn twn ergasiwn sthn m1*/
    p = ms[m1].machine->machine_next;
    while (p != NULL){
      p->cost = ms[m1].load;
      p = p->machine_next;
    }
  }
  /* Diagrafh ths job apo thn m1 */
  ms[m1].machine_tail = delete_any(ms[m1].machine_tail, &s[job]);
  
  /* Ayxhsh tou fortiou ths m2 kai allagh tou host ths job */
  s[job].host = m2;
  ms[m2].load += W[s[job].id][s[job].host];
  /* Eisagwgh ths job sthn m2 */
  ms[m2].machine_tail = machine_insert(ms[m2].machine_tail, &s[job], W, policy); 
  /* Update cost twn ergasiwn ths m2 [kai ths job]*/
  if (policy == 1){                          //FIFO
    /* Update cost ths job sth m2*/
    s[job].cost = ms[m2].load;
  }else if (policy == 2){                  //A8roisma varwn
    /* Update cost olwn twn ergasiwn sthn m2*/  
    p = ms[m2].machine->machine_next;
    while (p != NULL){
      p->cost = ms[m2].load;
      p = p->machine_next;
    }
  }else if ((policy == 3) || (policy == 4)){   //SJF, LJF
    /* Update cost ths job sth m2*/
    p = &s[job];
    while (p->machine_prev != NULL){ //mexri na ftasoume sto machine_head
      cost += W[p->id][p->host];
      p = p->machine_prev;
    }
    s[job].cost = cost;
    /* Update cost twn ergasiwn pou vriskontai panw apo thn job sthn m2*/
    p = &s[job];
    p = p->machine_next;
    while (p != NULL){
      p->cost += W[s[job].id][s[job].host];
      p = p->machine_next;
    }
  }
}



/***********************************************************************/
/*                       BOH8HTIKES SYNARTHSEIS                        */
/***********************************************************************/

/* Typwnei thn parousa katastash sto systhma state kai machine_state */
void print_state(struct state *s, struct machine_state *ms, int n, int m){
  int i;      /* metrhths */
  struct state *p;
 
  printf("\n---------------------------\ncost:  ");
  for (i=0;i < n;i++)  
    printf("%.0f ",s[i].cost);
  printf("\n---------------------------\nhost:  ");
  for (i=0;i < n;i++)
    printf("%d ",s[i].host);
  printf("\n---------------------------\nload:  ");
  for (i=0;i < m;i++)
    printf("%.0f ",ms[i].load);
  
  for (i=0;i < m;i++){
    printf("\n-------------------------\nMhxanh: %d :",i);
    p = ms[i].machine->machine_next;
    while (p != NULL){
      printf(" %d",p->id);
      p = p->machine_next;
    }
  }
  printf("\n");
}

void print_move_list(struct machine_state *ms, int m){
  struct state *p;  
  
  printf("\n-------------------------\nLISTA METAKINHSHS :");
  p = ms[m].machine_head.move_next;
  while (p != NULL){
    printf(" %d",p->id);
    p = p->move_next;
  }
  printf("\n");
}
 

void printW(double **W, int n, int m){
  /* Ektypwsh W */  
  int i, j;
  
  printf("W: -------------------\n   ");
  for (i=0;i < m;i++)
    printf("%d\t",i);
  printf("\n   --------------------------------\n");
  for (i=0;i < n;i++){
    printf("%d: ",i);
    for (j=0;j < m;j++){
      printf("%.0f\t",W[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}




  

/************************************************************************/
/*                   ALGORI8MOI KENTRIKOU ELEGKTH                       */
/************************************************************************/

/*---------- 1. MEGALYTEROU VAROUS & 2. MIKROTEROU VAROUS --------------*/
/* Epilegoume thn ergasia me to megalytero (mokrotero) varos apo aytes  *
 * pou 8eloun na metakinh8oun kai ths epitrepoume na metakinh8ei.       *
 * Diathroume mia taxinomhmenh ws pros ta varh lista, th diatrexoume apo*
 * thn arxh kai metakinoume thn prwth ergasia pou 8elei na metakinh8ei. *
 * An ayth exei allazei varos thn epanatopo8etoume sthn lista me vash to*
 * varos ths kai xekiname apo thn arxh th diatrexh ths listas. O        *
 * algori8mos termatizei otan, afou diatrexoume olh th lista kamia      *
 * ergasia den thelei na metakinh8ei.*/
 
int min_max_weight_job (struct state *s, struct machine_state *ms, double **W, int n, int m, int policy, char comp){
  struct state *p, *prev;
  int steps = 0, next_machine, old_w;
  
  ms[m].machine_head.move_next = listsort(ms[m].machine_head.move_next, W, comp);
  p = ms[m].machine_head.move_next;
  prev = ms[m].machine;
  while (p != NULL){
    next_machine = want2migrate(p->id, s, ms, W, n, m, policy);
    if (next_machine != -1){ /* An thelei na metakinh8ei */
      old_w = W[p->id][p->host];
      if (SHOW_INFO == 1)  
	printf("\nSTEP : %d, metakinh8hke h %d apo thn %d sthn %d.",steps+1,p->id,p->host,next_machine);
      move(p->id, p->host, next_machine, s, ms, W, n, m, policy);
      /* An allaxei mege8os xanampainei sthn lista */
      if (old_w != W[p->id][p->host]){  
        delete(NULL, prev);
        ms[m].machine_head.move_next = sorted_insert(ms[m].machine_head.move_next, p, W, comp);
      }
      /* O p tha deixnei sthn arxh ths listas */
      p = ms[m].machine_head.move_next;
      prev = ms[m].machine;
      steps++;
      if (SHOW_INFO == 1){
        print_state(s, ms, n, m);
        print_move_list(ms, m);
      }
    } else {
      prev = p;
      p = p->move_next;
    }
  }
  return steps;
}

/*---------------------------- 3. FIFO ------------------------------------*
 * Elegxoume gia tis n ergasies poies 8eloun na metakinh8oun.              *
 * Oses theloun diathroun th 8esh tous sth lista enw oi alles mpainoun sto *
 * telos ths listas. Meta epitrepoume na metakinh8ei h prwth ergasia kai   *
 * xanaelegxoume apo thn arxh th lista. H prwth ergasia pou exei metakinh- *
 * 8ei apo to prohgoumeno bhma den tha thelei na metakinh8ei xana opote 8a *
 * diagrafei kai 8a eisax8ei sto telos ths listas. O algori8mos termatizei *
 * otan kamia ergasia den thelei na metakinh8ei. */
  
int fifo(struct state *s, struct machine_state *ms, double **W, int n, int m, int policy){
  struct state *p, *prev, first_job;
  int i, steps = 0, next_machine;
  
  do{  
    /* Elegxos metakinhshs n ergasiwn */   
    p = ms[m].machine_head.move_next;
    prev = ms[m].machine;
    for(i = 0;i < n;i++){
      next_machine = want2migrate(p->id, s, ms, W, n, m, policy);
      if (next_machine != -1){ /* An thelei na metakinh8ei */
        if (SHOW_INFO == 1)  
	  printf("\nH %d thelei na metakinh8ei kai menei sth 8esh ths!\n",p->id);
	prev = p;
        p = p->move_next;
      } else {                 /* An DEN thelei na metakinh8ei */
        if (SHOW_INFO == 1)  
	  printf("\nH %d DEN thelei na metakinh8ei kai paei sto telos ths listas!\n",p->id);
        ms[m].machine_tail = delete(ms[m].machine_tail, prev);
        ms[m].machine_tail = move_insert(ms[m].machine_tail, p);
        p = prev->move_next;   
      }
    }
    /* Elegxoume an thelei na metakinh8ei h prwth ergasia */
    /* An den thelei tote kamia den thelei! */
    first_job = return_first(ms[m].machine_head.move_next);
    next_machine = want2migrate(first_job.id, s, ms, W, n, m, policy);
    if (next_machine != -1){ /*  */
      if (SHOW_INFO == 1)
        printf("\nSTEP : %d, metakinh8hke h %d apo thn %d sthn %d.",
             steps+1,first_job.id,first_job.host,next_machine);
      move(first_job.id, first_job.host, next_machine, s, ms, W, n, m, policy);
      steps++; 
      if (SHOW_INFO == 1){  
	print_state(s, ms, n, m);
        print_move_list(ms, m);
      }   
    }
  } while (next_machine != -1);
  
  return steps;
}

/*---------------------------- 4. RANDOM -------------------------------------*/
/* Epilegoume tuxaia mia ergasia apo aytes pou 8eloun na metakinh8oun kai ths *
 * epitrepoume na metakinh8ei. Sundeoume tis j-osth ergasia pou thelei na     *
 * metakinh8ei me thn j 8esh tou pinaka s xrhsimopoiontas ton deikth move_next*
 * Epilegoume tyxaia mia apo tis prwtes j 8eseis tou s pou antistoixoun stis  *
 * ergasies pou 8eloun na metakinh8oun kai epitrepoume na metakinh8ei h ergasia
 * pou einai syndedemenh me th 8esh ayth. An kamia den thelei na metakinh8ei o*
 * algori8mos termtizei. */ 

int Random(struct state *s, struct machine_state *ms, double **W, int n, int m, int policy){
  int i, j, chosen, steps= 0, next_machine;
  
  do{
    for (i = 0, j = 0;i < n;i++){
      next_machine = want2migrate(s[i].id, s, ms, W, n, m, policy);
      if (next_machine != -1){ /* An thelei na metakinh8ei */
        s[j++].move_next = &s[i];
	if (SHOW_INFO == 1){   
	  printf("%d ",s[i].id);
	}
      }
    }
    if (j != 0){
      chosen = rand()%j;
      if (SHOW_INFO == 1)  
	printf("Chosen %d from 0 - %d",chosen,j-1);
      next_machine = want2migrate(s[chosen].move_next->id, s, ms, W, n, m, policy);
      move(s[chosen].move_next->id, s[chosen].move_next->host, next_machine, 
           s, ms, W, n, m, policy);
      steps++;
      if (SHOW_INFO == 1)  
	printf("\n%d moved\n",s[chosen].move_next->id); 
    }
    if (SHOW_INFO == 1){  
      print_state(s, ms, n, m);
    }
  }while (j != 0);
  
  return steps;    
}



/***********************************************************************/
/*                             PEIRAMATA                               */
/***********************************************************************/


//10% ek8etika - 90% monadiaia
void weight_a(double **W, int n, int m){
  int i, j;
  
  for(i= 0 ;i < n; i++){
    for(j= 0 ;j < m; j++){
      W[i][j] = pow(10,n/10);
    }
  }
  for(i= (n/10); i < n; i++){
    for(j=0;j < m;j++){
      W[i][j] = 1;
    }
  }
  if (SHOW_INFO == 1)
    printW(W, n, m);
}

//50% ek8etika - 50% monadiaia
void weight_b(double **W, int n, int m){
  int i, j;
  
  for(i= 0 ;i < n/2; i++){
    for(j=0;j < m;j++){
      W[i][j] = pow(10,n/10);
    }
  }
  
  for(i= (n/2); i < n; i++){
    for(j=0;j < m;j++){
      W[i][j] = 1;
    }
  }
  if (SHOW_INFO == 1)  
    printW(W, n, m);
}

//90% ek8etika - 10% monadiaia
void weight_c(double **W, int n, int m){
  int i, j;
  
  for(i = 0 ; i < n/10 ; i++){
    for(j = 0 ; j < m ; j++){
      W[i][j] = 1;
    }
  }
  
  for(i = (n/10) ; i < n ; i++){
    for(j=0;j < m;j++){
      W[i][j] = pow(10,n/10);
    }
  }
  if (SHOW_INFO == 1)  
    printW(W, n, m);
}

//tyxaia [1,10^(n/10)]
void weight_d(double **W, int n, int m){
  int i, j, k;
  double w;
  
  k = pow(10,n/10);
  for(i = 0 ; i < n ; i++){
    w = (rand() % k) + 1;
    for(j = 0 ; j < m ; j++){
      W[i][j] = w;
    }
  }
  
  if (SHOW_INFO == 1)  
    printW(W, n, m);
}

void weight_min_worst(double **W, int n, int m){
  int i, j , k, l;
  double w= 1;
  
  l = n/(m-1);
  for(i = 0 ; i < m-1 ; i++){
    for(k = 0 ; k < l ; k++){
      for(j = 0 ; j < m ; j++){
        W[k + i*l ][j] = w;
      }
    }
    w = w * ( l + 1 );
  }
 
  
  if (SHOW_INFO == 1)  
    printW(W, n, m);
}

void weight_sjf_worst(double **W, int n, int m){
  int i, j ;
  
  for(i = 0 ; i < n ; i++)
    for(j = 0 ; j < m ; j++)
        W[i][j] = i+1;
    
  if (SHOW_INFO == 1)  
    printW(W, n, m);
}

//varh gia coalitions 
void weight_e(double **W, int n, int m){
  int i, j;
  double w;
  
  for(i = 0 ; i < n ; i++){
    if (i < n/4)
      w = 11;
    else if (i < 2*n/4)
      w = 13;
    else if (i < 3*n/4)
      w = 20;
    else
      w = 22;
    for(j = 0 ; j < m ; j++){
      W[i][j] = w;
    }
  }
  
  if (SHOW_INFO == 1)  
    printW(W, n, m);
}


