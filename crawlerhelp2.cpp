#include<stdio.h>
#include<string.h>
#include <ctype.h>
#include<stdlib.h>
#include<conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<time.h>

#define HASH_SIZE 100
#define MAX_DEPTH 3
#define Url_Length 1000

#define seed "https://www.chitkara.edu.in"
#define  SEED_DOMAIN "chitkara.edu.in"

#define MAX_URL_PER_PAGE 100
#define MAX_Anchor_Tag 500
#define INTERVAL_TIME 10      //time in ms

#define CHECK_MALLOC(s) if ((s) == NULL) { \
printf("memory allocation failed in %s: line%d", _FILE, __LINE_); \
perror(":"); \
exit(0); \
}

#define SET_MEM(start,size) memset(start,0,size)
// https://www.chitkara.edu.in 2 C:\Users\HP\Desktop\crawlerHelp

struct url
{
char *str;
int depth;
};
struct node
{
struct url u;
int isVisted;
int key;
node *next;
};
struct hash
{
struct node *head;
struct node *tail;
int count;
};
int generateKey(char *url)
{
int sum=0,i;
for(i=0;url[i]!='\0';i++)
{
sum=sum+url[i];
}
return sum%HASH_SIZE;
}

struct node*create_node(char *s,int d,int iv,int key1)
{
struct node *temp=NULL;
int len=strlen(s);
temp=(struct node *)malloc(sizeof(struct node));
temp->u.str=(char *)malloc(len*sizeof(char));
strcpy(temp->u.str,s);
temp->u.depth=d;
temp->isVisted=iv;
temp->key=key1;
temp->next=NULL;
return temp;
}
void hashTable(int key1,char *url,int d,struct node **head,struct node **tail,struct hash *h)
{
int iv=0;
struct node *t,*t2,*t3,*ptr;
struct node *temp;
if(*head==NULL)    // Different cluster.....First node 
{
iv=1;
temp=create_node(url,d,iv,key1);
*head=temp;
*tail=temp;
h[key1].count=1;
h[key1].head=temp;
h[key1].tail=temp;
//printf("\n\nMy head NULLy %s   %d\n\n",url,key1);
return;
}
if(h[key1].head==NULL)   //Different cluster
{
temp=create_node(url,d,iv,key1);
t2=(*tail)->next;
(*tail)->next=temp;
temp->next=t2;
h[key1].count=1;
h[key1].head=temp;
h[key1].tail=temp;
//printf("\n\nMy h[key1].head==NULL  %s    %d\n\n",url,key1);
return;
}
else                          // Same cluster
{
t=h[key1].head;
t3=t;
while(t->key==key1 && t->next!=NULL)
{
if(strcmp(t->u.str,url)==0)
{
return;
}
ptr=t;
t=t->next;
}
if(t->next==NULL && t->key==key1)
{
if(strcmp(t->u.str,url)==0)
{
return;
}
else
{
temp=create_node(url,d,iv,key1);
t->next=temp;
h[key1].count++;
h[key1].tail=temp;
if(t3==*head)
{
*tail=temp;
}
}
//printf("\n\nAt last in link list  %s  %d\n\n",url,key1);
return;
}
else
{
t2=ptr->next;
temp=create_node(url,d,iv,key1);
ptr->next=temp;
temp->next=t2;
h[key1].count++;
h[key1].tail=temp;
if(t3==*head)
{
*tail=temp;
}
//printf("\n\nCluster middle %s   %d\n\n",url,key1);
}
}
}
void add_node(char **as,struct node **head,struct node **tail,int ls,int d,struct hash *h)
{
int i=0,key1;
while(i<ls)
{
key1=generateKey(as[i]);
hashTable(key1,as[i],d,head,tail,h);
i++;	
}
}
void display(struct node **head)
{
int i=0;
struct node *t;
t=*head;
while(t!=NULL)
{
printf("%d %s %d  %d  %d \n\n",i,t->u.str,t->u.depth,t->isVisted,t->key);
t=t->next;
i++;
}
}
struct node* getUrlFromList(struct node **head,int depth)
{
struct node *temp;
temp=*head;
while(temp!=NULL)
{
if(temp->isVisted==0 && temp->u.depth<depth)
{
temp->isVisted=1;
return temp;
}
temp=temp->next;
}
return temp;
}
int checkUrl(char *url)
{
char ch[Url_Length]="wget --no-check-certificate --spider ";
strcat(ch,url);
if(!system(ch))
{
printf("Valid URL");
}
else
{
printf("Invalid URL");
return 0;
}
}
int checkStart1(char* url)
{
int j=0,i=0,k;
char temp;
while(url[i]!='\0')
{
temp=url[i];
k=i;
j=0;
if(temp==seed[j])
{
while(url[k]==seed[j] && url[k]!='\0' && seed[j]!='\0')
{
k++;
j++;
}
if(seed[j]=='\0')
{
return 1;
}
}
i++;
}
return 0;
} 

