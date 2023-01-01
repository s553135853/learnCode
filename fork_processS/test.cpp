#include <iostream>
#include <unistd.h>
#include <vector>
#include<chrono>
#include<thread>
int main()
{
  #if 0 
  printf("hello ubuntu\n");
  pid_t _pid = fork();
  if(_pid < 0)
  {	
  	printf("this process is fork failed\n");
  	return -1;
  }
  else if(_pid == 0)
  {	
  	size_t count=0;
  	while(++count<50)
  	{
  	printf("this process is son,the pid is [%d]\n",getpid());
  	sleep(1);  	  
  	}
  } 	
  else
  {	
  	size_t count=0;
  	while(++count<50)
  	{
  	printf("this process is parent,the pid is [%d]\n",getpid());
  	sleep(1);  	  
  	}
  	
  } 	
  #endif 
  #if 1
  std::vector<pid_t> _pids;
  pid_t _pid;
  //init 240 services,
  size_t services_count=240;
  for(size_t j=0;j<services_count;++j)
  {
    _pid = fork();
    if(_pid < 0)
    {
      printf("fork process is error ! \n");
      return -1;
    }
    else if(_pid == 0){
      //per service with only one process,so need to init service in here
      /*
      TO DO : 
      initService();
      offService();
      _pids.push_back(getpid()); 记录子进程的pid
      */
      #if 0
      for (size_t i = 0; i < 10; i++)
      {
         printf("this is son,pid is [%d]\n",getpid());
        _pids.push_back(getpid());
         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      }
      #endif
      break;
    }
    else
    {
      printf("this is parents,pid is [%d]\n",getpid());   
    }
  }
  #endif 
  for (auto &&i : _pids)
  {
     printf("pid: %d",i);
  }
  printf("pid size is [%ld]\n",_pids.size());
  #if 0
  auto _begainTime = std::chrono::system_clock::now(); 
  printf("hello world\n");
  // std::this_thread::sleep_for(50ms);
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  auto _endTime = std::chrono::system_clock::now();
  auto _mills = std::chrono::duration_cast<std::chrono::seconds>(_endTime-_begainTime);
  printf("use time is [%d]/ms\n",_mills.count());
  #endif
}
