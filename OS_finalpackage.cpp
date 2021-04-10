#include <iostream>
using namespace std;
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>
pthread_mutex_t lock;
//#define Frame_num 5

int page_table[1000][5];
int total_pages;
int counter[1000][17];
int bits;
int frame_acc;
int frames = 5;
int p_fault;

int replacement(int r,int m)
{
        int i,val,ret,min,j;
        int check[1000];
        for(i=0; i < total_pages ;i++)
        {
                check[i] = 10000;
                if(page_table[i][0]==1)
                {
                        val = 0;
                        for(j=0; j< bits ;j++)
                        {
                                val = val + counter[i][(bits - j - 1)] * pow(2,j);
                        }
                        check[i] = val;
                }
        }
        min = 10000;
        for(i=0; i<total_pages ;i++)
        {
                if(check[i] < min)
                {
                        min = check[i];
                        ret = i;
                }
        }
        return ret;
}


int clockIncrease(int p)
{
        int i;
        for(i=0 ; i<total_pages ; i++)
        {
                if(page_table[i][0] == 1  && i!= p)
                {
                        page_table[i][2] = 0;
                }
        }
}

int shift()
{
        int i,j;
        for (i=0;i<total_pages ; i++)
        {
                if(page_table[i][0] == 1)
                {
                        for(j=bits - 1;j > 0 ;j--)
                        {
                                counter[i][j] = counter[i][j - 1];
                        }
                        counter[i][0] = page_table[i][2];
                        page_table[i][2] = 0;
                }
        }
}

void print()
{
        int i,j;
        for(i=0; i<total_pages ; i++)
        {
                for(j=0;j<bits;j++)
                {
                        cout<<counter[i][j];
                }
                cout<<" ";
        }
        cout<<"\n\n";
}

void setcounter(int pno)
{
        int i;
        for(i=0;i<bits;i++)
        {
                counter[pno][i] = 0;
        }
}

void accessPT(int pno,int rw)
{
        pthread_mutex_lock(&lock);

        int i,k,r,m,frame,j;
        int ret = 10000;
        if(page_table[pno][0] == 1)
        {
                cout<< "Page " << pno <<" present in the table" << "|| Mode: "<<rw << " || "<< endl;
                if(frame_acc < frames + 1)
                        frame_acc = frame_acc - 1;
                k=0;
                if(rw==0)
                        page_table[pno][2] = 1;
                if(rw==1)
                {
                        page_table[pno][1] = 1;
                        page_table[pno][2] = 1;
                }
        }
        else
        {
                if(frame_acc < frames + 1 )
                {
                        cout<<"Page fault for page "<< pno << "|| Mode: "<< rw<< " || ";
                        cout<< " No page removed"<<endl;
                        page_table[pno][0] = 1;
                        page_table[pno][1] = 0;
                        page_table[pno][2] = 1;
                        page_table[pno][3] = frame_acc;
                        p_fault = p_fault + 1;
                }
                else
                {

                        cout<< "Page fault for page "<< pno << "|| Mode: "<< rw<< " || ";
                        ret = replacement(r,m);
                        page_table[ret][0] = 0;
                        page_table[ret][1] = 0;
                        page_table[ret][2] = 0;
                        frame = page_table[ret][3];
                        page_table[ret][3] = 0;
                        setcounter(ret);
                        cout << "Page " << ret <<" removed from frame"<< endl;

                        page_table[pno][0] = 1;
                        page_table[pno][1] = 0;
                        page_table[pno][2] = 1;
                        page_table[pno][3] = frame_acc;
                        p_fault = p_fault + 1;
                }

        }
        clockIncrease(pno);
        shift();
        print();
        pthread_mutex_unlock(&lock);
}

void *page(void * arg)
{
        int *page_no =(int *)arg;
        int rw = rand() % 2;
        accessPT(*page_no,rw);
}

