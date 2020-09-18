#include"include.h"
class IfConfig
{
public:
   void if_config()
   {
     struct ifreq ir[5];
     struct ifconf conf;
     conf.ifc_len=sizeof(ir);
     conf.ifc_req=ir;
     int sfd=socket(AF_INET,SOCK_STREAM,0);
     ioctl(sfd,SIOCGIFCONF,&conf);
     int cnt = conf.ifc_len/sizeof(struct ifreq);
     cout << cnt <<endl;
     for(int i=0;i<cnt ;i++ )
     {
       struct ifreq iq;
       memcpy(iq.ifr_name,ir[i].ifr_name,sizeof(ir[i].ifr_name));

       //打印各个网卡的网卡名称
       cout << iq.ifr_name <<endl;
       
       //获取ip地址
       cout << "ip地址: " <<endl;
       ioctl(sfd,SIOCGIFDSTADDR,&iq);
       struct sockaddr_in* addr = (struct sockaddr_in*)&iq.ifr_dstaddr;
       cout << inet_ntoa(addr->sin_addr)<<endl;

       //获取子网掩码
       cout << "子网掩码: " <<endl; 
       ioctl(sfd,SIOCGIFNETMASK,&iq) ;
       addr=(struct sockaddr_in*)&iq.ifr_netmask;
       cout << inet_ntoa(addr->sin_addr)<<endl;

       //获取广播地址
       cout << "广播地址: "<<endl;
       ioctl(sfd,SIOCGIFBRDADDR,&iq);
       addr=(struct sockaddr_in*)&iq.ifr_netmask;
       cout << inet_ntoa(addr->sin_addr)<<endl;

       //获取MTU
       cout << "MTU: " ;
       ioctl(sfd,SIOCGIFMTU,&iq);
       cout << iq.ifr_mtu<<endl;


       //获取MAC地址
       ioctl(sfd,SIOCGIFHWADDR,&iq);
       char buf[6];
       memcpy(buf,iq.ifr_hwaddr.sa_data,6);
       cout << "MAC地址: " <<endl;
       for(int i=0;i<6;i++)
       {
         if(i<5)
         printf("%02X:",buf[i]);
         else 
         {
           printf("%02X\n",buf[i]);
         }
       }
       cout << endl <<endl;
     }
   }
};
