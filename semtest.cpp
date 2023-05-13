#include<iostream>
#include<semaphore.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<string>

using namespace std;

sem_t semaphore[5];
sem_t mainSem;

struct student{
    string name;
    int groupid;
};

void* demo_func(void* args){
    struct student* s = (struct student*)args;
    int value;
    //wait for the group memeber to take demo
    sem_wait(&mainSem);
    sem_getvalue(&mainSem, &value);
    // while(value < 5);
    sem_wait(&semaphore[s->groupid]);
    // cout <<"Some one from group: "<< s->groupid << " giving demo "<< endl;

    sleep(1);
    //release the semaphore
    sem_post(&semaphore[s->groupid]);
    cout<<value<<endl;
    // cout<<"Demo completed of some one from group: "<<s->groupid<<endl;
    sem_post(&mainSem);
    pthread_exit(NULL);
}


int main(){

    sem_init(&mainSem,0,5);
    //each semaphore represents the group 
    sem_init(&semaphore[0],0,1);
    sem_init(&semaphore[1],0,1);
    sem_init(&semaphore[2],0,1);
    sem_init(&semaphore[3],0,1);
    sem_init(&semaphore[4],0,1);

    //making 25 studenets and creating a total of 5 groups
    student stds[10];
    for(int i = 0; i< 10; i++){
        stds[i].groupid = (i + 5) / 5;
    }
    //there are a total of 25 threads
    //each thread is a student



    pthread_t thread[10];
    for(int j=0;j<10;j++){
        pthread_create(&thread[j],NULL,demo_func,&stds[j]);
    }
    for(int j=0;j<10;j++){
        pthread_join(thread[j],NULL);
    }


    //destroying the semaphores
    for(int i=0;i<5;i++){
        sem_destroy(&semaphore[i]);
    }

    return 0;
  
}