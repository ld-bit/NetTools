#include"include.h"
class Shark
{
public:
   void printmac(struct ethhdr* eth) //打印max地址
   {
     cout << "#####################"<<endl;
     cout << "链路层信息: " <<endl; 
     cout << "源MAC地址: ";
     for(int i=0;i<6;i++)
     {
     if(i<5)
     cout << hex << (int)eth->h_source[0] << ":"; // hex 表示按照16进制输出
     else 
     {
       cout << hex << (int)eth->h_source[5] <<endl;
     }
     }
     cout << "目标MAC地址: ";
     for(int i=0;i<6;i++)
     {
     if(i<5)
     cout << hex << (int)eth->h_dest[0] << ":";
     else 
     {
       cout << hex << (int)eth->h_dest[5] <<endl;
     }
     }
     cout << "#####################"<<endl;
   }
   void print_ip(struct iphdr* iph)
   {
     cout << "#####################"<<endl;
     printf("ip协议信息\n");
     struct in_addr ad;
     ad.s_addr=iph->saddr;
     cout << "源ip地址: " << inet_ntoa(ad) <<endl;
     ad.s_addr=iph->daddr;
     cout << "目标ip地址: " << inet_ntoa(ad) << endl;
     cout <<"ttl: ";
     printf("%d\n",iph->ttl);
     cout << "#####################"<<endl;
   }
   void shark()    
   {
    int lfd=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL)); //AF_PACKET 表示要抓获取帧信息  SOCK_RAW 表示获取数据报
    char* buf=(char*)malloc(MAX_SIZE);
    while(1)
    {
      memset(buf,0x00,MAX_SIZE);
      int r = read(lfd,buf,MAX_SIZE);
      if(r<=0) break;
      struct ethhdr* eth = (struct ethhdr*)buf;
      printmac(eth);
      if(ntohs(eth->h_proto)==ETH_P_IP)
      {
        struct iphdr* iph=(struct iphdr*)(buf+sizeof(struct ethhdr));
        print_ip(iph);
       if(iph->protocol==IPPROTO_TCP)
       {
       print_tcp((struct tcphdr*)(buf+sizeof(struct ethhdr)+sizeof(struct iphdr)));
       }
        else if(iph->protocol==IPPROTO_UDP)
        {
          struct udphdr* udp = (struct udphdr*)(buf+sizeof(struct ethhdr)+sizeof(struct iphdr));
          print_udp(udp);
        }
        else if(iph->protocol==IPPROTO_ICMP)
        {
          struct icmphdr* icmp=(struct icmphdr*)(buf+sizeof(struct ethhdr)+sizeof(struct iphdr));
          print_icmp(icmp);
        }
      }
      cout << endl <<endl;
    }
   }
   void print_icmp(struct icmphdr* icmp)
   {
     cout << "#####################"<<endl;
     cout << "icmp协议信息: "<<endl;
     cout << "类型: ";
     printf("%d\n",icmp->type);
     cout << "代码: "; 
     printf("%d\n",icmp->code);
     cout << "#####################"<<endl;
   }
   void print_tcp(struct tcphdr* tcp)
   {
     cout << "#####################"<<endl;
    cout << "tcp协议信息: "<<endl;
    cout << "紧急指针(URG): ";
    if(tcp->urg==0)
    {
      cout << "无效" <<endl;
    }
    else 
      cout << "有效" <<endl;
    cout << "确认序号(ACK): ";
    if(tcp->ack==0)
    {
      cout << "无效" <<endl;
    }
    else 
      cout << "有效" <<endl;
    cout << "重新连接(RST): ";
    if(tcp->rst==0)
    {
      cout << "无效" <<endl;
    }
    else 
      cout << "有效" <<endl;
    cout << "发起连接(SYN): ";
    if(tcp->syn==0)
    {
      cout << "无效" <<endl;
    }
    else 
      cout << "有效" <<endl;
    cout << "断开连接(FIN): ";
    if(tcp->fin==0)
    {
      cout << "无效" <<endl;
    }
    else 
      cout << "有效" <<endl;
    cout << "源端口号: "; 
    printf("%d\n",ntohs(tcp->source));
    cout << "目标端口号: "; 
    printf("%d\n",ntohs(tcp->dest));
    cout << "#####################"<<endl;
   }
   void print_udp(struct udphdr* udp)
   {
     cout << "#####################"<<endl;
     cout <<"udp协议信息: " <<endl;
     cout << "源端口号: ";
    printf("%d\n",ntohs(udp->source));
     cout << "目标端口号: ";
    printf("%d\n",ntohs(udp->dest));
     cout << "#####################"<<endl;
   }
};
