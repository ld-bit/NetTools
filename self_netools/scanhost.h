#include"include.h"
#include<time.h>
#define PACK_LEN 72
class ScanHost
{
public:
   void scHost()
   {
     char buf[32];
     printf("请输入你要扫描的网段: ");
     scanf("%s",buf);
     int sfd=socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
     char ipaddr[32];
     for(int i=1;i<255;i++)
     {
       //先装包
       memset(ipaddr,0x00,32);
       sprintf(ipaddr,"%s.%d",buf,i);
       printf("正在扫描: %s\n" , ipaddr);
       char send_buf[PACK_LEN];
       memset(send_buf,0x00,sizeof(send_buf));
       struct sockaddr_in addr;
       memset(&addr,0x00,sizeof(addr));
       addr.sin_family=AF_INET;
       inet_aton(ipaddr,&addr.sin_addr);
       for(int j=1;j<3;j++)
       {
         make_icmp_packet((struct icmp*)send_buf,PACK_LEN,j); //封包
         if(sendto(sfd,send_buf,PACK_LEN,0,(struct sockaddr*)&addr,sizeof(addr))==-1)
         {
           perror("sendto") ,exit(-1);
         }
         fd_set rset;
         FD_ZERO(&rset);
         FD_SET(sfd,&rset);
         struct timeval tv;
         tv.tv_sec=0;
         tv.tv_usec=200000;
         while(1)
         {
           int r;
           if((r=select(sfd+1,&rset,NULL,NULL,&tv))<=0)  
           {
             break;
           }
           char rcv_buf[2048];
           if(recvfrom(sfd,rcv_buf,2048,0,NULL,NULL)==-1)  //获取返回数据
           {
             exit(0);
           }
           struct ip* pip=(struct ip*)rcv_buf; 
            int len  = pip->ip_hl << 2;
           if(pip->ip_src.s_addr==addr.sin_addr.s_addr)
           {
            struct icmp* picmp=(struct icmp*)(rcv_buf+len);
            if(picmp->icmp_type==ICMP_ECHOREPLY)
            {
              printf("%s\n",inet_ntoa(pip->ip_src));
              struct timeval cur;
              gettimeofday(&cur,NULL);
              tvsub(&cur,(struct timeval*)(picmp->icmp_data));
              printf("ttl: %d\n rtt: %lf\n",pip->ip_ttl,(double)cur.tv_sec+(double)cur.tv_usec/1000);
            }
            goto lab;
           }
         }
       }
      lab:;
     }
   }
   void make_icmp_packet(struct icmp* picmp,int len,int n)
   {
      memset(picmp,0x00,len);
      gettimeofday((struct timeval*)(picmp->icmp_data), NULL);
      picmp->icmp_type=ICMP_ECHO;
      picmp->icmp_code=0;
      picmp->icmp_cksum=0;
      picmp->icmp_id=getpid();
      picmp->icmp_seq=n;
      picmp->icmp_cksum=checksum((u_short*)picmp,len);
   }
   int checksum(u_short* data,int len)
   {
    u_long sum=0;
    for(;len>1;len-=2)
    {
       sum+=*data++;
       if(sum&0x800000000)
       {
         sum=(sum&0xffff)+(sum>>16);
       }
       if(len==1)
       {
        u_short i = 0;
        *(u_char*)&i=*(u_char*)data;
        sum+=i;
       }
       while(sum>>16)
       {
          sum = (sum&0xffff) + (sum>>16);    
       }
    }
       return (sum == 0xffff) ? sum : ~sum; 
   }
   void tvsub(struct timeval* out ,struct timeval * in)
   {
     out->tv_sec-=in->tv_sec;
   }
};