void q2()
{
        //int frames = 5;  //Frame_num;
        p_fault = 0;
        int i,j;
        int arr[50];
        time_t t;
        srand((unsigned) time(&t));
        int num_pages = 12;
        total_pages = num_pages;
        int leng = 20;
        bits = 8;
        cout<<"Enter total number of pages"<<endl;
        cin>>num_pages;
        cout<<"Enter total number of frames"<<endl;
        cin>>frames;
        cout<<"Enter size of reference string"<<endl;
        cin>>leng;
        cout<<"Enter total number of bits in counter"<<endl;
        cin>>bits;

        cout<<"Total number of Pages : "<<num_pages<<endl;
        cout<<"Total frames : "<<frames<<endl;
        pthread_t t_id[100];
        for(i=0; i< num_pages ;i++)
        {
                page_table[i][0] = 0;
                page_table[i][1] = 0;
                page_table[i][2] = 0;
                page_table[i][3] = 0;
                for(j=0;j<8;j++)
                {
                        counter[i][j] = 0;
                }
        }
        int check = 100;
        cout<<"Reference String : "<<endl;
        for(i=0; i < leng ; i++)
        {
                while(1)
                {
                        arr[i] = rand() % num_pages;
                        if( i!=0)
                                check = arr[i];
                        if(check != arr[i-1])
                                break;
                }
                cout << arr[i] << " ";
        }
        frame_acc = 0;
        cout<<endl;
        for(i=0; i<leng; i++)
        {
                frame_acc = frame_acc + 1;
                pthread_create(&t_id[i],NULL,page,(void *) &arr[i]);
                sleep(1);
        }
        for(i=0; i< leng ; i++)
        {
                pthread_join(t_id[i],NULL);
        }
        cout << "Total page fault are : " << p_fault<<endl;
        pthread_mutex_destroy(&lock);

}
//pthread_mutex_t lock;
//#define Frame_num 5

//int page_table[1000][5];
//int total_pages;
//int frame_acc;
//frames = 5;
//int p_fault;

int replacement1(int r,int m)
{
        int i;
        for(i=0; i < total_pages ;i++)
        {
                if(page_table[i][0]==1)
                {
                        if(page_table[i][1] == m && page_table[i][2] == r)
                        {
                                return i;
                        }
                }
        }
        return 10000;
}

int timeCheck1(int p)
{
        int i;
        for(i=0 ; i<total_pages ; i++)
        {
                if(page_table[i][0] == 1)
                {
                        if(page_table[i][4] == 6)
                        {
                                page_table[i][2] = 0;
                                page_table[i][4] = 0;
                        }
                }
        }
}

int clockIncrease1(int p)
{
        int i;
        for(i=0 ; i<total_pages ; i++)
        {
                if(page_table[i][0] == 1  && i!=p)
                {
                        page_table[i][4] = page_table[i][4] + 1;
                }
        }
}

void accessPT1(int pno,int rw)
{
        pthread_mutex_lock(&lock);

        int i,k,r,m,frame,j;
        int ret = 10000;
        timeCheck1(pno);
        if(page_table[pno][0] == 1)
        {
                cout<< "Page " << pno <<" present in the frame" << "|| Mode: "<<rw << " || "<< endl;
                if(frame_acc < frames + 1)
                        frame_acc = frame_acc - 1;
                k=0;
                if(rw==0)
                        page_table[pno][2] = 1;
                        page_table[pno][4]= 0;
                if(rw==1)
                {
                        page_table[pno][1] = 1;
                        page_table[pno][2] = 1;
                        page_table[pno][4] = 0;
                }
        }
        else
        {
                if(frame_acc < frames + 1 )
                {
                        cout<<"Page fault for page "<< pno << "|| Mode: "<< rw<< " || ";
                        cout<< " No page removed"<<endl;
                        page_table[pno][0] = 1;
                        page_table[pno][1] = 0;
                        page_table[pno][2] = 0;
                        page_table[pno][3] = frame_acc;
                        page_table[pno][4] = 0;
                        p_fault = p_fault + 1;
                }
                else
                {

                        cout<< "Page fault for page "<< pno << "|| Mode: "<< rw<< " || ";
                        for(i=0;i<2;i++)
                        {
                                for(j=0;j<2;j++)
                                {
                                        r = i;
                                        m = j;
                                        ret = replacement1(r,m);
                                        if(ret != 10000 )
                                                break;
                                }
                                if(ret != 10000)
                                        break;
                        }
                        page_table[ret][0] = 0;
                        page_table[ret][1] = 0;
                        page_table[ret][2] = 0;
                        frame = page_table[ret][3];
                        page_table[ret][3] = 0;
                        page_table[ret][4] = 0;
                        cout << "Page " << ret <<" removed from frame"<< endl;

                        page_table[pno][0] = 1;
                        page_table[pno][1] = 0;
                        page_table[pno][2] = 0;
                        page_table[pno][3] = frame;
                        page_table[pno][4] = 0;
                        p_fault = p_fault + 1;
                }

        }
        clockIncrease1(pno);
        pthread_mutex_unlock(&lock);
}

void *page1(void * arg)
{
        int *page_no =(int *)arg;
        int rw = rand() % 2;
        accessPT1(*page_no,rw);
}