int isDepthNum(char *depth)
{
int i=0;
while(depth[i]!='\0')
{
if(depth[i]>=48 && depth[i]<=57)
{

}
else
{
return 0;
}
i++;
}
return 1;
}
int getDepth(char*depth)
{
int n;
char buffer[50];
n=atoi(depth);
return n;
}
int depthRange(int depth)
{
if(MAX_DEPTH>=depth)
return 1;
else
return 0;
}
int checkStore1(char *dir)
{
struct stat statbuf;
if( stat(dir, &statbuf) == -1 )
{
fprintf(stderr, "\nInvalid directory\n");
return 0;
}
///Both check if there's a directory and if it's writable
if ( !S_ISDIR(statbuf.st_mode) )
{
fprintf(stderr, "\nInvalid directory entry. Your input isn't a directory\n");
return 0;
}
if ( (statbuf.st_mode & S_IWUSR) != S_IWUSR )
{
fprintf(stderr, "\nInvalid directory entry. It isn't writable\n");
return 0;;
}
return 1;
}
void getWebPage(char *url,char *dir)
{
char urlbuffer[Url_Length+300]={0};
strcat(urlbuffer, "wget --no-check-certificate -O ");
strcat(urlbuffer,dir);
strcat(urlbuffer,"/temp.txt ");
strcat(urlbuffer, url);
system(urlbuffer);
}
char* makePermaFile(char *dir,int fn)
{
int i=0,file_size;
char *fs,c;
char str[1000];
sprintf(str, "%d", fn);
struct stat st; //variable which will count length of file.
char path1[300]="",path2[300]="";
strcat(path1,dir);
strcat(path1,"/temp.txt ");
strcat(path2,dir);
strcat(path2,"/file ");
strcat(path2,str);
strcat(path2,".txt ");
stat(path1,&st); // temp.txt is the file where wget fetch the html
file_size=st.st_size;
fs=(char*)malloc(file_size*sizeof(char));
FILE *fp1=fopen(path1,"r");
FILE *fp2=fopen(path2,"w");
c=fgetc(fp1);
while(c!=EOF)
{
fs[i]=c;
fputc(c,fp2);
i++;
c=fgetc(fp1);
}
fclose(fp1);
fclose(fp2);
fs[i]='\0';
return fs;
} 
int checkAchor1Base(const char *anchor)
{ 
char* p; 
p=strstr(anchor,SEED_DOMAIN); 
if (p) 
{ 
return 1; 
} 
else
return 0; 
}
int checkAchor3Duplicate(char **as,int t,char *anchor)
{
int i=0;
for(i=0;i<t;i++)
{	
if(strcmp(as[i],anchor)==0)
{
return 0;
}
}
return 1;
}
int checkAchor2Hash(char *anchor)
{
if(anchor[0]=='#')	
return 0;
else
return 1;
}
void fetchAnchor(char *fs,int *ls,char **as,char *murl)
{
int i=0,j=0,k,p=0,flag=1,flagnotout=1;
char defAnchor[14]="";
char anchor[(MAX_Anchor_Tag+1)];
char *anchorf;
char temp;
strcat(defAnchor,"ahref");;
anchorf=(char*)malloc((MAX_Anchor_Tag+1)*sizeof(char));
strcpy(anchorf,murl);
as[(*ls)++]=anchorf;
while(fs[i]!='\0')
{
temp=tolower(fs[i]);
j=0;
k=i;
if(temp==defAnchor[j])
{
while((temp==defAnchor[j] || temp==' ') && defAnchor[j]!='\0')
{
if(temp==defAnchor[j])
{
j++;
}
k++;
temp=tolower(fs[k]);
}
if(defAnchor[j]=='\0')
{
flag=1;
p=0;
while(temp!=34 && temp!=39)
{
k++;
temp=tolower(fs[k]);
}
k++;
temp=tolower(fs[k]);
while(temp!=34 && temp!=39)
{
anchor[p]=temp;
p++;
if(p>(MAX_Anchor_Tag))
{
flag=0;
break;
}
k++;
temp=tolower(fs[k]);
}
anchor[p]='\0';
if(flag==1)
{
if((*ls)<MAX_URL_PER_PAGE)
{
flagnotout=1;
if(checkAchor2Hash(anchor))
{
if(checkAchor1Base(anchor))
{
if(checkAchor3Duplicate(as,*ls,anchor))
{
anchorf=(char*)malloc((p+1)*sizeof(char));
strcpy(anchorf,anchor);
as[(*ls)++]=anchorf;	
}	
}
}
}
else
{
flagnotout=0;
}
}
i=k;
}
else
{
j=0;
}
}
if(flagnotout==0)
{
break;
}
i++;
}
}
int main(int argc,char*argv[])
{
int depth,i,ls=0,d=1,t,c=0,fn=1,flag=0;
char*filestr;
struct node*head=NULL;
struct node*tail=NULL;
struct node*getUrl=NULL;
struct hash h[HASH_SIZE]={ };
char *as[MAX_URL_PER_PAGE];
if(argc==4)
{
if(checkStart1(argv[1]) && checkUrl(argv[1]))
{
printf("Domain match.........\n");
if(isDepthNum(argv[2]))
{
printf("Conngo Depth is Number\n");
depth = getDepth(argv[2]);
printf("Depth = %d\n",depth);
if(depthRange(depth))
{
if(checkStore1(argv[3]))
{
printf("Directory is valid\n");
do
{
flag=0;
getWebPage(argv[1],argv[3]);
filestr=makePermaFile(argv[3],fn);
fn++;
fetchAnchor(filestr,&ls,as,argv[1]);
printf("\n\nGoing to display keep patience\n\n");
add_node(as,&head,&tail,ls,d,h);
display(&head);
getUrl=getUrlFromList(&head,depth);
if(getUrl!=NULL)
{
argv[1]=getUrl->u.str;
d=getUrl->u.depth;
d++;
ls=0;
flag=1;
}
else
{
flag=0;
}
}while(flag);
}
}
}
}
}
else
{
printf("Sorry number of argument do not match with appropriate prototype\n");
} 
}

