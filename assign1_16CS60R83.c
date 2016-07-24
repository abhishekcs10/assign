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
3- start index
4- end index

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

/**
function: find_unique
task: find unique words from given sorted list of words adn store their frequency
arguements: 
1- number of words in sorted list
2- array of sorted words
3- resultant array having unique words
4- frequency of correspoding unique words

returns index of last unique word
*/


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



/**
function: comp_unique
task: compares unique words from file 1 and file 2 stored in uniq1 and uniq2
arguements: 
1- number of unique words in file 1
2- array of unique words
3- number of unique words in file 2
4- array of uniq words in file 2
returns number of common unique words in file1 and2
*/
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
/**
function : find_uniq_each
task: finds unique words and their frequencies for different chunks given to different processes
arguements:
1- size of file
2- array of words
3- start index
4- end index
5- file in which output is saved

*/
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
/**

function: merge_file
task: merge different chunks of file together mainting the property of uniqueness and counting their respective frequencies
arguements:
1- size of files merged+size of new chunk to be added
2- array of words to be merged
3- frequencies of the words
4- start index
5- index at which file devides [start..mid] and [mid+1..end]
6- end index
returns total unique words after merging
*/
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
	}
	for( i=0;i<right;i++)
	{
		strcpy(word_r[i],words[mid+i+1]);
		freq_right[i]=freq[mid+i+1];
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
	while(i<left)
	{
		strcpy(words[k],word_l[i]);
		freq[k]=freq_left[i];
		i++;k++;

	}
	while(j<right)
	{
		strcpy(words[k],word_r[j]);
		freq[k]=freq_right[j];
		j++;k++;
	}

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
	openfile1=fopen(filename1,"r");
	openfile2=fopen(filename2,"r");
	if(openfile1==NULL||openfile2==NULL)
	{
		printf("-1\n");
	}

	fscanf(openfile1,"%d ",&no_in_file1);
	fscanf(openfile2,"%d ",&no_in_file2);


	for(iterate1=0;iterate1<no_in_file1;iterate1++)
	{
		fscanf(openfile1,"%s ",wordsin1[iterate1]);
	}
	for(iterate2=0;iterate2<no_in_file2;iterate2++)
	{
		fscanf(openfile2,"%s ",wordsin2[iterate2]);
	}

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
		fclose(inp_file);
	}

	fprintf(output_file,"%d\n",op1);
	for(iter_op1=0;iter_op1<op1;iter_op1++)
	{
		fprintf(output_file,"%s %d\n",wordsin1[iter_op1],freq1[iter_op1]);
	}





	op2=0;
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
	printf("Unique words in file1: %d\nUnique words in file2: %d\n",op1,op2);
	printf("similarity: %.2f%%\n",(float)common/(float)(op1+op2-common)*100);


	end=clock();
	time_spent=((double)(end-start))/CLOCKS_PER_SEC;
	printf("Elapsed %lf seconds\n",time_spent);
	//printf("done successfully :)\n");
	return 0;
}