void q1()
{
        //int frames = 5;  //Frame_num;
        p_fault = 0;
        int i;
        int arr[50];
        time_t t;
        srand((unsigned) time(&t));
        int num_pages = 12;
        total_pages = num_pages;
        int leng = 20;
        cout<<"Enter total number of pages"<<endl;
        cin>>num_pages;
        cout<<"Enter total number of frames"<<endl;
        cin>>frames;
        cout<<"Enter length of reference string"<<endl;
        cin>>leng;

        cout<<"Total number of Pages : "<<num_pages<<endl;
        cout<<"Total frames : "<<frames<<endl;
        pthread_t t_id[100];
        for(i=0; i< num_pages ;i++)
        {
                page_table[i][0] = 0;
                page_table[i][1] = 0;
                page_table[i][2] = 0;
                page_table[i][3] = 0;
                page_table[i][4] = 0;
        }
        int check = 100;
        cout<<"Reference String : "<<endl;
        for(i=0; i < leng ; i++)
        {
                while(1)
                {
                        arr[i] = rand() % num_pages;
                        if( i!=0)
                                check = arr[i];
                        if(check != arr[i-1])
                                break;
                }
                cout << arr[i] << " ";
        }
        frame_acc = 0;
        cout<<endl;
        for(i=0; i<leng; i++)
        {
                frame_acc = frame_acc + 1;
                pthread_create(&t_id[i],NULL,page1,(void *) &arr[i]);
                sleep(1);
        }
        for(i=0; i< leng ; i++)
        {
                pthread_join(t_id[i],NULL);
        }
        cout << "Total page fault are : " << p_fault<<endl;
        pthread_mutex_destroy(&lock);

}
int check_for_fault(int t,int f, int* arr1,int*arr2)

{
        int flag=0;
        for(int i=0;i<f;i++)

        {
                if(t==arr1[i])
                {
                        arr2[i]=1;

                        flag=1;
                        break;
                }
        }
        if(flag==1)
        {


                return 0;
        }
        else{
        return 1;
        }
}
int* fn3(int t,int f,int idx,int arr1[],int arr2[])
{
        int *newarr=new int[2];
        int i;
        for(i=0;i<f;i++)
        {
                if(arr2[idx]==0)

                {
                        newarr[0]=arr1[idx];
                        arr1[idx]=t;
                        newarr[1]=(idx+1)%f;
                        return newarr;
                }
                else{
                arr2[idx]=0;
                idx=(idx+1)%f;}
        }
}
void q3(){
        int vpages,pframes;

        cout<<"Enter the number of virtual pages"<<endl;
        cin>>vpages;
        cout<<"Enter the number of page frames"<<endl;
        cin>>pframes;

        int n_ref=0;
        cout<<"Enter the number of references"<<endl;

        cin>>n_ref;
        int ref_string[n_ref];
        int i;
        for(i=0;i<n_ref;i++)
        {
                printf("\nEnter the page referenced(between 0 and (num_of_virtual_pages-1)):");
                cin>>ref_string[i];
        }
        int *r=ref_string;
        int f=pframes;
        int c=n_ref;
        int faults_count=0;

        int arr[f];
        int sc[f];
        int idx=0;
        int j;
        for(j=0;j<f;j++)

        {

                sc[j]=0;
        }
        for(i=0;i<f;i++)
        {

                arr[i]=-999;
        }
        for(i=0;i<c;i++)
        {
                cout<<"Page Referenced:"<<r[i]<<endl;
                if(check_for_fault(r[i],f,arr,sc))

                {     int *ptr=fn3(r[i],f,idx,arr,sc);

                        idx=ptr[1];

                        if(ptr[0]==-999){

                        cout<<"Page Fault: no page evicted, page "<<r[i]<<" brought in to memory."<<endl;}

                        else{

                        cout<<"Page Fault: page " <<ptr[0]<<" evicted, page "<<r[i]<<" brought in to memory."<<endl;

                        }

                        faults_count++;

                }
                else{cout<<"No page fault on referencing page "<<r[i]<< " using Second chance page replacement algorithm"<<endl;

               }
        }

        cout<<"Total no of page faults is:"<<faults_count<<endl;


}
int main()
{
        int opt;
        cout<<"1. NRU Page replacement algorithm\n";
        cout<<"2. Clock Page replacement algorithm\n";
        cout<<"3. Aging Page replacement algorithm\n";
        cin>>opt;
        if(opt==1) q1();
        else if (opt==2) q3();
        else if(opt==3) q2();
}