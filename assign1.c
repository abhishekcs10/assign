#include<stdio.h>
#include<assert.h>
#include<string.h>
void merge(int file_size,char words[][50],int start,int mid,int end)
{
int left=mid-start+1;
int right=end-mid,i,j;
//printf("came\n");
char word_l[500][50],word_r[500][50];
for(i=0;i<left;i++)
{
strcpy(word_l[i],words[start+i]);
//printf("left %s\n",word_l[i]);
}
for( i=0;i<right;i++)
{
strcpy(word_r[i],words[mid+i+1]);
//printf("right %s\n",word_r[i]);
}

int k=start;
for( i=0,j=0;i<left&&j<right;)
{
int cmp=strcmp(word_l[i],word_r[j]);
	if(cmp<=0)
	{
	strcpy(words[k++],word_l[i]);	
	i++;
	}
	else{
	strcpy(words[k++],word_r[j]);
	j++;
	}

}
//printf("aisa %d %d\n",i,j);
while(i<left)
{
strcpy(words[k++],word_l[i]);
i++;

}
//printf("h\n");
while(j<right)
{
strcpy(words[k++],word_r[j]);
j++;
}

//printf("n");
//for(i=0;i<k;i++)
//printf("%s\n",words[i]);
}


void sort(int file_size,char file[][50],int start,int end)
{
if(start==end)
return;
int mid=(start+end)/2;
sort(file_size,file,start,mid);
sort(file_size,file,mid+1,end);

//printf("merging %d %d %d\n",start,mid,end);
merge(file_size,file,start,mid,end);


}




int find_unique(int num,char words[][50],char unique[][50],int freq[])
{
int i,k=0;
for(i=0;i<num;i++)
freq[i]=1;

for(i=0;i<num-1;i++)
{
if(!i)
strcpy(unique[k],words[i]);

	if(strcmp(words[i],words[i+1])==0)
	{
	freq[k]++;
	}
	else
	{
	k++;
	strcpy(unique[k],words[i+1]);
	}
}
return k;
}

int comp_unique(int no1,char uniq1[][50],int no2,char uniq2[][50])
{
int i=0,j=0,com=0;
for(;i<no1&&j<no2;)
{
if(strcmp(uniq1[i],uniq2[j])<0)
i++;
else if (strcmp(uniq1[i],uniq2[j])>0)
j++;
else
{com++;
i++,j++;
}

}

return com;
}


int main()
{

FILE *openfile1,*openfile2;
char filename1[50],filename2[50];
char take_input[50];
int no_in_file1,no_in_file2,iterate1,iterate2,iter,common;
char wordsin1[500][50],wordsin2[500][50],unique1[500][50],unique2[500][50];
int freq1[500],freq2[500],uniquein1,uniquein2;
scanf("%s ",filename1);
scanf("%s",filename2);
//printf("%so\n%so",filename1,filename2);
//puts(filename2);    
openfile1=fopen(filename1,"r");
openfile2=fopen(filename2,"r");
if(openfile1==NULL||openfile2==NULL)
{
printf("-1\n");
}

//read_input(wordsin1[][],wordsin2[][]);
fscanf(openfile1,"%d ",&no_in_file1);
//puts(take_input);
fscanf(openfile2,"%d ",&no_in_file2);
//puts(take_input);

//read_input(wordsin1[][],no_in_file1,wordsin2[][]

for(iterate1=0;iterate1<no_in_file1;iterate1++)
{
fscanf(openfile1,"%s ",wordsin1[iterate1]);
}
for(iterate2=0;iterate2<no_in_file2;iterate2++)
{
fscanf(openfile2,"%s ",wordsin2[iterate2]);
}

//printf("%s %s\n",wordsin1[0],wordsin2[0]);
sort(no_in_file1-1,wordsin1,0,no_in_file1-1);
uniquein1=find_unique(no_in_file1,wordsin1,unique1,freq1);
sort(no_in_file2-1,wordsin2,0,no_in_file2-1);
uniquein2=find_unique(no_in_file2,wordsin2,unique2,freq2);
printf("Unique words of File-1: {\n");
for( iter=0;iter<=uniquein1;iter++)
printf("%s (%d)\n",unique1[iter],freq1[iter]);
printf("}\nUnique words of File-2:{\n");
for(iter=0;iter<=uniquein2;iter++)
printf("%s (%d)\n",unique2[iter],freq2[iter]);
printf("}\n");
common=comp_unique(uniquein1+1,unique1,uniquein2+1,unique2);
printf("%d %d\n",uniquein1,uniquein2);
printf("similarity:%d %.2f\n",common,(float)common/(float)(uniquein1+1+uniquein2+1-common));
//printf("done successfully :)\n");
return 0;
}
