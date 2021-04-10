#include <iostream>
using namespace std;
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>
pthread_mutex_t lock;

int page_table[1000][5];
int total_pages;                                                                //Global declaration of page table 
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
                check[i] = 10000;                               //sets a high value of 10000 so that min value can choosen
                if(page_table[i][0]==1)
                {
                        val = 0;
                        for(j=0; j< bits ;j++)
                        {
                                val = val + counter[i][(bits - j - 1)] * pow(2,j);    //converting binary value stored in counter to value
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
                        ret = i;                                                // returning minimum value to replace page               
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
                        page_table[i][2] = 0;                                   // setting referenced bit to zero
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
                                counter[i][j] = counter[i][j - 1];              //function to shift the counter to left and set rightmost bit to reference bit value
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
                        cout<<counter[i][j];                    //printing the value in the counter array
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
                counter[pno][i] = 0;                            //Initialy all value in counter set to zero
        }
}

void accessPT(int pno,int rw)                                   //function to access page table
{
        pthread_mutex_lock(&lock);                              //lock used so that multiple thraeds so not access the page at the same time

        int i,k,r,m,frame,j;
        int ret = 10000;
        if(page_table[pno][0] == 1)
        {
                cout<< "Page " << pno <<" present in the table" << "|| Mode: "<<rw << " || "<< endl;
                if(frame_acc < frames + 1)
                        frame_acc = frame_acc - 1;
                k=0;
                if(rw==0)                                                                          //If page is present in the frame no page fault
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
                        page_table[pno][2] = 1;                                                 //Initially only frames are allocated 
                        page_table[pno][3] = frame_acc;
                        p_fault = p_fault + 1;
                }
                else
                {

                        cout<< "Page fault for page "<< pno << "|| Mode: "<< rw<< " || ";
                        ret = replacement(r,m);                                                 //function returns index value of page to be replaced
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

void *page(void * arg)                                                  //thread for each page in the reference string
{
        int *page_no =(int *)arg;
        int rw = rand() % 2;
        accessPT(*page_no,rw);
}

void q2()
{
        p_fault = 0;
        int i,j;
        int arr[50];
        time_t t;
        srand((unsigned) time(&t));
        int num_pages = 12;
        total_pages = num_pages;
        int leng = 20;
        bits = 8;                                                               //user inputs taken 
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
                page_table[i][2] = 0;                                                           //Page table initial values set to zero
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
                        arr[i] = rand() % num_pages;                    //Random used to generate reference string
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

int replacement1(int r,int m)
{
        int i;
        for(i=0; i < total_pages ;i++)                                          //Not modified, Not Referenced ideal for replacement
                                                                                //Modified, Not Referenced prefered next
        {
                if(page_table[i][0]==1)                                         //Not modified, Referenced prefered next 
                {                                                               //Modified,Referenced considered last
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
                                page_table[i][2] = 0;                   //After 6 clock pulse reference bit set to zero
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
                if(page_table[i][0] == 1  && i!=p)                        //counter incremented so that after 6 clock pulse reference bit can be set to zero
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
                k=0;                                                                            //If page has a frame allocated not page fault
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

void *page1(void * arg)                                                                 //Thread created for each pagein reference string
{
        int *page_no =(int *)arg;
        int rw = rand() % 2;
        accessPT1(*page_no,rw);
}

void q1()                                                                       //NRU page replacement algorithm
{
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
        cin>>frames;                                                            //User inputs considered
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
                page_table[i][4] = 0;                                           //page table initially set to zero
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
        //loop through the page frame array and check if the referenced page is already present in the page frame array
        for(int i=0;i<f;i++)

        {
                if(t==arr1[i])//if the referenced element is already present then change the second chance bit to 1 and return 0
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
        else{//if the referenced element isnt present in the page frame array then return 0
        return 1;
        }
}
int* fn3(int t,int f,int idx,int arr1[],int arr2[])
{
        int *newarr=new int[2];//array to return the new index and to return the element being evicted.
        int i;
        for(i=0;i<f;i++)
        {
                //finding the page to be replaced by scanning the second chance array
                if(arr2[idx]==0)

                {
                        newarr[0]=arr1[idx];//store the element that was originally present in the position
                        arr1[idx]=t;//replace the element 
                        newarr[1]=(idx+1)%f;//increment the index to return
                        return newarr;//return the array with new index and the evicted page
                }
                else{
                arr2[idx]=0;//change the second chance bit to zero if it is already 1 since it has already been given one chance
                idx=(idx+1)%f;}//increment the index after changing the second chance bit
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
        int ref_string[n_ref];//to store the reference string
        int i;
        //getting the reference string
        for(i=0;i<n_ref;i++)
        {
                printf("\nEnter the page referenced(between 0 and (num_of_virtual_pages-1)):");
                cin>>ref_string[i];
        }
        int *r=ref_string;
        int f=pframes;
        int c=n_ref;
        int faults_count=0;

        int arr[f];//the main page frame array
        int sc[f];//second chance array
        int idx=0;//index for going in a circular order in the page frame array and to replace the elements according to clock page replacement algorithm 
        int j;
        for(j=0;j<f;j++)//initialising all second chance bits to zero

        {

                sc[j]=0;
        }
        //initialising all members initially in the page table to -999 to verify initial cases where the page fault has occurred but no page is evicted
        for(i=0;i<f;i++)
        {

                arr[i]=-999;
        }
        for(i=0;i<c;i++)
        {
                cout<<"Page Referenced:"<<r[i]<<endl;
                if(check_for_fault(r[i],f,arr,sc))//checking for page fault and proceeding to replace a page in the page frames array

                {     int *ptr=fn3(r[i],f,idx,arr,sc);

                        idx=ptr[1];

                        if(ptr[0]==-999){//if the element being replaced is -999 then there is no page evicted and the referenced page is brought into memory

                        cout<<"Page Fault: no page evicted, page "<<r[i]<<" brought in to memory."<<endl;} 

                        else{//otherwise if the page being evicted isnt -999 then the page being replaced is displayed and the referenced page is brought into memory 

                        cout<<"Page Fault: page " <<ptr[0]<<" evicted, page "<<r[i]<<" brought in to memory."<<endl;

                        }

                        faults_count++;//increment the number of page faults

                }
                else{cout<<"No page fault on referencing page "<<r[i]<< " using Second chance page replacement algorithm"<<endl;

               }
        }

        cout<<"Total no of page faults is:"<<faults_count<<endl;//display the number of page faults


}
int main()
{
        int opt;
        cout<<"1. NRU Page replacement algorithm\n";
        cout<<"2. Clock Page replacement algorithm\n";
        cout<<"3. Aging Page replacement algorithm\n";
        cin>>opt;
        if(opt==1) q1();//call the function for NRU page replacement algorithm
        else if (opt==2) q3(); //call the function for the clock page replacement algorithm
        else if(opt==3) q2();//call the function for the Aging page replacement algorithm
}
