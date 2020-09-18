#include "all.h"
#include<signal.h>
#include<sys/wait.h>
class  NetTools 
{
 public:
int menu() {
		printf("##################################################\n");
		printf("# 1. myifconfig                                  #\n");
		printf("# 2. scanhost                                    #\n");
		printf("# 3. scanport                                    #\n");
		printf("# 4. shark                                       #\n");
		printf("# 5. quit                                        #\n");
		printf("##################################################\n");
    int chose=0;
    cin >> chose;
    return chose;
}
void do_proc(int chose)
{
  
   pid_t pid= fork();
   if(pid==0)
   {
   sigset_t sig;
   sigemptyset(&sig);
   sigaddset(&sig,SIGINT);
   sigprocmask(SIG_UNBLOCK,&sig,NULL);
   if(chose==1)
   {
    IfConfig cof;
    cof.if_config();
   }
   else if(chose==2)
   {
     ScanHost sh;
     sh.scHost();
   }
   else if(chose ==3 )
   {
    ScanPort sp;
    sp.scan_port();
   }
   else if(chose==4)
   {
    Shark sk;
    sk.shark();
   }
   cout << "按任意键退出..." <<endl;
   getchar();
   getchar();
  }
  else
  {
    wait(NULL);
  }
}
void play() {
  sigset_t sig;
  sigemptyset(&sig);
  sigaddset(&sig,SIGINT);
  sigprocmask(SIG_BLOCK,&sig,NULL);
	while ( 1 ) {
		int chose = menu();
    if(chose==5)
    {
     cout << "谢谢使用 " <<endl;
     exit(0);
    }
    else if(chose == 1 || chose ==2 ||chose ==3 ||chose ==4 )
    {
     do_proc(chose);
     system("clear");
    }
    else 
    {
      cout << "您的输入有误,请重新输入.." <<endl;
    }
	}
}
};
int main()
{
  NetTools nts;
  nts.play();
  return 0;
}
