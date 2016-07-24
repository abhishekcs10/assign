/**
File: assign1_16CS60R83
@Author: Abhishek Tiwari


*/
#include<stdio.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<math.h>
#include<sys/wait.h>
#include<time.h>


/**
function: merge
task: merge two sorted parts in an array, first from [start..mid] and second from [mid+1..end]
arguements:
1- number of words in file
2- array of words
3- start address
4- mid
5- end

*/

void merge(int file_size,char words[][50],int start,int mid,int end)
{
	int left=mid-start+1;
	int right=end-mid,i,j;
	char word_l[500][50],word_r[500][50];
	for(i=0;i<left;i++)
	{
		strcpy(word_l[i],words[start+i]);
	}
	for( i=0;i<right;i++)
	{
		strcpy(word_r[i],words[mid+i+1]);
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
	while(i<left)
	{
		strcpy(words[k++],word_l[i]);
		i++;

	}
	while(j<right)
	{
		strcpy(words[k++],word_r[j]);
		j++;
	}

}

/**
function: sort
task: sort the given array of words
arguements: 
1- number of words in array
2- array of words
3- nu

*/
void sort(int file_size,char file[][50],int start,int end)
{
	if(start==end)
		return;
	int mid=(start+end)/2;
	sort(file_size,file,start,mid);
	sort(file_size,file,mid+1,end);

	merge(file_size,file,start,mid,end);


}




int find_unique(int num,char words[][50],char unique[][50],int freq[])
{
	int i,k=0;
	for(i=0;i<num;i++)
		freq[i]=1;
	strcpy(unique[k],words[0]);
	for(i=0;i<num-1;i++)
	{
		//if(!i)
		//	strcpy(unique[k],words[i]);

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


//new for forking

void find_uniq_each(int size,char words[][50],int start,int end,char file_cr[])
{
	FILE *fp=fopen(file_cr,"w+");

	int freq[500];
	int i,k=0;
	char uniq[500][50];
	for(i=0;i<size;i++)
		freq[i]=1;
	//
	strcpy(uniq[k],words[start]);
	for(i=start;i<end;i++) //comapre i with i+1
	{
		//if(i==start)
		//	strcpy(uniq[k],words[i]);

		if(strcmp(words[i],words[i+1])==0)
		{
			freq[k]++;
		}
		else
		{
			k++;
			strcpy(uniq[k],words[i+1]);
		}
	}
	//return k;
	fprintf(fp,"%d\n",k+1);
	for(i=0;i<=k;i++)
	{
		fprintf(fp,"%s %d\n",uniq[i],freq[i]);

	}
	fclose(fp);
	return;
}

//merge files
int merge_file(int file_size,char words[][50],int freq[],int start,int mid,int end)
{
	if(mid-1<0)
		return end+1;

	int left=mid-start+1;
	int right=end-mid,i,j;
	//printf("came\n");
	char word_l[500][50],word_r[500][50];
	int freq_left[500],freq_right[500];
	for(i=0;i<left;i++)
	{
		strcpy(word_l[i],words[start+i]);
		freq_left[i]=freq[start+i];
		//printf("left %s\n",word_l[i]);
	}
	for( i=0;i<right;i++)
	{
		strcpy(word_r[i],words[mid+i+1]);
		freq_right[i]=freq[mid+i+1];
		//printf("right %s\n",word_r[i]);
	}

	int k=start;
	for( i=0,j=0;i<left&&j<right;)
	{
		int cmp=strcmp(word_l[i],word_r[j]);
		if(cmp<0)
		{
			strcpy(words[k],word_l[i]);	
			freq[k]=freq_left[i];
			i++;k++;
		}
		else if (cmp>0){
			strcpy(words[k],word_r[j]);
			freq[k]=freq_right[j];
			j++;k++;
		}
		else{
			strcpy(words[k],word_r[j]);
			freq[k]=freq_right[j]+freq_left[i];
			i++;j++;k++;
		}

	}
	//printf("aisa %d %d\n",i,j);
	while(i<left)
	{
		strcpy(words[k],word_l[i]);
		freq[k]=freq_left[i];
		i++;k++;

	}
	//printf("h\n");
	while(j<right)
	{
		strcpy(words[k],word_r[j]);
		freq[k]=freq_right[j];
		j++;k++;
	}

	//printf("n");
	//for(i=0;i<k;i++)
	//printf("%s\n",words[i]);
	return k;//total number
}




int main()
{
	clock_t start=clock(),end;
	FILE *openfile1,*openfile2,*output_file,*inp_file;
	char filename1[50],filename2[50];
	char take_input[50],inp_buffer[50];
	int no_in_file1,no_in_file2,iterate1,iterate2,iter,common;
	char wordsin1[500][50],wordsin2[500][50],unique1[500][50],unique2[500][50],filename[50];
	int freq1[500],freq2[500],uniquein1,uniquein2;
	int proc_iter,chunks,size_chunk,status;
	int op1,op2,file_iter,file_size,iter_op1,iter_op2;
	double time_spent;

	pid_t pids,proc[500],proc2[500];
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
	scanf("%d",&chunks);
	if(no_in_file1%chunks==0)
		size_chunk=no_in_file1/chunks;
	else
		size_chunk=(no_in_file1/chunks)+1;
	for(proc_iter=0;proc_iter<chunks;proc_iter++)
	{
		proc[proc_iter]=fork();
		if(proc[proc_iter]==0)
		{
			sprintf(filename,"FILEOP_%d",proc_iter);
			if(proc_iter==chunks-1)
			{

				sort(no_in_file1-proc_iter*size_chunk,wordsin1,proc_iter*size_chunk,no_in_file1-1);
				find_uniq_each(no_in_file1-proc_iter*size_chunk,wordsin1,proc_iter*size_chunk,no_in_file1-1,filename);
			}
			else
			{
				sort(size_chunk-1,wordsin1,proc_iter*size_chunk,(proc_iter+1)*(size_chunk)-1);
				find_uniq_each(size_chunk,wordsin1,proc_iter*size_chunk,(proc_iter+1)*(size_chunk)-1,filename);
			}
			exit(0);
		}
	}
	//second file

	if(no_in_file2%chunks==0)
		size_chunk=no_in_file2/chunks;
	else
		size_chunk=(no_in_file2/chunks)+1;
	for(proc_iter=0;proc_iter<chunks;proc_iter++)
	{
		proc2[proc_iter]=fork();
		if(proc2[proc_iter]==0)
		{
			sprintf(filename,"FILEOP2_%d",proc_iter);
			if(proc_iter==chunks-1)
			{
				sort(no_in_file2-proc_iter*size_chunk,wordsin2,proc_iter*size_chunk,no_in_file2-1);
				find_uniq_each(no_in_file2-proc_iter*size_chunk,wordsin2,proc_iter*size_chunk,no_in_file2-1,filename);
			}
			else
			{
				sort(size_chunk-1,wordsin2,proc_iter*size_chunk,(proc_iter+1)*(size_chunk)-1);
				find_uniq_each(size_chunk,wordsin2,proc_iter*size_chunk,(proc_iter+1)*(size_chunk)-1,filename);
			}
			exit(0);
		}
	}

	for(proc_iter=0;proc_iter<chunks;proc_iter++)
	{
		waitpid(proc2[proc_iter],&status,WIFEXITED(&status));
		waitpid(proc[proc_iter],&status,WIFEXITED(&status));
	}


	// merging the files of files 1
	op1=0;
	//op2=0;
	memset(wordsin1,0,sizeof wordsin1);
	output_file=fopen("final.op","w");
	for(proc_iter=0;proc_iter<chunks;proc_iter++)
	{

		sprintf(filename,"FILEOP_%d",proc_iter);
		inp_file=fopen(filename,"r");
		fscanf(inp_file,"%d",&file_size);
		for(file_iter=0;file_iter<file_size;file_iter++)
		{
			fscanf(inp_file,"%s %d",wordsin1[file_iter+op1],&freq1[file_iter+op1]);

		}

		op1 = merge_file(file_size+op1,wordsin1,freq1,0,op1-1,file_size+op1-1);
		//for(iter_op1=0;iter_op1<op1;iter_op1++)
		//printf("%s %d\n",wordsin1[iter_op1],freq1[iter_op1]);

		//printf("\n");
		fclose(inp_file);
	}

	//print op
	fprintf(output_file,"%d\n",op1);
	for(iter_op1=0;iter_op1<op1;iter_op1++)
	{
		fprintf(output_file,"%s %d\n",wordsin1[iter_op1],freq1[iter_op1]);
	}



	//merging the output files 2


	op2=0;
	//op2=0;
	memset(wordsin2,0,sizeof wordsin2);
	output_file=fopen("final2.op","w");
	for(proc_iter=0;proc_iter<chunks;proc_iter++)
	{

		sprintf(filename,"FILEOP2_%d",proc_iter);
		inp_file=fopen(filename,"r");
		fscanf(inp_file,"%d",&file_size);
		for(file_iter=0;file_iter<file_size;file_iter++)
		{
			fscanf(inp_file,"%s %d",wordsin2[file_iter+op2],&freq2[file_iter+op2]);

		}

		op2 = merge_file(file_size+op2,wordsin2,freq2,0,op2-1,file_size+op2-1);
		//for(op2=0;op2<op1;op2++)
		//printf("%s %d\n",wordsin1[op2],freq1[op2]);

		//printf("\n");
		fclose(inp_file);
	}

	//print op
	fprintf(output_file,"%d\n",op2);
	for(iter_op2=0;iter_op2<op2;iter_op2++)
	{
		fprintf(output_file,"%s %d\n",wordsin2[iter_op2],freq2[iter_op2]);
	}




	/*sort(no_in_file1-1,wordsin1,0,no_in_file1-1);
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
	 */
	common=comp_unique(op1,wordsin1,op2,wordsin2);
	printf("%d %d\n",op1,op2);
	printf("similarity:%d %.2f\n",common,(float)common/(float)(op1+op2-common));


end=clock();
time_spent=((double)(end-start))/CLOCKS_PER_SEC;
printf(" Elapsed %lf seconds\n",time_spent);
	printf("done successfully :)\n");
	return 0;
}
