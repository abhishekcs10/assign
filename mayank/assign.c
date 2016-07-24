#include<stdio.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>

void merge(char a[][20],int start,int mid,int end)
{
	char arr[1000][20];
	int i,j,k;
	i=start,j=mid+1,k=start;
	while(i<=mid&&j<=end)
	{
		if(strcmp(a[i],a[j])<0)
		{
			strcpy(arr[k],a[i]);
			i++;k++;
		}	
		else if(strcmp(a[i],a[j])>0)
		{
			strcpy(arr[k],a[j]);
			j++;k++;
		}
		else
		{
			strcpy(arr[k],a[i]);
			k++;i++;
			strcpy(arr[k],a[j]);
			k++;j++;		
		}

	}
	if(i>mid)
	{
		while(j<=end)
		{
			strcpy(arr[k],a[j]);
			k++;j++;
		}	
	}
	else
	{
		while(i<=mid)
		{
			strcpy(arr[k],a[i]);
			k++;i++;		
		}
	}
	for(i=start;i<=end;i++)
	strcpy(a[i],arr[i]);
	
}

void mergesort(char a[][20],int m,int start,int end)
{
	int mid;
	if(start<end)
	{
		mid=(start+end)/2;
		mergesort(a,m,start,mid);
		mergesort(a,m,mid+1,end);
		merge(a,start,mid,end);
	}	
}
int unique(char a[][20],int count[],int m)
{
	int k=0,i;
	char arr[1000][20];
	for(i=0;i<m;i++)
	{
		count[k]=1;
		strcpy(arr[k],a[i]);
		
		while(i<m-1&&strcmp(a[i],a[i+1])==0)
		{
			i++;
			count[k]++;
		}
		k++;
		
	}
	for(i=0;i<k;i++)
	strcpy(a[i],arr[i]);
	return (k);
}
int merging(char a[][20],int u[],int start,int mid,int end)
{
	char arr[1000][20],b[1000];
	int i,j,k;
	i=start,j=mid+1,k=start;
	while(i<=mid&&j<=end)
	{
		if(strcmp(a[i],a[j])<0)
		{
			strcpy(arr[k],a[i]);
			b[k]=u[i];
			i++;k++;
		}	
		else if(strcmp(a[i],a[j])>0)
		{
			strcpy(arr[k],a[j]);
			b[k]=u[j];
			j++;k++;
		}
		else
		{
			strcpy(arr[k],a[i]);
			b[k]=u[i]+u[j];
			k++;i++;
			j++;		
		}

	}
	if(i>mid)
	{
		while(j<=end)
		{
			strcpy(arr[k],a[j]);
			b[k]=u[j];
			k++;j++;
		}	
	}
	else
	{
		while(i<=mid)
		{
			strcpy(arr[k],a[i]);
			b[k]=u[i];
			k++;i++;		
		}
	}
	for(i=start;i<k;i++)
	{
		strcpy(a[i],arr[i]);
		u[i]=b[i];
	}
	return k;
	
}
int main()
{
    char out[1000][20],name[20];
    char f1[100],f2[100];
    int k,i,m,n,num;
	int count[1000];
for( i=0;i<1000;i++)
count[i]=0;

i=0;
    //printf("enter the file name");
    //scanf("%s",f1);
    //scanf("%s",f1);
    FILE *fp1,*fp2;
    fp1=fopen("SampleInputFile_1.txt","r");
    fp2=fopen("SampleInputFile_2.txt","r");
    fscanf(fp1,"%d",&m);
    fscanf(fp2,"%d",&n);
    printf("enter the number of chunks");
    scanf("%d",&k);
    pid_t pid1[k],pid2[k];		
    int status;
    for(i=0;i<k;i++)
    {
        num=fork();
        if(num<0)
        {	
            printf("error");
            exit(1);
        }
        else if(num==0)
        {
		int j,l;
		char arr[1000][20],a[1000][20];
		pid1[i]=getpid();
        	FILE *f1;
        	sprintf(name,"file1%d",i);
        	f1=fopen(name,"w+");
        	
		int p=0,z;
		if(m%k==0)
		z=m/k;
		else
		z=m/k+1;
		for(j=0;j<i*z;j++)
		{
			
			
			fscanf(fp1,"%s",a[j]);
		}	
        	for(j=i*(z);j<((i==k-1)?m:(i+1)*(z));j++)
		{
			fscanf(fp1,"%s",arr[p]);
			p++;
		}
		mergesort(arr,p,0,p-1);
		l=unique(arr,count,p);
		fprintf(f1,"%d\n",l);   			     
                for(j=0;j<l;j++)
		{
			fprintf(f1,"%s %d\n",arr[j],count[j]);
		}
		fclose(f1);
         	//do work
         	exit(0);   
        }
        
    }
    fclose(fp1);
//file 2    
    for(i=0;i<k;i++)
    {
        num=fork();
        if(num<0)
        {	
            printf("error");
            exit(1);
        }
        else if(num==0)
        {
		int t,r;
		char brr[1000][20],b[1000][20];
		pid2[i]=getpid();
        	FILE *f2;
        	sprintf(name,"file2%d",i);
        	f2=fopen(name,"w+");
        	
		int q=0,z;
		if(n%k==0)
		z=n/k;
		else
		z=n/k+1;
		for(t=0;t<i*(n/k);t++)
		fscanf(fp2,"%s",b[t]);
        	for(t=i*(n/k);t<((i+1)*(n/k));t++)
		{
			fscanf(fp2,"%s",brr[q]);
			q++;
		}
		mergesort(brr,q,0,q-1);
		r=unique(brr,count,q);
		fprintf(f2,"%d\n",r);   			     
                for(t=0;t<r;t++)
		{
			fprintf(f2,"%s %d\n",brr[t],count[t]);
		}
		fclose(f2);
         	//do work
         	exit(0);   
        }
        
    }	
    
    fclose(fp2);
for(i=0;i<k;i++)
    {
	waitpid(pid1[i],&status,WIFEXITED(&status));
    }
    for(i=0;i<k;i++)
    {
	waitpid(pid2[i],&status,WIFEXITED(&status));
    }





    //merging of file1
FILE *ff1;    
    char arr[1000][20];
    int b[1000],u[1000];
    int op=0;
    for(i=0;i<k;i++)
    {
	int p=0,j,q;
	ff1=NULL;
	memset(name,0,sizeof name);
	sprintf(name,"file1%d",i);
	ff1=fopen(name,"r+");
	if(ff1==NULL)
	printf("Error\n");
ff1=fopen(name,"r+");
	//printf("Before file1%d\n",i);
	rewind(ff1);
	fscanf(ff1,"%d ",&p);
	printf("file1%d %d\n",i,p);

	for(j=0;j<p;j++)
	{
		fscanf(ff1," %s %d ",arr[j],&b[j]); //arr[j] wordsscannedfromfile   b[j] frequency
	//	printf("w %s %d",arr[j],b[j]);
	}
	if(i==0)
	{
		for(j=0;j<p;j++)
		{
			strcpy(out[op],arr[j]);      // out[op]stores result
			u[op]=b[j];				//u stores o/p freq
			op++;
		}
	}
	else
	{
		for(j=0;j<p;j++)
		{
			strcpy(out[op],arr[j]);
			u[op]=b[j];
			op++;
		}
		//printf("%d",p);
		op=merging(out,u,0,op-p-1,op-1);
		printf("modified op %d\n",op);
		
	}
	
    }
    fclose(ff1);
    int add1=0;
    for(i=0;i<op;i++)
    {
	printf("%s(%d)\n",out[i],u[i]);
	add1++;
    }
	printf("\n%d\n\n",add1);
       	
    
}

